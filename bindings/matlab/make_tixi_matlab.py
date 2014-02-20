# -*-# coding: utf-8 -*-
"""
Created on Wed May 01 11:44:49 2013

@author: Martin Siggel <martin.siggel@dlr.de>
"""

import sys, os, shutil, glob

filepath = os.path.dirname(os.path.realpath(__file__))
tixipath = filepath + '/../..'

sys.path.append(tixipath + '/bindings')
import bindings_generator.matlab_generator as MG
import bindings_generator.cheader_parser   as CP


blacklist = ['tixiGetRawInterface', 'tixiAddDoubleListWithAttributes']

if __name__ == '__main__':
    # parse the file
    
    #ann =   CP.Annotation('#annotate out: 3, 4A(3)')  

    # copy handwritten *.m files into current directory
    for mfile in glob.glob(filepath + r'/*.m'):
        shutil.copy(mfile, '.')
    
    parser = CP.CHeaderFileParser()
    
    # set the handle string that the parser can identify the handles
    parser.handle_str = 'TixiDocumentHandle'
    parser.returncode_str  ='ReturnCode'
    parser.parse_header_file(tixipath + '/src/tixi.h')
    
    # create the wrapper
    generator = MG.MatlabGenerator(parser, 'tixi', 'tixi.h')
    generator.blacklist = blacklist
    generator.create_wrapper()
    
    
