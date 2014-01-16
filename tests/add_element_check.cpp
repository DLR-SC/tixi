/* 
* Copyright (C) 2007-2012 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: add_attribute_check.cpp 179 2012-10-09 11:17:53Z markus.litz $ 
*
* Version: $Revision: 179 $
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#include "test.h" // Brings in the GTest framework

#include "tixi.h"


/** 
    @test Tests for addElement routines.
*/   

static TixiDocumentHandle documentHandle = -1;

class AddElementTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
    const char* rootElementName = "rootElement";
    tixiCreateDocument( rootElementName, &documentHandle );
  }
  
  //   virtual void TearDown() {}
};


TEST_F(AddElementTests, addElement_parentRoot)
{
    const char* parentPath = "/rootElement";
    const char* elementName = "child";
    const char* text = "This is my content.";

    ASSERT_TRUE( tixiAddTextElement( documentHandle, parentPath, elementName, text ) == SUCCESS );
}

TEST_F(AddElementTests, addElement_addTree)
{
    const char* parentPath = "/rootElement";
    const char* elementName = "level1";

    ASSERT_TRUE( tixiAddTextElement( documentHandle, parentPath, elementName,  NULL ) == SUCCESS );
    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement/level1", "level2", "level2" ) == SUCCESS );
    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement/level1", "level2", "level2" ) == SUCCESS );
}

TEST_F(AddElementTests, addElement_addElementAndParents)
{
    const char* parentPath = "/rootElement/newParent";
    const char* elementName = "withParent";

    ASSERT_TRUE( tixiAddTextElement( documentHandle, parentPath, elementName,  NULL ) == ELEMENT_NOT_FOUND );
}

TEST_F(AddElementTests, addElement_invalidName)
{
    const char* parentPath = "/parents/parent";
    const char* elementName = "4711";
    const char* text = "This is my content.";

    ASSERT_TRUE( tixiAddTextElement( documentHandle, parentPath, elementName, text ) == INVALID_XML_NAME );
}

TEST_F(AddElementTests, addElement_parentNotUnique)
{
    const char* parentPath = "/rootElement/level1/level2";
    const char* elementName = "i_am_a_child";
    const char* text = "This is my content.";

    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement", "level1", "" ) == SUCCESS );
    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement/level1", "level2", "" ) == SUCCESS );
    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement/level1", "level2", "" ) == SUCCESS );
    ASSERT_TRUE( tixiAddTextElement( documentHandle, parentPath, elementName, text ) == ELEMENT_PATH_NOT_UNIQUE );
}

TEST_F(AddElementTests, addElement_parentNotFound)
{
    const char* parentPath = "/this_is_not_a_parent";
    const char* elementName = "i_am_a_child";
    const char* text = "This is my content.";

    ASSERT_TRUE( tixiAddTextElement( documentHandle, parentPath, elementName, text ) == ELEMENT_NOT_FOUND );
}

TEST_F(AddElementTests, addElement_alreadySaved)
{
    const char* parentPath = "/parent";
    const char* elementName = "i_am_a_child";
    const char* text = "This is my content.";

    ASSERT_TRUE( tixiSaveDocument( documentHandle, "addElement.xml" ) == SUCCESS );
    ASSERT_TRUE( tixiAddTextElement( documentHandle, parentPath, elementName, text ) == ALREADY_SAVED );
}

TEST_F(AddElementTests, addElement_addDouble)
{
    const char* parentPath = "/rootElement";
    double number = 3.14159265;
    const char* format = "%12.8f";
    ASSERT_TRUE( tixiAddDoubleElement( documentHandle, parentPath, "double", number, format ) == SUCCESS );
    ASSERT_TRUE( tixiAddDoubleElement( documentHandle, parentPath, "double_g", number, NULL ) == SUCCESS );
}

TEST_F(AddElementTests, addElement_addInteger)
{
    const char* parentPath = "/rootElement";
    int number = 4711;
    const char* format = "%5d";
    ASSERT_TRUE( tixiAddIntegerElement( documentHandle, parentPath, "integer", number, format ) == SUCCESS );
    ASSERT_TRUE( tixiAddIntegerElement( documentHandle, parentPath, "integer_d", number, NULL ) == SUCCESS );
}

TEST_F(AddElementTests, addElement_addBoolean)
{
    const char* parentPath = "/rootElement";
    int val_true = 1;
    int val_false = 0;
    ASSERT_TRUE( tixiAddBooleanElement( documentHandle, parentPath, "boolean_t", val_true ) == SUCCESS );
    ASSERT_TRUE( tixiAddBooleanElement( documentHandle, parentPath, "boolean_f", val_false ) == SUCCESS );
    ASSERT_TRUE( tixiAddBooleanElement( documentHandle, parentPath, "boolean_x", 2 ) == FAILED );
}

TEST_F(AddElementTests, addElement_addPoint)
{
    const char* parentPath = "/rootElement";
    double x = 1.;
    double y = 2.;
    double z = 3.14159265;

    const char* format = "%10.3f";

    ASSERT_TRUE( tixiAddPoint( documentHandle, parentPath, x, y, z, format ) == SUCCESS );
    ASSERT_TRUE( tixiAddPoint( documentHandle, parentPath, x, y, z, NULL ) == SUCCESS );
}


TEST_F(AddElementTests, addElement_addTextElementAtIndex)
{
    char *text = NULL;
    const char *elementText = "Ganz anders";

    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement", "level1", "level1" ) == SUCCESS );
    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement", "level1", "level1" ) == SUCCESS );
    ASSERT_TRUE( tixiAddTextElementAtIndex( documentHandle, "/rootElement", "level1", elementText, 1 ) == SUCCESS );

    // Not check if the last added node is at position 0
    ASSERT_TRUE( tixiGetTextElement( documentHandle, "/rootElement/level1[1]", &text ) == SUCCESS );
    ASSERT_TRUE( !strcmp(text, elementText) );
}


TEST_F(AddElementTests, removeElement)
{
    char *text = NULL;
    int count = 0;

    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement", "level1", "level1" ) == SUCCESS );
    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement", "level1", "level1" ) == SUCCESS );
    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement", "level1", "level1" ) == SUCCESS );

    // count the elements
    ASSERT_TRUE( tixiGetNamedChildrenCount(documentHandle, "/rootElement", "level1", &count ) == SUCCESS );
    ASSERT_TRUE( count == 3 );

    // remove one element and count again
    ASSERT_TRUE( tixiRemoveElement( documentHandle, "/rootElement/level1[1]") == SUCCESS );
    ASSERT_TRUE( tixiGetNamedChildrenCount(documentHandle, "/rootElement", "level1", &count ) == SUCCESS );
    ASSERT_TRUE( count == 2 );
}

