# -*-# coding: utf-8 -*-
"""
Created on Wed May 01 11:44:49 2013

@author: Martin Siggel <martin.siggel@dlr.de>
"""

import sys, os 
from datetime import date

tixipath = os.path.dirname(os.path.realpath(__file__)) + '/../..'
sys.path.append(tixipath + '/bindings')
import bindings_generator.python_generator as PG
import bindings_generator.cheader_parser   as CP

tixilicense = \
'''#############################################################################
# Copyright (C) 2007-2013 German Aerospace Center (DLR/SC)
#
# Created: 2013-05-01 Martin Siggel <Martin.Siggel@dlr.de>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#############################################################################

'''
tixilicense += \
'# This file is automatically created from tixi.h on %s.\n'\
    % date.today() + \
'# If you experience any bugs please contact the authors\n\n'

print date.today()

userfunctions = \
'''def open(self, xmlInputFilename, recursive = False):
    if recursive:
        self.openDocumentRecursive(xmlInputFilename, OpenMode.OPENMODE_RECURSIVE)
    else:
        self.openDocument(xmlInputFilename)

def save(self, fileName, recursive = False, remove = False):
    \'\'\' Save the main tixi document.
        If the document was opened recursively,
         * 'recursive' tells to save linked nodes to their respecitve files, too.
         * 'remove' tells to remove the links to external files after saving the complete CPACS inclusively all linked content to the main file.
        You cannot have 'remove' without 'recursive'.
    \'\'\'
    if recursive and remove:
        self.saveAndRemoveDocument(fileName)
    elif recursive:
        self.saveCompleteDocument(fileName)
    else:
        self.saveDocument(fileName)


'''


if __name__ == '__main__':
    # parse the file
    
    #ann =   CP.Annotation('#annotate out: 3, 4A(3)')    
    
    parser = CP.CHeaderFileParser()
    
    # set the handle string that the parser can identify the handles
    parser.handle_str = 'TixiDocumentHandle'
    parser.returncode_str  ='ReturnCode'
    parser.parse_header_file(tixipath + '/src/tixi2.h')
    
    # create the wrapper
    pg = PG.PythonGenerator('tixi','TIXI')
    pg.license = tixilicense
    pg.userfunctions = userfunctions
    pg.closefunction = 'close'
    pg.add_alias('tixiCloseDocument', 'close')
    pg.add_alias('tixiOpenDocumentFromHTTP', 'openHttp')
    pg.add_alias('tixiCreateDocument', 'create')
    wrapper = pg.create_wrapper(parser)
    
    # write file
    fop = open('tixiwrapper.py','w')
    fop.write(wrapper)