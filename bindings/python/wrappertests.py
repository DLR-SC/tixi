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

import os
import unittest
from tixiwrapper import Tixi, TixiException, StorageMode

class Tests(unittest.TestCase):
    ''' Test almost all TIXI functions to ensure correct generated code '''
    
    def setUp(self):
        pass
        
    def tearDown(self):
        pass
    
    def test_open(self):
        t = Tixi()
        self.assertRaises(TixiException, t.open, "____HOPEFULLY_THIS_FILE_NAME_DOES_NOT_EXIST~~~~") #OPEN_FAILED
        self.assertRaises(TixiException, t.open, os.path.join("TestData", "illformed.xml")) #NOT_WELL_FORMED
        t.open(os.path.join("TestData", "in.xml"))
        self.assertTrue(t._handle.value != -1)
        t.close()

    def test_open_http(self):
        t = Tixi()
        t.openHttp("http://www.w3schools.com/XML/note.xml")
        t.close()

    def test_open_string(self):
        t = Tixi()
        cpacs = '<?xml version="1.0"?><cpacs><header><version>1.2.3</version></header></cpacs>'
        t.openString(cpacs)
        t.close()

    def test_create(self):
        t = Tixi()
        t.create("cpacs")
        self.assertTrue(t._handle != -1)
        t.close()

    
    def test_multiple_tixis(self):
        ''' Check if we can handle several loaded DLLs and CPACS at the same time. '''
        a = Tixi()
        b = Tixi()
        self.assertNotEqual(a._handle, b._handle)
        a.open(os.path.join("TestData", "in.xml"))
        b.open(os.path.join("TestData", "in.xml"))
        self.assertNotEqual(a._handle, b._handle)
        a.close()
        b.close()
        
    def test_validation_and_export(self):
        t = Tixi()
        t.open(os.path.join("TestData", "valid_CPACS_dokumentiert.xml"))
        c = t.exportDocumentAsString()
        self.assertTrue(c != None and c != "")
        header = '<?xml version="1.0" encoding="UTF-8"?>'
        self.assertTrue(c.startswith(header))
        t.schemaValidateFromFile(os.path.join("TestData", "valid_cpacs_schema.xsd"))
        schema = 'invalid'
        self.assertRaises(TixiException, t.schemaValidateFromString, schema) #OPEN_SCHEMA_FAILED
        schema = '<?xml version="1.0" encoding="UTF-8"?>\\n<?xml-stylesheet href="xs3p.xsl" type="text/xsl"?>\\n<xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns="http://www.w3.org/1999/xhtml" targetNamespace="http://www.w3.org/1999/xhtml" elementFormDefault="qualified" attributeFormDefault="unqualified">\\n</xsd:schema>' # TODO
        self.assertRaises(TixiException, t.schemaValidateFromString, schema) #NOT_SCHEMA_COMPLIANT
        t.close()
    
    def test_elements(self):
        t = Tixi()
        t.open(os.path.join("TestData", "in.xml"))
        self.assertEqual(t.getTextElement("/plane/name"), "Junkers JU 52")
        t.updateTextElement("/plane/name", "D150")
        self.assertEqual(t.getTextElement("/plane/name"), "D150")
        t.addTextElement("/plane", "name", "B747")
        self.assertEqual(t.getTextElement("/plane/name[2]"), "B747")
        self.assertEqual(t.getIntegerElement("/plane/numberOfPassengers"), 57)
        t.addIntegerElement("/plane", "nop", 123456, None)
        self.assertEqual(t.getIntegerElement("/plane/nop"), 123456)
        self.assertEqual(t.getDoubleElement("/plane/wings[1]/wing[1]/centerOfGravity/x"), 30.0)
        t.addDoubleElement("/plane/wings/wing[1]", "cog", 123.456, None)
        self.assertEqual(t.getDoubleElement("/plane/wings/wing[1]/cog"), 123.456)
        self.assertFalse(t.getBooleanElement("/plane/bool/aBool2"))
        self.assertTrue(t.getBooleanElement("/plane/bool/aBool1b"))
        self.assertFalse(t.checkElement("/plane/xx")) #ELEMENT_NOT_FOUND
        t.createElement("/plane", "xx")
        t.checkElement("/plane/xx")
        self.assertEquals(t.getTextElement("/plane/xx"), "")
        t.updateTextElement("/plane/xx", "value")
        self.assertEquals(t.getTextElement("/plane/xx"), "value")
        t.removeElement("/plane/xx")
        self.assertEqual(t.checkElement("/plane/xx"),False)
        self.assertRaises(TixiException, t.getTextElement, "/plane/xx") #ELEMENT_NOT_FOUND
        
    def test_attributes(self):
        t = Tixi()
        t.open(os.path.join("TestData", "in.xml"))
        self.assertEquals(t.getTextAttribute("/plane/wings/wing[1]", "position"), "left")
        self.assertEquals(t.getIntegerAttribute("/plane/wings", "numberOfWings"), 2)
        t.addIntegerAttribute("/plane", "intattr", 324, None)
        self.assertEquals(t.getIntegerAttribute("/plane", "intattr"), 324)
        t.removeAttribute("/plane", "intattr")
        self.assertEquals(t.getDoubleAttribute("/plane/coordinateOrigin", "scaling"), 1.3456)
        self.assertFalse(t.checkAttribute("/plane", "doubleattr")) # ATTRIBUTE_NOT_FOUND \n
        t.addDoubleAttribute("/plane", "doubleattr", 123.456, None)
        t.checkAttribute("/plane", "doubleattr")
        self.assertEquals(t.getDoubleAttribute("/plane", "doubleattr"), 123.456)
        t.removeAttribute("/plane", "doubleattr")
        t.addTextAttribute("/plane", "attr", "val")
        self.assertEquals(t.getTextAttribute("/plane", "attr"), "val")
        self.assertEquals(t.getNamedChildrenCount("/plane/wings", "wing"), 2)
        t.addDoubleListWithAttributes("/plane", "list", "elem", "attr", [1., 2., 3.], None, ["a", "b", "c"], 3)
        self.assertEquals(t.getTextAttribute("/plane/list/elem[1]", "attr"), "a")
        self.assertEquals(t.getDoubleElement("/plane/list/elem[3]"), 3.0)
        t.close()
        
    def test_vector_array(self):
        t = Tixi()
        t.open(os.path.join("TestData", "vectorcount.xml"))
        self.assertEquals(t.getVectorSize("/a/aeroPerformanceMap/cfx"), 32)
        #v = t.getFloatVector("/a/aeroPerformanceMap/cfx", 32)
        #print v[0], v[1], v[-1]
        #1 2 118
        t.close()

        # manual creation of vector
        t.create('doc')
        t.addFloatVector('/doc', 'myvec', range(100,130), 30, "%g")
        
        size = t.getVectorSize('/doc/myvec')  
        self.assertEquals(size, 30)
        v = t.getFloatVector('/doc/myvec', 30)
        self.assertEquals(v, tuple(range(100,130)))
        
        t.close()
        
        t.open(os.path.join("TestData", "arraytests.xml"))
        self.assertEquals(t.getArrayDimensions("/root/aeroPerformanceMap"), 4)
        self.assertEquals(t.getArrayDimensionSizes("/root/aeroPerformanceMap", 4), ((1, 2, 3, 8), 48))
        self.assertEquals(t.getArrayDimensionNames("/root/aeroPerformanceMap", 4), ("machNumber", "reynoldsNumber", "angleOfYaw", "angleOfAttack"))
        # print t.getArrayDimensionValues("/root/aeroPerformanceMap", 0) 1.0
        # print t.getArrayDimensionValues("/root/aeroPerformanceMap", 2)            0 5 10
        self.assertEquals(t.getArrayParameters("/root/aeroPerformanceMap"), 7)
        # print t.getArrayParameterNames("/root/aeroPerformanceMap")            ("cfx", "cfy", "cfz", "cmx", "cmy", "cmz", "def")
        # print t.getArray("/root/aeroPerformanceMap", "cmx")            None
        # print t.getArrayValue(arr, "/root/aeroPerformanceMap", "angleOfAttack")
        #8
        #print t.getArrayElementNames("/root/aeroPerformanceMap", "vector")
        #print t.getArrayElementNames("/root/aeroPerformanceMap", "array")
        t.getArrayElementCount("/root/aeroPerformanceMap","array")
        t.createElement("/root", "p1")
        t.addPoint("/root/p1", 1, 2, 3, None)
        self.assertEquals(t.getPoint("/root/p1"), (1.0, 2.0, 3.0))
        self.assertEquals(t.xPathEvaluateNodeNumber("/root/aeroPerformanceMap"), 1)
        t.close()
        t.open(os.path.join("TestData", "in.xml"))
        self.assertEquals(t.xPathExpressionGetTextByIndex("/plane/wings/wing/centerOfGravity/x", 2), "30.0")
        t.close()
        t.open(os.path.join("TestData", "uid_correct.xml"))
        t.uIDCheckDuplicates()
        t.uIDCheckExists("schlumpf")
        t.close()
        t.open(os.path.join("TestData", "uid_duplicated.xml"))
        self.assertRaises(TixiException, t.uIDCheckDuplicates) #UID_NOT_UNIQUE

    def test_api(self):
        t = Tixi()
        self.assertEquals(t.version, t.getVersion())
        t.create("root")
        t.addDoubleElement("/root","myDouble",3.2,0)
        t.addIntegerElement("/root","myInteger",6,None)

        # Here starts the rest
        t.addBooleanElement("/root","myBoolean",0)
        t.updateDoubleElement("/root/myDouble",3.14159262,0)
        t.updateIntegerElement("/root/myInteger",7,None)
        t.updateBooleanElement("/root/myBoolean",1)
        t.addTextElementAtIndex("/root","myNewTextElement","myText",2)
        t.createElementAtIndex("/root","array",0)
        t.addFloatVector("/root","myFloatVector",(0.0,1.1,2.2),3, "%g")
        t.addExternalLink("/root","/externalLink",".xml")
        t.addHeader("tool","version","author")
        t.addCpacsHeader("name","creator","version","description","cpacsVersion")
        t.usePrettyPrint(1)
        t.uIDCheckLinks()
        t.uIDSetToXPath("/root/myBoolean","booleanID")
        self.assertEqual(t.uIDGetXPath("booleanID"),"/root/myBoolean")
        #t.dTDValidate()
        #t.xSLTransformationToFile()
        t.save(os.path.join("TestData", "test_save.xml"))
        t.close()
        t.cleanup()
        
if __name__ == '__main__':
    unittest.main()
