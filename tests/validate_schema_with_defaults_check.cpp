/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2016-08-23 Melven Roehrig-Zoellner <Melven.Roehrig-Zoellner@DLR.de>
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
  @test Tests for validating (against) schema files with default elements/attributes.
 */

static TixiDocumentHandle documentHandle = -1;

class ValidateSchemaWithDefaultsTests : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    const char* xmlFilename = "TestData/schema_with_defaults.xml";
    ASSERT_EQ(SUCCESS, tixiOpenDocument( xmlFilename, &documentHandle ));
  }

  virtual void TearDown()
  {
    ASSERT_EQ(SUCCESS, tixiCloseDocument( documentHandle ));
  }
};

TEST_F(ValidateSchemaWithDefaultsTests, tixiValidateSchemaFromFile)
{
  const char* schemaFilename = "TestData/schema_with_defaults.xsd";
  ASSERT_EQ(SUCCESS, tixiSchemaValidateFromFile( documentHandle, schemaFilename ));

  // check that defaults are not there!
  char *text = NULL;
  int i;
  ASSERT_EQ(SUCCESS, tixiGetTextElement( documentHandle, "/configuration/defaultString", &text ));
  ASSERT_STREQ("", text);
  ASSERT_EQ(ATTRIBUTE_NOT_FOUND, tixiGetIntegerAttribute( documentHandle, "/configuration/elemWithDefaultAttr", "defaultAttr", &i ));
}

TEST_F(ValidateSchemaWithDefaultsTests, tixiValidateSchemaWithDefaultsFromFile)
{
  const char* schemaFilename = "TestData/schema_with_defaults.xsd";
  ASSERT_EQ(SUCCESS, tixiSchemaValidateWithDefaultsFromFile( documentHandle, schemaFilename));

  // check that defaults have been inserted
  char *text = NULL;
  int i;
  ASSERT_EQ(SUCCESS, tixiGetTextElement( documentHandle, "/configuration/defaultString", &text));
  ASSERT_STREQ("Default string value", text);
  ASSERT_EQ(SUCCESS, tixiGetIntegerAttribute( documentHandle, "/configuration/elemWithDefaultAttr", "defaultAttr", &i ));
  ASSERT_EQ(4, i);
}

TEST_F(ValidateSchemaWithDefaultsTests, tixiValidateSchemaWithDefaultsFromFile_notAFile)
{
  const char* schemaFilename = "TestData/InvaLid_ScheMa_fIleE_nAmE.xsd";
  ASSERT_EQ(OPEN_SCHEMA_FAILED, tixiSchemaValidateWithDefaultsFromFile( documentHandle, schemaFilename ));
}
