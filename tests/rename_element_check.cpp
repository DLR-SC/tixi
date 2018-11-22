/*
* Copyright (C) 2018 German Aerospace Center (DLR/SC)
*
* Created: 2018-06-29 Jan Kleinert <Jan.Kleinert@dlr.de>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*   http://www.apache.org/licenses/LICENSE-2.0
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
  @test Tests for renaming element routine.
*/

static TixiDocumentHandle documentHandle = -1;

class RenameElementTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
     const char* xmlFilename = "TestData/in.xml";
     ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS);
  }

  virtual void TearDown() {
      ASSERT_TRUE ( tixiCloseDocument( documentHandle ) == SUCCESS );
  }
};

TEST_F(RenameElementTests, renameSingleElement)
{
    ASSERT_TRUE( tixiRenameElement( -1, "/plane/aPoint", "point", "thisnameiswaycooler") == INVALID_HANDLE);
    ASSERT_TRUE( tixiRenameElement( documentHandle, "/plane/idontexist", "point", "thisnameiswaycooler") == ELEMENT_NOT_FOUND);
    ASSERT_TRUE( tixiRenameElement( documentHandle, "/plane/aPoint", "idontexist", "thisnameiswaycooler") == ELEMENT_NOT_FOUND);


    ASSERT_TRUE( tixiRenameElement( documentHandle, "/plane/aPoint", "point", "thisnameiswaycooler" ) == SUCCESS);
    ASSERT_TRUE( tixiCheckElement( documentHandle, "/plane/aPoint/thisnameiswaycooler") == SUCCESS);
    ASSERT_TRUE( tixiCheckElement( documentHandle, "/plane/aPoint/thisnameiswaycooler/z") == SUCCESS);
}
