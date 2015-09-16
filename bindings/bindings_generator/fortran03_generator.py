# -*- coding: utf-8 -*-
"""
Created on Wed Sep 16 13:57:10 2015

@author: Martin Siggel <martin.siggel@dlr.de>
@author: Melven Roehrig-Zoellner <Melven.Roehrig-Zoellner@DLR.de>
"""

class GeneratorException(Exception):
    
    def __init__(self, errormsg, arg):
        msg = errormsg + '\n'
        msg += 'Argument was:\n'
        msg += '  type      : ' + arg.type + '\n'
        msg += '  name      : ' + arg.name + '\n'
        msg += '  is_outarg : ' + str(arg.is_outarg) + '\n'
        msg += '  is_const  : ' + str(arg.is_const) + '\n'
        msg += '  npointer  : ' + str(arg.npointer) + '\n'
        msg += '  is_string : ' + str(arg.is_string) + '\n'
        msg += '  arrayinfos: ' + str(arg.arrayinfos)+ '\n'
        msg += '  is_handle : ' + str(arg.is_handle) + '\n'
        self.value = msg
        
    def __str__(self):
        return repr(self.value)


indent = 2*' '

class Fortran03Generator(object):
    '''
    Generates Fortran 2003 code wrappers from the parsed semantics
    '''

    def __init__(self):
        self.license = None
        self.userfunctions = None
        self.postconstr = None
        self.closefunction = None
        self.blacklist = []
        self.aliases = {}
        self.integer_types = {'int':'C_INT','long':'C_LONG'}
        self.real_types = {'float':'C_FLOAT','double':'C_DOUBLE'}
            

    def add_alias(self, oldname, newname):
        self.aliases[oldname] = newname
            

    def create_wrapper(self, cparser):
        string = ''
        if self.license:
            for line in self.license.splitlines():
                string += '! ' + line + '\n'
        
        string += '\n\n'
        for enumname, values in cparser.enums.iteritems():
            string += self.create_enum(enumname, values) + '\n\n'

        if self.userfunctions:
            for line in self.userfunctions.splitlines():
                string += indent + line + '\n'
        
        string += '\n\n'
        # start interface block
        string += 'interface\n'

        for dec in cparser.declarations:
            if dec.method_name not in self.blacklist:
                string += self.create_method_wrapper(dec) + '\n\n'
            
        # end interface block
        string += 'end interface\n'

        string += '\n\n'

        return string
        

    def create_enum(self, enumname, values):
        '''
        Generates the Fortran 2003 equivalent of a C enum
        '''
        string = '! enum ' + enumname + '\n'
        string += 'enum, bind(C)\n'

        val_key = list()
        for index, val in values.iteritems():
            val_key.append('%s = %d' % (val, index))
        string += indent + 'enumerator :: ' + ', '.join(val_key) + '\n'
        string += 'end enum'
        
        return string
    
    
    def create_method_wrapper(self, fun_dec):
        '''
        Generates the Fortran 2003 wrapper code around a c function call
        '''
        
        string = ''
        # raw C code
        for line in fun_dec.raw_string.splitlines():
            string += '! ' + line + '\n'
        # raw annotation
        if fun_dec.raw_annotation:
            for line in fun_dec.raw_annotation.splitlines():
                string += '! ' + line + '\n'

        # begin subroutine/function block
        if fun_dec.return_value:
            string += indent + 'function ' + fun_dec.method_name
        else:
            string += indent + 'subroutine ' + fun_dec.method_name
        # argument names
        string += '(' + ', '.join((arg.name for arg in fun_dec.arguments)) + ')'
        # return value name
        if fun_dec.return_value:
            string += ' result(%s)' % fun_dec.return_value.name
        # bind C
        string += ' bind(C,name=\'%s\')\n' % fun_dec.method_name

        # use iso_c_binding
        string += 2*indent + 'use, intrinsic :: iso_c_binding\n'

        # return value declaration
        if fun_dec.return_value:
            string += 2*indent + self.create_argument_decl(fun_dec.return_value, function_result=True) + '\n'

        # argument declarations
        for arg in fun_dec.arguments:
            string += 2*indent + self.create_argument_decl(arg, function_result=False) + '\n'

        # end subroutine/function block
        if fun_dec.return_value:
            string += indent + 'end function\n'
        else:
            string += indent + 'end subroutine\n'

        return string


    def create_argument_decl(self, arg_dec, function_result):
        '''
        Generates the Fortran 2003 declaration for an argument of a function/subroutine call
        '''

        string = ''
        # basic argument type
        if arg_dec.type in self.integer_types:
            string += 'integer(kind=%s)' % self.integer_types[arg_dec.type]
        if arg_dec.type in self.real_types:
            string += 'real(kind=%s)' % self.real_types[arg_dec.type]
        elif arg_dec.is_string:
            string += 'character(kind=C_CHAR)'

        # additional qualifiers
        if not function_result:
            if arg_dec.npointer == 0:
                string += ', value'
            elif arg_dec.is_outarg:
                string += ', intent(out)'
            else:
                string += ', intent(in)'

        # argument name
        string += ' :: ' + arg_dec.name

        # array dimensions
        if arg_dec.is_string:
            string += '(*)'
        elif arg_dec.arrayinfos['is_array']:
            string += '(*)'

        return string
