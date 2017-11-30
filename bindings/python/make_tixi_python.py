# -*-# coding: utf-8 -*-
"""
Created on Wed May 01 11:44:49 2013

@author: Martin Siggel <martin.siggel@dlr.de>
"""

from __future__ import print_function
import sys, os
from datetime import date

tixipath = os.path.dirname(os.path.realpath(__file__)) + '/../..'
sys.path.append(tixipath + '/bindings')
import bindings_generator.python_generator as PG
import bindings_generator.cheader_parser   as CP

apache = \
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
apache += \
'# This file is automatically created from tixi.h on %s.\n'\
    % date.today() + \
'# If you experience any bugs please contact the authors\n\n'

userfunctions = \
'''def open(self, xmlInputFilename, recursive = False):
    if recursive:
        self.tixiOpenDocumentRecursive(xmlInputFilename, OpenMode.OPENMODE_RECURSIVE)
    else:
        self.tixiOpenDocument(xmlInputFilename)

def close(self):
    if self._handle.value >= 0:
        ret = self.lib.tixiCloseDocument(self._handle)
        self._handle.value = -1
        catch_error(ret)

def save(self, fileName, recursive = False, remove = False):
    \'\'\' Save the main tixi document.
        If the document was opened recursively,
         * 'recursive' tells to save linked nodes to their respecitve files, too.
         * 'remove' tells to remove the links to external files after saving the complete CPACS inclusively all linked content to the main file.
        You cannot have 'remove' without 'recursive'.
    \'\'\'
    if recursive and remove:
        self.tixiSaveAndRemoveDocument(fileName)
    elif recursive:
        self.tixiSaveCompleteDocument(fileName)
    else:
        self.tixiSaveDocument(fileName)

def checkElement(self, elementPath):
    \'\'\' boolean return values from special return code is coded manually here \'\'\'
    _c_elementPath = ctypes.c_char_p()
    _c_elementPath.value = str.encode(elementPath)
    tixiReturn = self.lib.tixiCheckElement(self._handle, _c_elementPath)
    if tixiReturn == ReturnCode.SUCCESS:
        return True
    if tixiReturn == ReturnCode.ELEMENT_NOT_FOUND:
        return False
    catch_error(tixiReturn, elementPath)

def uIDCheckExists(self, uID):
    _c_uID = ctypes.c_char_p()
    _c_uID.value = str.encode(uID)
    tixiReturn = self.lib.tixiUIDCheckExists(self._handle, _c_uID)
    if tixiReturn == ReturnCode.SUCCESS:
        return True
    else:
        return False
    catch_error(tixiReturn, uID)

def checkAttribute(self, elementPath, attributeName):
    \'\'\' boolean return values from special return code is coded manually here \'\'\'
    _c_elementPath = ctypes.c_char_p()
    _c_elementPath.value = str.encode(elementPath)
    _c_attributeName = ctypes.c_char_p()
    _c_attributeName.value = str.encode(attributeName)
    tixiReturn = self.lib.tixiCheckAttribute(self._handle, _c_elementPath, _c_attributeName)
    if tixiReturn == ReturnCode.SUCCESS:
        return True
    if tixiReturn == ReturnCode.ATTRIBUTE_NOT_FOUND:
        return False
    catch_error(tixiReturn, elementPath, attributeName)


'''

postconstr = '''
self.version = self.tixiGetVersion()
'''

blacklist = ['tixiCheckElement', 'tixiUIDCheckExists', 'tixiCheckAttribute', 'tixiCloseDocument', 'tixiGetRawInterface', 'tixiSetPrintMsgFunc']

if __name__ == '__main__':
    # parse the file

    #ann =   CP.Annotation('#annotate out: 3, 4A(3)')

    parser = CP.CHeaderFileParser()

    # set the handle string that the parser can identify the handles
    parser.handle_str = 'TixiDocumentHandle'
    parser.returncode_str  ='ReturnCode'
    parser.typedefs = {'TixiPrintMsgFnc': 'void'}
    parser.parse_header_file(tixipath + '/src/tixi.h')

    # create the wrapper
    pg = PG.PythonGenerator(name_prefix = 'tixi3', libname = 'tixi3')
    pg.license = apache
    pg.userfunctions = userfunctions
    pg.blacklist = blacklist
    pg.postconstr = postconstr
    pg.closefunction = 'close'
    pg.add_alias('tixiOpenDocumentFromHTTP', 'openHttp')
    pg.add_alias('tixiCreateDocument', 'create')
    pg.add_alias('tixiImportFromString', 'openString')

    print('Creating python interface... ', end=' ')
    wrapper = pg.create_wrapper(parser)
    print('done')

    # write file
    filename = 'tixi3wrapper.py'
    print('Write tixi python interface to file "%s" ... ' % filename, end=' ')
    fop = open(filename, 'w')
    fop.write(wrapper)
    print('done')


