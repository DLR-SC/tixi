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

#include <cstdio>

#define N_VALUES 5

static double values[N_VALUES] = {0., 1., 2., 3., 4.};
static const char* attributes[N_VALUES] = { "a", "b", "c", "d", "e"};
static const char* parentPath = "/root";
static const char* listName = "numbers";
static const char* childName = "number";
static const char* format = "%08.2f";
static const char* childAttributeName = "letter";
static int nValues = N_VALUES;

/**
    @test Tests for adding composite elements.
*/

class CompositeTests : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    const char* rootElementName = "root";
    documentHandle = -1;
    ASSERT_TRUE( tixiCreateDocument( rootElementName, &documentHandle ) == SUCCESS);
  }

  virtual void TearDown()
  {
    ASSERT_EQ(SUCCESS, tixiCloseDocument(documentHandle));
    documentHandle = -1;
  }

  TixiDocumentHandle documentHandle;
};


TEST_F(CompositeTests, add_list)
{
  ASSERT_TRUE( tixiAddDoubleListWithAttributes( documentHandle, parentPath,  listName, childName, childAttributeName,
                                                values,  format,  attributes,  nValues ) == SUCCESS );

  ASSERT_TRUE( tixiAddDoubleListWithAttributes( documentHandle, parentPath,  "numbers2", childName, childAttributeName,
                                                values,  NULL,  attributes,  nValues ) == SUCCESS );
}

TEST_F(CompositeTests, add_list_invalid_handle)
{
  ASSERT_EQ(INVALID_HANDLE, tixiAddDoubleListWithAttributes( -1, parentPath,  listName, childName, childAttributeName,
                                                             values,  format,  attributes,  nValues ) );
}

TEST_F(CompositeTests, add_list_already_saved)
{
  ASSERT_EQ( SUCCESS, tixiAddDoubleListWithAttributes( documentHandle, parentPath,  listName, childName, childAttributeName,
                                                       values,  format,  attributes,  nValues ) );

  const char* tmpfile = "tmp12345656.xml";
  ASSERT_EQ(SUCCESS, tixiSaveDocument(documentHandle, tmpfile));

  ASSERT_EQ(ALREADY_SAVED, tixiAddDoubleListWithAttributes( documentHandle, parentPath,  "numbers2", childName, childAttributeName,
                                                            values,  format,  attributes,  nValues ) );

  remove(tmpfile);
}

TEST_F(CompositeTests, add_list_invalid_names)
{
  ASSERT_EQ( INVALID_XML_NAME, tixiAddDoubleListWithAttributes( documentHandle, parentPath,  "list\\Name", childName, childAttributeName,
                                                                values,  format,  attributes,  nValues ) );

  ASSERT_EQ( INVALID_XML_NAME, tixiAddDoubleListWithAttributes( documentHandle, parentPath,  listName, "child\\Name", childAttributeName,
                                                                values,  format,  attributes,  nValues ) );

  ASSERT_EQ( INVALID_XML_NAME, tixiAddDoubleListWithAttributes( documentHandle, parentPath,  listName, childName, "childAttr\\ibuteName",
                                                                values,  format,  attributes,  nValues ) );
}
