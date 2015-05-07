/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2014-06-16 Martin Siggel <martin.siggel@dlr.de>
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

class GetNodeType : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    ASSERT_TRUE( tixiOpenDocument  ( "TestData/doctype.xml",   &inDocumentHandle)  == SUCCESS);
  }

  virtual void TearDown()
  {
    ASSERT_TRUE ( tixiCloseDocument( inDocumentHandle ) == SUCCESS );
  }

protected:
  TixiDocumentHandle inDocumentHandle;
};

TEST_F(GetNodeType, doctype_node)
{
  char* type = NULL;
  ASSERT_EQ(SUCCESS, tixiGetNodeType(inDocumentHandle, "/DAVEfunc[1]", &type));
  ASSERT_STREQ("ELEMENT_NODE", type);

  int childs = 0;
  ASSERT_EQ(SUCCESS, tixiGetNumberOfChilds(inDocumentHandle, "/", &childs));
  ASSERT_EQ(1, childs);
  ASSERT_EQ(SUCCESS, tixiGetNamedChildrenCount(inDocumentHandle, "/", "DAVEfunc", &childs));
  ASSERT_EQ(1, childs);

  char* childname;
  ASSERT_EQ(SUCCESS, tixiGetChildNodeName(inDocumentHandle, "/", 1, &childname));
  ASSERT_STREQ("DAVEfunc", childname);
}

