# -*- coding: utf-8 -*-
"""
Created on Sat Apr 27 22:38:05 2013

@author: Martin Siggel
"""

import re
import copy

class CHeaderFileParser(object):
    '''
    Parses C-Header file code and creates a semantic
    This generated semantic can passed to wrapper generator
    classes in order to create bindings for the code
    '''

    def __init__(self, libname, libprefix):
        self.declarations = []
        
        # blacklist of methods not to be parsed
        self.blacklist = ()
        
        self.libname = libname
        self.prefix = libprefix
        self.lines = []
        self.decoration = 'DLL_EXPORT'
        self.enums = []
        self.typedefs = {}
        
    def read_header(self, filename):
        fop = open( filename, 'r')
        self.lines = fop.readlines()
        
        # remove // style comments
        for index, line in enumerate(self.lines):
            pos = line.find('//')
            if pos >= 0:
                self.lines[index] = line[:pos]
    
    def parse_enum_string(self, enumstr):
        # simplify
        thestr = copy.copy(enumstr).strip()
        thestr = thestr.replace('\n',' ')

        # remove comments
        match = re.compile(r'/\*(?P<comment>.*?)\*/')  
        #comments = match.findall(thestr)   
        thestr = match.sub('', thestr)
        
        # remove multpiple whitespaces
        thestr = re.compile(r'\s+').sub(' ', thestr)
        
        #extract name and value
        match = re.compile(r'enum\s+(?P<name>\w+)\s*?{(?P<values>.*?)}')
        res = match.search(thestr)
        
        values = res.group('values').split(',')
        for index, val in enumerate(values):
            values[index] = val.strip()
            
        return {'name': res.group('name'), 'values': values}
    
    def parse_methods(self):
        '''
        Parse methods and their annotations
        '''
        dec_pattern = re.compile(self.decoration + 
            r'\s+(?P<fundec>\w+\s+\w+\s*\([\w\s,*]*(\);)?)')
            
        anno_pattern = re.compile('#annotate.*?#')
        
        endterm = ');'        
        
        fundec_dict = []
        anno_dict = []
        
        # find functions
        continuing = False
        fundec = ''
        for index, line in enumerate(self.lines):
            if not continuing:
                match = dec_pattern.search(line)
                if match:
                    fundec = match.group('fundec').strip()
                    if not endterm in line:
                        continuing = True
                    else:
                        fundec_dict.append({'line': index, 'declaration': fundec})
            else:
                fundec += ' ' + line.strip()
                if endterm in line:
                    continuing = False
                    fundec_dict.append({'line': index, 'declaration': fundec})
        
        for index, line in enumerate(self.lines):
            match = anno_pattern.search(line)
            if match:
                anno_dict.append({'line': index, 'annotation': match.group()})
        
        # match annotation with functions
        anno_pos = 0
        for function in fundec_dict:
            dec = CFunctionDec()
            dec.parse_method_header(function['declaration'], self.typedefs, self.enums)
            if anno_pos < len(anno_dict) and anno_dict[anno_pos]['line'] < function['line']:
                anno = Annotation(anno_dict[anno_pos]['annotation'])
                dec.apply_annotation(anno)
                anno_pos += 1
                
            self.declarations.append(dec)
            
        for dec in self.declarations:
                PG = PythonGenerator('tixi')
                print PG.create_method_wrapper(dec)
                
    
    def add_alias(self, newname, basictype):
        self.typedefs[newname] = basictype
    
    
    def parse_defines(self):
        '''
        Parse typedefs, enums...
        '''
        
        enum_pattern = re.compile(r'\s*enum\s+(?P<name>\w+)')
        continuing = False
        enum_str = ''
        for line in self.lines:
            if not continuing:
                match = enum_pattern.match(line)
                if match:
                    continuing = True
                    enum_str = line
                    if "};" in line:
                        self.enums.append(self.parse_enum_string(enum_str))
            else:
                enum_str += line
                if "};" in line:
                    self.enums.append(self.parse_enum_string(enum_str))
                    continuing = False
                    
        # parse typedefs
        typedef_pattern = re.compile('typedef\s+(?P<type>[\w\s]+)\s+(?P<name>\w+)\s*?;')
        for line in self.lines:
            match = typedef_pattern.search(line)
            if match:
                self.typedefs[match.group('name')] =  match.group('type')
        
                
    
    def register_own_type(self, own_type, cnative_type):
        pass

