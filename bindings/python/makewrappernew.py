# -*- coding: utf-8 -*-
"""
Created on Sat Apr 27 22:38:05 2013

@author: Martin Siggel
"""

import re

class Annotation(object):
    """Helper class to parse the function annotation if present"""
    
    regex = r'((?P<index>\d)((?P<array>A)(?P<indexlist>\([\d,\s]+\))?)?)'    
    
    def __init__(self):
        self.inargs = {}
        self.outargs = {}
        self.uses_handle = True
        self.returns_error = True

    def parse_string(self, string):
        """
        Parses an annotion string for input and output arguments        
        #annotate ins: 1,2 outs: 3A(4) nohandle returns: error|value
        """

        #search output args
        self.parse_param_group('outs', string, self.outargs)

        #search input args
        self.parse_param_group('ins', string, self.inargs)

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
                        r'|(,[\s]*))+)($|\s)', text)
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

class FunctionArg(object):
    '''
    Stores the type, name, number of pointers
    and constness of a function argument
    '''    
    
    def __init__(self, string, type_list = None, handle_str = None):
        self.type = ''
        self.npointer = 0
        self.is_const = False
        self.name = ''
        self.is_handle = False
        self.is_array = False
        self.is_outarg = False
        self.is_sizearg = False
        
        if len(string) > 0:
            self.parse_arg(string, type_list, handle_str)
        
    def parse_arg(self, string, type_list = None, handle_str = None):
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
        
        # reserved keywords and types that are not allowed as names
        keywords = ['const', 'while', 'for']
        types    = ['int', 'long', 'float', 'double', 'char', handle_str]
        if type_list: 
            types.extend(type_list)   
        
        if self.name in keywords:
            raise Exception('"%s" is an invalid argument name' % (self.name))
            
        if self.type not in types:
            raise Exception('"%s" is not a valid type name' % (self.type))

class CFunctionDec(object):
    def __init__(self):
        self.arguments = []
        self.return_value = None
        self.returns_error = True
        self.method_name = ''
        self.uses_handle = True
        
    def parse_method_header(self, string, type_list = None, handle_str = None):
        '''
        Parses a typical function header like
            int getMyValue(double * p, int index)
            
        Scans for function name, arguments and return value
        '''        
        
        regex = r'(?P<retval>(const )?[\w*]+)[\s]+(?P<name>[\w]+)' + \
                r'\((?P<args>[\w\s,*\[\]]*)\)'
        res = re.search(regex, string)
        if not res:
            raise Exception('Cannot parse function declaration "%s"' % string)
        
        arg_string = res.group('args')
        arg_str_list = arg_string.split(',')
        for index, arg in enumerate(arg_str_list):
            ARGUMENT = FunctionArg(arg, type_list, handle_str)

            # apply some default behaviour
            
            # first argument is assumed to be handle
            if index == 0 and ARGUMENT.npointer == 0:
                ARGUMENT.is_handle = True
            

            # if there is a asterisk, we assume it to be an array, except
            # if its the last argument, then it is an outarg
            ARGUMENT.is_outarg = False
            if ARGUMENT.npointer > 0:
                if index < len(arg_str_list) - 1:
                    ARGUMENT.is_array = True
                else:
                    ARGUMENT.is_outarg = True
            
            self.arguments.append(ARGUMENT)
            
            
        self.return_value = FunctionArg(res.group('retval'), type_list, handle_str)
        self.method_name  = res.group('name')
        
    def apply_annotation(self, annotation):
        if annotation.returns_error:
            self.returns_error = True
        else:
            self.returns_error = False
            self.return_value.name = 'ret'
        
        # apply outpt args
        for index, outarg in annotation.outargs.iteritems():
            if index >= len(self.arguments):
                raise Exception('annotation index is too large for function %s' % self.method_name)
                
            self.arguments[index].is_outarg = True
            self.arguments[index].is_array  = outarg['isarray']
            
            
        # apply input args
        for index, inarg in annotation.inargs.iteritems():
            if index >= len(self.arguments):
                raise Exception('annotation index is too large for function %s' % self.method_name)
                
            self.arguments[index].is_outarg = False
            self.arguments[index].is_array  = inarg['isarray']
            if inarg['isarray']:
                for sizeindex in inarg['arraysizes']:
                    print 'hall'
                    self.arguments[sizeindex].is_sizearg = True
            
            
        self.uses_handle = annotation.uses_handle

