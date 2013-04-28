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
        self.method_name = ''
        
    def parse_method_header(self, string):
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
        for arg in arg_string.split(','):
            ARGUMENT = FunctionArg(arg)
            self.arguments.append(ARGUMENT)
            
        self.return_value = FunctionArg(res.group('retval'))
        self.method_name  = res.group('name')

class PythonGenerator(object):
    def __init__(self):
        pass
    
    def create_method_wrapper(self, annotation, argument_list):
        for index, arg in enumerate(argument_list):
            print index
    

AN = Annotation()
AN.parse_string('#annotate nohandle noerror outs: 1A(1,2) ins: 2A(3,4), 5 #')
for k, v in AN.inargs.iteritems():
    print v
    
print AN.returns_error
FA = FunctionArg('const TixiHandle value', 
                 handle_str = 'TixiHandle', type_list=['RetVal'])

MP = CFunctionDec()
MP.parse_method_header('blubb ddouble tixiGetFloat()')