class Annotation(object):
    """Helper class to parse the function annotation if present"""
    
    regex = r'((?P<index>\d)((?P<array>A)(?P<indexlist>\([\d,\s]+\))?)?)'    
    
    def __init__(self, string = None):
        self.inargs = {}
        self.outargs = {}
        self.uses_handle = True
        self.returns_error = True
        
        if string:
            self.parse_string(string)

    def parse_string(self, string):
        """
        Parses an annotion string for input and output arguments        
        #annotate in: 1,2 out: 3A(4) nohandle returns: error|value
        """

        #search output args
        self.parse_param_group('out', string, self.outargs)

        #search input args
        self.parse_param_group('in', string, self.inargs)

        #search if to use handle
        res = re.search(r'\bnohandle\b|\bhandle\b', string)
        if res:
            self.uses_handle = res.group() != 'nohandle'
            
        #search if function returns status error (or value)
        res = re.search(r'\bnoerror\b', string)
        if res:
            self.returns_error = res.group() != 'noerror'
        else:
            self.returns_error = True
            
        #check correctness
        for inarg in self.inargs:
            if inarg in self.outargs:
                raise Exception('Input argument can not be an output ' +
                                'argument at the same time')

    @staticmethod
    def parse_param_group(inout, text, params):
        """
        parses the index values of each in and output argument group,
        i.e. it tries to decode something like: 1, 4A(5)
        """
        
        # find the appropriate section in the annotation string
        res = re.search(inout + r':\s((' + Annotation.regex + 
                        r'|(,[\s]*))+)($|\s|#)', text)
        if not res:
            return
        
        ins = res.group()
        # parse each input parameter
        while ins:
            # find paramter
            tmp = re.search(Annotation.regex, ins)
            if not tmp:
                break
            
            arg_index =  int(tmp.group('index'))
            # parse array size arguments
            indexlist = ()
            if tmp.group('indexlist'):
                tmpstr = tmp.group('indexlist')
                indexlist = tuple([int(val) for val in tmpstr[1:-1].split(",")])
            
            params[arg_index]  = {'isarray':  tmp.group('array') is 'A', 
                                  'arraysizes': indexlist,
                                  'index': arg_index}
            ins = ins.replace(tmp.group(),'')

class CFunctionArg(object):
    '''
    Stores the type, name, number of pointers
    and constness of a function argument
    '''    
    
    def __init__(self, string, typedeflist = None, enumlist = None, handle_str = None):
        self.type = ''
        self.npointer = 0
        self.name = ''
        self.is_const   = False
        self.is_handle  = False
        self.is_string  = False
        self.is_array   = False
        self.is_outarg  = False
        self.is_sizearg = False
        
        if len(string) > 0:
            self.parse_arg(string, typedeflist, enumlist, handle_str)
        
    def parse_arg(self, string, typedeflist, enumlist, handle_str):
        ''' Parse the function argument.
            param: type definition from the signature
            Attributes:
              .name = name of the parameter in the signature
              .type = type of the parameter in the signature
              .is_const = True if there was a const
              .npointer = number of "*" stars

        '''
        regex = r'((?P<const>const)[\s]+)?(?P<type>[\w]+)[\s]*' + \
                r'(?P<pointer>[\*]*)[\s]*(?P<name>[\w]+)?[\s]*'
        res = re.search(regex, string)
         
        
        self.is_const  = res.group('const') == 'const'
        self.type      = res.group('type')
        self.npointer  = len(res.group('pointer'))
        self.name      = res.group('name')
        self.is_handle = self.type == handle_str
        self.is_string = self.type == 'char' and self.npointer > 0
        
        # reserved keywords and types that are not allowed as names
        keywords = ['const', 'while', 'for']
        
        # resolve type
        self.type = self.resolv_type(self.type, enums = enumlist, typedefs = typedeflist)

        
        if self.name in keywords:
            raise Exception('"%s" is an invalid argument name' % (self.name))
            
            
    def resolv_type(self, mytype, enums, typedefs):
        basictypes    = ['int', 'long', 'float', 'double', 'char', 'void']

        if mytype in basictypes:
            return mytype
        
        while True:
            if mytype.startswith('enum'):
                return 'int'
            
            if mytype in typedefs:
                newtype = typedefs[mytype]
                if newtype in basictypes:
                    return newtype
                if newtype == mytype:
                    raise Exception('Recursive definition of type %s' % mytype)
                else:
                    mytype = newtype
            else:
                break
        
        
        raise Exception('"%s" is not a valid type name' % (self.type))
             
