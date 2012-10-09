/*
* Copyright (C) 2007-2012 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: add_attribute_check.cpp 176 2012-10-08 21:20:23Z markus.litz $
*
* Version: $Revision: 176 $
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
    @test Tests for validating (against) schema files.
*/

static TixiDocumentHandle documentHandle = -1;

class ValidateSchemaTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
     char* xmlFilename = "TestData/valid_CPACS_dokumentiert.xml";
     ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS);
  }

  virtual void TearDown() {
      ASSERT_TRUE ( tixiCloseDocument( documentHandle ) == SUCCESS );
  }
};

TEST_F(ValidateSchemaTests, tixiValidateSchema)
{
    char* schemaFilename = "TestData/valid_cpacs_schema.xsd";
    ASSERT_TRUE( tixiSchemaValidateFromFile( documentHandle, schemaFilename ) == SUCCESS);
}
