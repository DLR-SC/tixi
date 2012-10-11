/* 
* Copyright (C) 2007-2012 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id$ 
*
* Version: $Revision$
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
    @test Tests for addattribute routines.
*/   

static TixiDocumentHandle documentHandle = -1;

class AttributeTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    char* rootElementName = "rootElement";
    char* elementName = "element";

    tixiCreateDocument( rootElementName, &documentHandle );
    tixiAddTextElement( documentHandle, "/rootElement", elementName, "content" );
  }
  
  // virtual void TearDown() {}
};

TEST_F(AttributeTest, add_text_attribute) 
{
    char* elementPath = "/rootElement/element";
    char* attributeName = "attribute";

    ASSERT_TRUE( tixiAddTextAttribute( documentHandle, elementPath, attributeName, "attributeValue" ) == SUCCESS );
}


TEST_F(AttributeTest, add_double_attribute) 
{
  char* elementPath = "/rootElement/element";
  double value = 3.14159;
  char* format = "%10.6f";

  ASSERT_TRUE( tixiAddDoubleAttribute( documentHandle, elementPath, "doubleAttribute", value, format ) == SUCCESS );
  ASSERT_TRUE( tixiAddDoubleAttribute( documentHandle, elementPath, "doubleAttribute_g", value, NULL ) == SUCCESS );
}

TEST_F(AttributeTest, add_integer_attribute) 
{
  char* elementPath = "/rootElement/element";
  int value = 4711;
  char* format = "%5d";
  
  ASSERT_TRUE( tixiAddIntegerAttribute( documentHandle, elementPath, "intAttribute", value, format ) == SUCCESS );
  ASSERT_TRUE( tixiAddIntegerAttribute( documentHandle, elementPath, "intAttribute_d", value, NULL ) == SUCCESS );
}


#if __unix__ || unix || __APPLE__
TEST_F(AttributeTest, attribute_compare) 
{
    char* elementPath = "/rootElement/element";
    char* attributeName = "attribute";
    ASSERT_TRUE( tixiAddTextAttribute( documentHandle, elementPath, attributeName, "attributeValue" ) == SUCCESS );

    char* elementPath2 = "/rootElement/element";
    double value2 = 3.14159;
    char* format2 = "%10.6f";
    ASSERT_TRUE( tixiAddDoubleAttribute( documentHandle, elementPath2, "doubleAttribute", value2, format2 ) == SUCCESS );
    ASSERT_TRUE( tixiAddDoubleAttribute( documentHandle, elementPath2, "doubleAttribute_g", value2, NULL ) == SUCCESS );

    char* elementPath3 = "/rootElement/element";
    int value3 = 4711;
    char* format3 = "%5d";
    ASSERT_TRUE( tixiAddIntegerAttribute( documentHandle, elementPath3, "intAttribute", value3, format3 ) == SUCCESS );
    ASSERT_TRUE( tixiAddIntegerAttribute( documentHandle, elementPath3, "intAttribute_d", value3, NULL ) == SUCCESS );

    
    // check against a reference
    ASSERT_TRUE( tixiSaveDocument( documentHandle, "addAttribute.xml" ) == SUCCESS );
    ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
    ASSERT_TRUE( system("diff -w addAttribute.xml TestData/addAttributeReference.xml" ) == 0 );
}
#endif /* __unix__ */



