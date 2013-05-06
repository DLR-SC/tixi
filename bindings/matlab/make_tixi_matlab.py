# -*-# coding: utf-8 -*-
"""
Created on Wed May 01 11:44:49 2013

@author: Martin Siggel <martin.siggel@dlr.de>
"""

import sys, os 

tixipath = os.path.dirname(os.path.realpath(__file__)) + '/../..'
sys.path.append(tixipath + '/bindings')
import bindings_generator.matlab_generator as MG
import bindings_generator.cheader_parser   as CP


blacklist = []

if __name__ == '__main__':
    # parse the file
    
    #ann =   CP.Annotation('#annotate out: 3, 4A(3)')    
    
    parser = CP.CHeaderFileParser()
    
    # set the handle string that the parser can identify the handles
    parser.handle_str = 'TixiDocumentHandle'
    parser.returncode_str  ='ReturnCode'
    parser.parse_header_file(tixipath + '/src/tixi.h')
    
    # create the wrapper
    generator = MG.MatlabGenerator(parser, 'tixi', 'tixi.h')
    generator.create_wrapper()
    
    
