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
    @test Tests for getting attributes.
*/

static TixiDocumentHandle documentHandle = -1;

class GetAttributeTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
     char* xmlFilename = "TestData/in.xml";

     ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS);
  }

  virtual void TearDown() {
      ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
  }
};

TEST_F(GetAttributeTests, invalidHandle)
{
    char* text = NULL;
    char* elementPath = "/plane/name";
    char* attributeName = "non_existing_attribute";

    ASSERT_TRUE( tixiGetTextAttribute( -1, elementPath, attributeName, &text ) == INVALID_HANDLE );
}

TEST_F(GetAttributeTests, attributeNotFound)
{
    char* text = NULL;
    char* elementPath = "/plane/wings/wing[1]";
    char* attributeName = "non_existing_attribute";
    ASSERT_TRUE( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == ATTRIBUTE_NOT_FOUND );
}

TEST_F(GetAttributeTests, elementNotFound)
{
    char* text = NULL;
    char* elementPath = "/plane/name/pp";
    char* attributeName = "non_existing_attribute";
    ASSERT_TRUE( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == ELEMENT_NOT_FOUND );
}

TEST_F(GetAttributeTests, invalidXpath)
{
    char* text = NULL;
    char* elementPath = "cc/plane/name|||/pp";
    char* attributeName = "position";
    ASSERT_TRUE( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == INVALID_XPATH );
}

TEST_F(GetAttributeTests, notUnique)
{
    char* text = NULL;
    char* elementPath = "/plane/wings/wing";
    char* attributeName = "position";
    ASSERT_TRUE( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == ELEMENT_PATH_NOT_UNIQUE );
}

TEST_F(GetAttributeTests, getDoubleAttribute)
{
    double number = 0.;
    char* elementPath = "/plane/coordinateOrigin";
    char* attributeName = "scaling";
    ASSERT_TRUE( tixiGetDoubleAttribute( documentHandle, elementPath, attributeName, &number ) == SUCCESS );
    EXPECT_EQ( number, 1.3456);
}

TEST_F(GetAttributeTests, getIntegerAttribute)
{
    int number = 0;
    char* elementPath = "/plane/wings";
    char* attributeName = "numberOfWings";

    ASSERT_TRUE( tixiGetIntegerAttribute( documentHandle, elementPath, attributeName, &number ) == SUCCESS );
    EXPECT_EQ( number , 2 );
}