class CFunctionDec(object):
    def __init__(self):
        self.arguments = []
        self.return_value = None
        self.returns_error = True
        self.method_name = ''
        self.uses_handle = True
        
    def parse_method_header(self, string, typedeflist = None, enumlist = None,  handle_str = None):
        '''
        Parses a typical function header like
            int getMyValue(double * p, int index)
            
        Scans for function name, arguments and return value
        '''        
        
        regex = r'(?P<retval>(const )?[\w*]+([\s]+[\*]+)?)[\s]+(?P<name>[\w]+)' + \
                r'[\s]*\((?P<args>[\w\s,*\[\]]*)\)'
        res = re.search(regex, string)
        if not res:
            raise Exception('Cannot parse function declaration "%s"' % string)
        
        arg_string = res.group('args')
        arg_str_list = arg_string.split(',')
        for index, arg_str in enumerate(arg_str_list):
            arg = CFunctionArg(arg_str, typedeflist, enumlist, handle_str)

            # apply some default behaviour
            
            # first argument is assumed to be handle
            if index == 0 and arg.npointer == 0:
                arg.is_handle = True
            

            # if there is a asterisk, we assume it to be an array, except
            # if its the last argument, then it is an outarg
            arg.is_outarg = False
            if arg.npointer > 0:
                if index < len(arg_str_list) - 1:
                    arg.is_array = True
                elif (arg.is_string and arg.npointer > 1) \
                 or (not arg.is_string and arg.npointer > 0):
                    arg.is_outarg = True
                    
            if not arg.name:
                arg.name = 'arg%d' % index
            
            self.arguments.append(arg)
            
            
        self.return_value = CFunctionArg(res.group('retval'), typedeflist, enumlist, handle_str)
        self.method_name  = res.group('name')
        
    def apply_annotation(self, annotation):
        '''
        Uses the information parsed from the annotation to identify
        the roles of the arguments (in, outs, arrays, arraysizes...)
        '''
        
        if annotation.returns_error:
            self.returns_error = True
        else:
            self.returns_error = False
            self.return_value.name = 'ret'
        
        # apply outpt args
        for index, outarg in annotation.outargs.iteritems():
            if index >= len(self.arguments):
                raise Exception('annotation index is too large for function %s'\
                    % self.method_name)
                
            self.arguments[index].is_outarg = True
            self.arguments[index].is_array  = outarg['isarray']
            
            
        # apply input args
        for index, inarg in annotation.inargs.iteritems():
            if index >= len(self.arguments):
                raise Exception('annotation index is too large for function %s'\
                    % self.method_name)
                
            self.arguments[index].is_outarg = False
            self.arguments[index].is_array  = inarg['isarray']
            if inarg['isarray']:
                for sizeindex in inarg['arraysizes']:
                    self.arguments[sizeindex].is_sizearg = True
            
            
        self.uses_handle = annotation.uses_handle
        if not self.uses_handle and len(self.arguments) > 0:
            self.arguments[0].is_handle = False

