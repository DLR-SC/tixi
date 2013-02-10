/* 
* Copyright (C) 2007-2012 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: update_element_check.cpp 179 2012-10-09 11:17:53Z markus.litz $
*
* Version: $Revision: 179 $
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
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
static const char* formatInt = "%5d";
static const char* formatDbl = "%12.8f";

class UpdateElementTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
    const char* rootElementName = "rootElement";
    const char* parentPath = "/rootElement";
    const char* elementName = "level1";

    tixiCreateDocument( rootElementName, &documentHandle );

    ASSERT_TRUE( tixiAddTextElement( documentHandle, "/rootElement", "text", "textContent" ) == SUCCESS );
    ASSERT_TRUE( tixiAddDoubleElement( documentHandle, "/rootElement", "double", 2.0, formatDbl ) == SUCCESS );
    ASSERT_TRUE( tixiAddIntegerElement( documentHandle, "/rootElement", "integer", 2, formatInt ) == SUCCESS );
    ASSERT_TRUE( tixiAddBooleanElement( documentHandle, "/rootElement", "boolean", 1 ) == SUCCESS );
  }

  //   virtual void TearDown() {}
};


TEST_F(UpdateElementTests, updateTextElement_success)
{
    const char* parentPath = "/rootElement/text";
    const char* text = "This is my content.";
    char *text2 = NULL;

    ASSERT_TRUE( tixiUpdateTextElement( documentHandle, parentPath, text ) == SUCCESS );
    ASSERT_TRUE( tixiGetTextElement( documentHandle, parentPath, &text2 ) == SUCCESS );
    ASSERT_TRUE( !strcmp(text2, text ));
}

TEST_F(UpdateElementTests, updateTextElement_wrong_path)
{
    const char* parentPath = "/rootElement/not_exists";
    const char* text = "This is my content.";

    ASSERT_TRUE( tixiUpdateTextElement( documentHandle, parentPath, text ) == FAILED );
}

TEST_F(UpdateElementTests, updateDoubleElement_success)
{
    const char* parentPath = "/rootElement/double";
    double val = 666.666;
    double val2 = 0.0;

    ASSERT_TRUE( tixiUpdateDoubleElement( documentHandle, parentPath, val, formatDbl ) == SUCCESS );
    ASSERT_TRUE( tixiGetDoubleElement( documentHandle, parentPath, &val2 ) == SUCCESS );
    ASSERT_TRUE( val == val2 );
}

TEST_F(UpdateElementTests, updateDoubleElement_path_not_exists)
{
    const char* parentPath = "/rootElement/wring_path";
    double val = 666.666;

    ASSERT_TRUE( tixiUpdateDoubleElement( documentHandle, parentPath, val, formatDbl ) == FAILED );
}

TEST_F(UpdateElementTests, updateIntegerElement_success)
{
    const char* parentPath = "/rootElement/integer";
    int val = 42;
    int val2 = 0;

    ASSERT_TRUE( tixiUpdateIntegerElement( documentHandle, parentPath, val, formatInt ) == SUCCESS );
    ASSERT_TRUE( tixiGetIntegerElement( documentHandle, parentPath, &val2 ) == SUCCESS );
    ASSERT_TRUE( val == val2 );
}

TEST_F(UpdateElementTests, updateBooleanElement_success)
{
    const char* parentPath = "/rootElement/boolean";
    int val = 0;
    int val2 = 1;

    ASSERT_TRUE( tixiUpdateBooleanElement( documentHandle, parentPath, val ) == SUCCESS );
    ASSERT_TRUE( tixiGetBooleanElement( documentHandle, parentPath, &val2 ) == SUCCESS );
    ASSERT_TRUE( val == val2 );
}

TEST_F(UpdateElementTests, updateBooleanElement_not_bool)
{
    const char* parentPath = "/rootElement/boolean";
    int val = 2;

    ASSERT_TRUE( tixiUpdateBooleanElement( documentHandle, parentPath, val ) == FAILED );
}
