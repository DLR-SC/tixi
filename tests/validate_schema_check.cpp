/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
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
  @test Tests for validating (against) schema files.
 */

static TixiDocumentHandle documentHandle = -1;

class ValidateSchemaTests : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    const char* xmlFilename = "TestData/valid_CPACS_dokumentiert.xml";
    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS);
  }

  virtual void TearDown()
  {
    ASSERT_TRUE ( tixiCloseDocument( documentHandle ) == SUCCESS );
  }
};

TEST_F(ValidateSchemaTests, tixiValidateSchemaFromFile)
{
  const char* schemaFilename = "TestData/valid_cpacs_schema.xsd";
  ASSERT_TRUE( tixiSchemaValidateFromFile( documentHandle, schemaFilename ) == SUCCESS);
}

TEST_F(ValidateSchemaTests, tixiValidateSchemaFromFile_notAFile)
{
  const char* schemaFilename = "TestData/InvaLid_ScheMa_fIleE_nAmE.xsd";
  ASSERT_TRUE( tixiSchemaValidateFromFile( documentHandle, schemaFilename ) == OPEN_SCHEMA_FAILED);
}

TEST_F(ValidateSchemaTests, tixiValidateSchemaFromString)
{
  size_t f_size = 0;
  char* schemaString = NULL;
  const char* schemaFilename = "TestData/valid_cpacs_schema.xsd";

  // read schema file into string
  FILE* fp = fopen(schemaFilename, "r");

  //determine file size, not nice but portable
  while(getc(fp) != EOF) {
    f_size++;
  }

  fseek(fp, 0, SEEK_SET);
  schemaString = (char*)malloc(sizeof(char) * (f_size+4));
  fread(schemaString, 1, f_size, fp);
  schemaString[f_size] = '\0';

  ASSERT_TRUE( tixiSchemaValidateFromString( documentHandle, schemaString ) == SUCCESS);
  free(schemaString);
}

TEST_F(ValidateSchemaTests, tixiValidateSchemaFromString_not_A_Schema)
{
  long f_size = 0;
  const char* schemaString = "<this_is_not_a_valid_schema><<>";

  ASSERT_TRUE( tixiSchemaValidateFromString( documentHandle, schemaString ) == OPEN_SCHEMA_FAILED);
}
