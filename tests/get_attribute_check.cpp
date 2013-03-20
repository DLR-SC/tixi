/*
* Copyright (C) 2007-2012 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: add_attribute_check.cpp 173 2012-10-08 20:06:49Z markus.litz $
*
* Version: $Revision: 173 $
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
    @test Tests for getting attributes.
*/

static TixiDocumentHandle documentHandle = -1;

class GetAttributeTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
     const char* xmlFilename = "TestData/in.xml";

     ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS);
  }

  virtual void TearDown() {
      ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
  }
};

TEST_F(GetAttributeTests, invalidHandle)
{
    char* text = NULL;
    const char* elementPath = "/plane/name";
    const char* attributeName = "non_existing_attribute";

    ASSERT_TRUE( tixiGetTextAttribute( -1, elementPath, attributeName, &text ) == INVALID_HANDLE );
}

TEST_F(GetAttributeTests, attributeNotFound)
{
    char* text = NULL;
    const char* elementPath = "/plane/wings/wing[1]";
    const char* attributeName = "non_existing_attribute";
    ASSERT_TRUE( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == ATTRIBUTE_NOT_FOUND );
}

TEST_F(GetAttributeTests, elementNotFound)
{
    char* text = NULL;
    const char* elementPath = "/plane/name/pp";
    const char* attributeName = "non_existing_attribute";
    ASSERT_TRUE( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == ELEMENT_NOT_FOUND );
}

TEST_F(GetAttributeTests, invalidXpath)
{
    char* text = NULL;
    const char* elementPath = "cc/plane/name|||/pp";
    const char* attributeName = "position";
    ASSERT_TRUE( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == INVALID_XPATH );
}

TEST_F(GetAttributeTests, notUnique)
{
    char* text = NULL;
    const char* elementPath = "/plane/wings/wing";
    const char* attributeName = "position";
    ASSERT_TRUE( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == ELEMENT_PATH_NOT_UNIQUE );
}

TEST_F(GetAttributeTests, getDoubleAttribute)
{
    double number = 0.;
    const char* elementPath = "/plane/coordinateOrigin";
    const char* attributeName = "scaling";
    ASSERT_TRUE( tixiGetDoubleAttribute( documentHandle, elementPath, attributeName, &number ) == SUCCESS );
    EXPECT_EQ( number, 1.3456);
}

TEST_F(GetAttributeTests, getIntegerAttribute)
{
    int number = 0;
    const char* elementPath = "/plane/wings";
    const char* attributeName = "numberOfWings";

    ASSERT_TRUE( tixiGetIntegerAttribute( documentHandle, elementPath, attributeName, &number ) == SUCCESS );
    EXPECT_EQ( number , 2 );
}

TEST_F(GetAttributeTests, getNumberOfAttributes){
    int number = 0;
    ASSERT_EQ(SUCCESS, tixiGetNumberOfAttributes(documentHandle, "/plane/wings/wing[1]", &number ));
    ASSERT_EQ(1, number);

    ASSERT_EQ(SUCCESS, tixiGetNumberOfAttributes(documentHandle, "/plane/name", &number ));
    ASSERT_EQ(0, number);

    ASSERT_EQ(SUCCESS, tixiGetNumberOfAttributes(documentHandle, "/plane/coordinateOrigin/", &number ));
    ASSERT_EQ(1, number);
}

TEST_F(GetAttributeTests, getAttributeNames){
    int number = 0;
    char * name = NULL;
    ASSERT_EQ(SUCCESS, tixiGetNumberOfAttributes(documentHandle, "/plane/aPoint[1]", &number ));
    ASSERT_EQ(2, number);
    
    ASSERT_EQ(SUCCESS, tixiGetAttributeName(documentHandle, "/plane/aPoint[1]", 1, &name));
    ASSERT_STREQ("system", name);
    
    ASSERT_EQ(SUCCESS, tixiGetAttributeName(documentHandle, "/plane/aPoint[1]/", 2, &name));
    ASSERT_STREQ("type", name);
    
    //check invalid indices
    ASSERT_EQ(INDEX_OUT_OF_RANGE, tixiGetAttributeName(documentHandle, "/plane/aPoint[1]", 3, &name));
    ASSERT_EQ(INDEX_OUT_OF_RANGE, tixiGetAttributeName(documentHandle, "/plane/aPoint[1]", 0, &name));
    
    //check invalid paths
    ASSERT_EQ(ELEMENT_NOT_FOUND, tixiGetAttributeName(documentHandle, "/plane/aPointInvalid", 2, &name));
    
    //check invalid handle
    ASSERT_EQ(INVALID_HANDLE, tixiGetAttributeName(-1, "/plane/aPoint[1]", 1, &name));
}