class PythonGenerator(object):
    '''
    Generates Python code wrappers from the parsed semantics
    '''    
    
    def __init__(self, name_prefix):
        self.prefix = name_prefix
        self.handle_str = 'doc_handle'
        if len(name_prefix) > 0:
            self.handle_str = name_prefix + '_' + self.handle_str
    
    def create_method_wrapper(self, fun_dec):
        '''
        Generates the python wrapper code around a c function call
        '''
        
        native_types = ['int','double','float','char','bool']
        
        indent = 4*' '
        
        raw_name = fun_dec.method_name
        
        # generate simplified method name, e.g.: tixiGetValue -> getValue
        name = fun_dec.method_name
        if name.startswith(self.prefix):
            name = name[len(self.prefix)].lower() + name[len(self.prefix)+1:]
        
        # position of the handle variable
        handle_index = -1
        
        ret_val = None if fun_dec.returns_error else fun_dec.return_value
        num_inargs = 0
        num_outargs= 0
        
        string = ''
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

        #create input arguments
        if num_inargs > 0:
            string += 2*indent + '# input arg conversion\n'
        for index, arg in enumerate(fun_dec.arguments):
            tmp_str = ''
            if arg.is_outarg:
                continue
            else:
                if arg.is_handle:
                    continue
                elif not arg.is_array and arg.npointer == 0:
                    tmp_str = '_c_%s = ctypes.c_%s(%s)' % (arg.name, arg.type, arg.name)
                elif arg.is_array and arg.npointer > 0:
                    # create type
                    tmp_str = 'array_t_%s = ctypes.c_%s * len(%s)\n' % (arg.name, arg.type, arg.name)
                    tmp_str += 2*indent
                    tmp_str += '_c_%s = array_t_%s(*%s)' % (arg.name, arg.name, arg.name)
                else:
                    raise Exception('Cannot create python to c conversion ' + 
                     'for input argument "%s" in "%s"' % (arg.name, raw_name))
                
                if not arg.type in native_types:
                    raise Exception('Cannot create python to c conversion for type "%s"' % arg.type)
                
                string += 2*indent + tmp_str + '\n'

        #create output arguments
        if num_outargs > 0:
            string += '\n'
            string += 2*indent + '# output arg preparation\n'
        for index, arg in enumerate(fun_dec.arguments):
            tmp_str = ''
            if not arg.is_outarg:
                continue
            else:
                if arg.is_array and arg.npointer > 0:
                    tmp_str = '_c_%s = ctypes.POINTER(ctypes.c_%s)()' % (arg.name, arg.type)
                elif not arg.is_array and arg.npointer == 1:
                    tmp_str = '_c_%s = ctypes.c_%s()' % (arg.name, arg.type)
                else:
                    raise Exception('Cannot create python to c conversion ' +
                     'for output argument "%s" in %s' % (arg.name, raw_name) )
                    
                string += 2*indent + tmp_str + '\n'
                            
        # create function call
        string += '\n'
        string += 2*indent + '# call to native function\n'
        if ret_val:
            string += 2*indent + 'self.lib.%s.restype = ctypes.c_%s\n' \
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
                       
        #we assume that each function is returning an error code
        # only if explictly specified otherwise
        if not ret_val:
            call = 'catch_error(%s)' % call
        else:
            call = '_c_%s = %s' % (ret_val.name, call)
                
        
        string += 2*indent + call + '\n'
        
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
                tmp_str = '_py_%s = [_c_%s[i] for i in xrange(%s_size)]' % (arg.name, arg.name, arg.name)
            else:
                tmp_str = '_py_%s = _c_%s.value' % (arg.name, arg.name)
            
            string += 2*indent + tmp_str + '\n'
                

        # create the return statement
        if len(outargs) == 1:
            string += '\n' + 2*indent + 'return _py_%s' % outargs[0].name
        elif len(outargs) > 1:
            string += '\n' + 2*indent + 'return ('
            for i, arg in enumerate(outargs):
                string += '_py_' + arg.name
                if i < len(outargs)-1:
                    string += ', '
            string += ')\n'
        
        print string
    

if __name__ == '__main__':
    AN = Annotation()
    AN.parse_string('#annotate noerror outs: 2,3  #')
    for k, v in AN.outargs.iteritems():
        print  k, v
        
    print AN.returns_error
    FA = FunctionArg('const TixiHandle value',
                     handle_str = 'TixiHandle', type_list=['RetVal'])
    
    MP = CFunctionDec()
    MP.parse_method_header('double tixiGetFloatVector(TixiHandle handle, int index,  double * x, double * y)',
                            type_list=['RetVal', 'TixiHandle'])
    MP.apply_annotation(AN)
    
    PG = PythonGenerator('tixi')
    PG.create_method_wrapper(MP)