/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2015-05-06 Martin Siggel <martin.siggel@dlr.de>
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

#include "test.h"
#include "tixi.h"

class SaveDocumentCheck : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    ASSERT_EQ(SUCCESS, tixiOpenDocument ( "TestData/externaldata-save.xml", &handle));
    ASSERT_EQ(SUCCESS, tixiUsePrettyPrint(handle, 1));
  }

  virtual void TearDown()
  {
    ASSERT_EQ (SUCCESS, tixiCloseDocument( handle ));
  }

  TixiDocumentHandle handle;
};

TEST_F(SaveDocumentCheck, saveSplitted)
{
  ASSERT_EQ(SUCCESS, tixiSaveDocument(handle, "save-splitted.xml"));
}

TEST_F(SaveDocumentCheck, saveComplete)
{
  ASSERT_EQ(SUCCESS, tixiSaveCompleteDocument(handle, "save-complete.xml"));
}

TEST_F(SaveDocumentCheck, saveRemove)
{
  ASSERT_EQ(SUCCESS, tixiSaveAndRemoveDocument(handle, "save-remove.xml"));
}

