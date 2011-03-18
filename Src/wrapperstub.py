#! /usr/bin/env python
#############################################################################
# Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
#
# Created: 2010-08-13 Arne Bachmann <Arne.Bachmann@dlr.de>
# Changed: $Id: dbms.h 4577 20xx-xx-xx 09:27:39Z litz_ma $ 
#
# Version: $Revision: 3978 $
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

import sys
from ctypes import *

%s# generated enums are placed before this text

class TixiException(Exception):
    ''' The exception encapsulates the error return code of the library and arguments that were provided for the function. '''
    def __init__(self, code, *args, **kwargs):
        Exception.__init__(self)
        self.code = code
        if "error" in kwargs:
            self.error = str(kwargs["error"])
        elif code in ReturnCode._names:
            self.error = ReturnCode._names[code]
        else:
            self.error = "UNDEFINED"
        self.args = tuple(args)
        self.kwargs = dict(kwargs)
    def __str__(self):
        return self.error + " (" + str(self.code) + ") " + str(list(self.args)) + " " + str(self.kwargs)

class Tixi(object):
    def __init__(self):
        ''' The constructor initializes the TIXI library '''
        self._handle = c_int(-1)
        
        if sys.platform == 'win32':
            self.TIXI = cdll.TIXI
        else:
            self.TIXI = CDLL("libTIXI.so")
        self.version = c_char_p()
        self.version.value = self.TIXI.tixiGetVersion()
        self.version = self.version.value
            
    def __del__(self):
        ''' The destructor cleans up the library '''
        if hasattr(self, "TIXI"):
            if self.TIXI != None:
                self.close()
                self.TIXI = None

    def _validateReturnValue(self, tixiReturn, *args, **kwargs):
        ''' Helper function to raise an exception if return value is not SUCCESS '''
        if tixiReturn != ReturnCode.SUCCESS:
            raise TixiException(tixiReturn, args, kwargs)

    def open(self, xmlInputFilename, recursive = False):
        ''' Open a document from a file. '''
        if recursive:
            tixiReturn = self.TIXI.tixiOpenDocumentRecursive(xmlInputFilename, byref(self._handle), OpenMode.OPENMODE_RECURSIVE)
        else:
            tixiReturn = self.TIXI.tixiOpenDocument(xmlInputFilename, byref(self._handle))
        if tixiReturn != ReturnCode.SUCCESS:
            self._handle.value = -1
        self._validateReturnValue(tixiReturn, xmlInputFilename, recursive)
    
    def openHttp(self, url):
        ''' Open a document from an URL. '''
        tixiReturn = self.TIXI.tixiOpenDocumentFromHTTP(url, byref(self._handle))
        if tixiReturn != ReturnCode.SUCCESS:
            self._handle.value = -1
        self._validateReturnValue(tixiReturn, url)

    def openString(self, string):
        ''' Open a document from a string. '''
        tixiReturn = self.TIXI.tixiImportFromString(string, byref(self._handle))
        if tixiReturn != ReturnCode.SUCCESS:
            self._handle.value = -1
        self._validateReturnValue(tixiReturn, string)

    def create(self, root):
        ''' Create a new tixi document in memory. '''
        tixiReturn = self.TIXI.tixiCreateDocument(root, byref(self._handle))
        if tixiReturn != ReturnCode.SUCCESS:
            self._handle.value = -1
        self._validateReturnValue(tixiReturn, root)

    def save(self, fileName, recursive = False, remove = False):
        ''' Save the main tixi document.
            If the document was opened recursively,
             * 'recursive' tells to save linked nodes to their respecitve files, too.
             * 'remove' tells to remove the links to external files after saving the complete CPACS inclusively all linked content to the main file.
            You cannot have 'remove' without 'recursive'.
        '''
        if recursive and remove:
            tixiReturn = self.TIXI.tixiSaveAndRemoveDocument(self._handle, fileName)
        elif recursive:
            tixiReturn = self.TIXI.tixiSaveCompleteDocument(self._handle, fileName)
        else:
            tixiReturn = self.TIXI.tixiSaveDocument(self._handle, fileName)
        self._validateReturnValue(tixiReturn, fileName, recursive, remove)
        
    def close(self):
        ''' Closes the current document.
            Test cases contained in other functions already.
        '''
        if self._handle.value != -1:
            tixiReturn = self.TIXI.tixiCloseDocument(self._handle)
            self._handle.value = -1
            self._validateReturnValue(tixiReturn)
    
    def getBooleanElement(self, elementPath):
        ''' Manually edited from generated code. '''
        _c_elementPath = c_char_p()
        _c_elementPath.value = elementPath
        _c_boolean = c_int(0)
        tixiReturn = self.TIXI.tixiGetBooleanElement(self._handle, _c_elementPath, byref(_c_boolean))
        self._validateReturnValue(tixiReturn, elementPath)
        if _c_boolean.value == 1:
            _c_boolean = True
        else:
            _c_boolean = False
        return _c_boolean
        
    def addBooleanElement(self, parentPath, elementName, boolean):
        ''' Manually edited from generated code. '''
        if boolean in [True, 1, "1"] or str(boolean).lower() in ["true", "yes", "on"]:
            boolean = 1
        else:
            boolean = 0
        _c_parentPath = c_char_p()
        _c_parentPath.value = parentPath
        _c_elementName = c_char_p()
        _c_elementName.value = elementName
        _c_boolean = c_int(boolean)
        tixiReturn = self.TIXI.tixiAddBooleanElement(self._handle, _c_parentPath, _c_elementName, _c_boolean)
        self._validateReturnValue(tixiReturn, parentPath, elementName, boolean)
   
    def checkElement(self, elementPath):
        ''' boolean return values from special return code is coded manually here '''
        _c_elementPath = c_char_p()
        _c_elementPath.value = elementPath
        tixiReturn = self.TIXI.tixiCheckElement(self._handle, _c_elementPath)
        if tixiReturn == ReturnCode.SUCCESS:
            return True
        if tixiReturn == ReturnCode.ELEMENT_NOT_FOUND:
            return False
        self._validateReturnValue(tixiReturn, elementPath)

    def checkAttribute(self, elementPath, attributeName):
        ''' boolean return values from special return code is coded manually here '''
        _c_elementPath = c_char_p()
        _c_elementPath.value = elementPath
        _c_attributeName = c_char_p()
        _c_attributeName.value = attributeName
        tixiReturn = self.TIXI.tixiCheckAttribute(self._handle, _c_elementPath, _c_attributeName)
        if tixiReturn == ReturnCode.SUCCESS:
            return True
        if tixiReturn == ReturnCode.ATTRIBUTE_NOT_FOUND:
            return False
        self._validateReturnValue(tixiReturn, elementPath, attributeName)

%s# the method definitions are placed above