class PythonGenerator(object):
    '''
    Generates Python code wrappers from the parsed semantics
    '''    
    
    def __init__(self, name_prefix):
        self.prefix = name_prefix
        self.handle_str = 'doc_handle'
        self.native_types = ['int', 'double', 'float', 'char', 'bool']
        if len(name_prefix) > 0:
            self.handle_str = name_prefix + '_' + self.handle_str
    
    def create_header(self, fun_dec, indention_depth):
        '''Creates the method header as "def myFunc(self, myvalue):"'''
        
        string = ''
        indent = 4*indention_depth*' '
        
        # generate simplified method name, e.g.: tixiGetValue -> getValue
        name = fun_dec.method_name
        if name.startswith(self.prefix):
            name = name[len(self.prefix)].lower() + name[len(self.prefix)+1:]
        
        # position of the handle variable
        handle_index = -1
        
        num_inargs  = 0
        num_outargs = 0
        
        # create method header
        string += indent + 'def %s(self' % name
        for index, arg in enumerate(fun_dec.arguments):
            if arg.is_handle and fun_dec.uses_handle:
                # we dont use the handle as an function argument
                handle_index = index
            elif not arg.is_outarg:
                string += ', %s' % arg.name
                num_inargs += 1
            elif arg.is_outarg:
                num_outargs += 1
            
        string += '):\n'
        return (string, num_inargs, num_outargs, handle_index)
    
    def create_pre_call(self, fun_dec, num_inargs, num_outargs, indention_depth):
        '''
        Creates the code for the input argument conversion and prepares the
        output arguments.
        '''
        indent = 4*(indention_depth+1)*' '
        string = ''
        raw_name = fun_dec.method_name
        
        #create input arguments
        if num_inargs > 0:
            string += indent + '# input arg conversion\n'
            
        iargs = (arg for arg in fun_dec.arguments if not arg.is_outarg)
        for arg in iargs:
            tmp_str = ''
            if arg.is_handle:
                continue
            elif arg.is_string:
                tmp_str = '_c_%s = ctypes.c_char_p(%s)' \
                    % (arg.name, arg.name)
            elif not arg.is_array and arg.npointer == 0:
                tmp_str = '_c_%s = ctypes.c_%s(%s)' \
                    % (arg.name, arg.type, arg.name)
            elif arg.is_array and arg.npointer > 0:
                # create type
                tmp_str = 'array_t_%s = ctypes.c_%s * len(%s)\n' \
                    % (arg.name, arg.type, arg.name)
                tmp_str += indent
                tmp_str += '_c_%s = array_t_%s(*%s)' \
                    % (arg.name, arg.name, arg.name)
            else:
                raise Exception('Cannot create python to c conversion ' +
                 'for input argument "%s" in "%s"' % (arg.name, raw_name))
            
            if not arg.type in self.native_types:
                raise Exception('Cannot create python to c conversion ' +
                 'for type "%s"' % arg.type)
            
            string += indent + tmp_str + '\n'

        #create output arguments
        if num_outargs > 0:
            string += '\n'
            string += indent + '# output arg preparation\n'
        oargs = (arg for arg in fun_dec.arguments if arg.is_outarg)
        for arg in oargs:
            if arg.is_array and arg.npointer > 0:
                tmp_str = '_c_%s = ctypes.POINTER(ctypes.c_%s)()' \
                    % (arg.name, arg.type)
            elif arg.is_string:
                tmp_str = '_c_%s = ctypes.c_char_p()' % (arg.name)
            elif not arg.is_array and arg.npointer == 1:
                tmp_str = '_c_%s = ctypes.c_%s()' % (arg.name, arg.type)
            else:
                raise Exception('Cannot create python to c conversion ' +
                 'for output argument "%s" in %s' % (arg.name, raw_name) )
                
            string += indent + tmp_str + '\n'
                
        return string
        
    def create_call(self, fun_dec, ret_val, handle_index, indention_depth):
        '''
        Create function call code
        '''
        indent = 4*(indention_depth+1)*' '
        
        string = indent + '# call to native function\n'
        if ret_val:
            if ret_val.is_string:
                string +=  indent + 'self.lib.%s.restype = ctypes.c_char_p\n' \
                      % (fun_dec.method_name)
            else:
                string += indent + 'self.lib.%s.restype = ctypes.c_%s\n' \
                      % (fun_dec.method_name, ret_val.type)
            
        call = 'self.lib.%s(' % fun_dec.method_name
        for index, arg in enumerate(fun_dec.arguments):
            if index == handle_index:
                call += 'self.%s' % self.handle_str
            elif arg.is_outarg:
                call += 'ctypes.byref(_c_%s)' % arg.name
            else:
                call += '_c_%s' % arg.name
                
            if index < len(fun_dec.arguments) - 1:
                call += ', '
                
        call += ')'
        
        # we assume that each function is returning an error code
        # only if explictly specified otherwise
        if not ret_val:
            call = 'catch_error(%s)' % call
        else:
            call = '_c_%s = %s' % (ret_val.name, call)
                
        
        string += indent + call + '\n'
        return string
    
    def create_method_wrapper(self, fun_dec):
        '''
        Generates the python wrapper code around a c function call
        '''
        
        indent = 4*' '
        
        string, num_inargs, num_outargs, handle_index = \
            self.create_header(fun_dec, 1)
        
        ret_val = None if fun_dec.returns_error else fun_dec.return_value

        string += self.create_pre_call(fun_dec, num_inargs, num_outargs, 1)
        string += '\n'
                            
        string += self.create_call(fun_dec, ret_val, handle_index, 1)

        
        # accumulate return values
        outargs = []
        if ret_val:
            outargs.insert(0, ret_val)
        
        for arg in fun_dec.arguments:
            if arg.is_outarg:
                outargs.append(arg)
        
        # convert c ouputs to python output values
        if len(outargs) > 0:
            string += '\n'
        for arg in outargs:
            if arg.is_array:
                tmp_str = '_py_%s = (_c_%s[i] for i in xrange(%s_size))' \
                    % (arg.name, arg.name, arg.name)
            else:
                tmp_str = '_py_%s = _c_%s.value' \
                    % (arg.name, arg.name)
            
            string += 2*indent + tmp_str + '\n'
                

        # create the return statement
        if len(outargs) == 1:
            string += '\n' + 2*indent + 'return _py_%s\n' % outargs[0].name
        elif len(outargs) > 1:
            string += '\n' + 2*indent + 'return ('
            for i, arg in enumerate(outargs):
                string += '_py_' + arg.name
                if i < len(outargs)-1:
                    string += ', '
            string += ')\n'
        
        return string + '\n'
    

if __name__ == '__main__':
    AN = Annotation()
    AN.parse_string('')
    
    parser = CHeaderFileParser('TIXI', 'tixi')
    parser.read_header(r'D:\src\tixi\src\tixi2.h')
    #parser.add_alias('TixiDocumentHandle', 'int')
    parser.parse_defines()
    parser.parse_methods()