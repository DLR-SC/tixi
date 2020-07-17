/*
* Copyright (C) 2018 German Aerospace Center (DLR/SC)
*
* Created: 2018-11-22 Martin Siggel <martin.siggel@dlr.de>
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


class GetTextElementTests : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    const char* xmlFilename = "TestData/textelements.xml";

    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS);
  }

  virtual void TearDown()
  {
    ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
  }

    TixiDocumentHandle documentHandle;
};


TEST_F(GetTextElementTests, normalCheck)
{
    char* text = NULL;
    EXPECT_EQ(SUCCESS, tixiGetTextElement(documentHandle, "/root/a", &text));
    EXPECT_STREQ("Text", text);

    EXPECT_EQ(SUCCESS, tixiGetTextElement(documentHandle, "/root/b/c", &text));
    EXPECT_STREQ("Text2", text);
}


TEST_F(GetTextElementTests, multipleTextNodes)
{
    char* text = NULL;
    EXPECT_EQ(SUCCESS, tixiGetTextElement(documentHandle, "/root/b/text()[1]", &text));
    EXPECT_STREQ("Text1", text);

    EXPECT_EQ(SUCCESS, tixiGetTextElement(documentHandle, "/root/b/text()[2]", &text));
    EXPECT_STREQ("Text3", text);

    int count = 0;
    EXPECT_EQ(SUCCESS, tixiGetNamedChildrenCount(documentHandle, "/root/b", "text()", &count));
    EXPECT_EQ(2, count);

    EXPECT_EQ(SUCCESS, tixiCheckElement(documentHandle, "/root/b/text()[1]"));

    EXPECT_EQ(SUCCESS, tixiGetTextElement(documentHandle, "/root/d", &text));
    EXPECT_STREQ("", text);
}

TEST_F(GetTextElementTests, specialChars)
{
    char* text = NULL;
    tixiAddTextElement(documentHandle, "/root", "c", "'bad < text ! &");
    EXPECT_EQ(SUCCESS, tixiGetTextElement(documentHandle, "/root/c", &text));
    EXPECT_STREQ("'bad < text ! &", text);
}
