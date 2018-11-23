/*
* Copyright (C) 2018 German Aerospace Center (DLR/SC)
*
* Created: 2018-11-23 Martin Siggel <martin.siggel@dlr.de>
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

#include <string>

#define EXPECT_XML_STR_EQ(expected_text, handle)    \
  {                                                 \
    char* text;                                     \
    tixiExportDocumentAsString(handle, &text);      \
    EXPECT_STREQ("<?xml version=\"1.0\"?>\n"        \
      expected_text                                 \
      "\n", text);                                  \
  }                                                 \

class SwapElementsCheck : public testing::Test
{
protected:
    virtual void SetUp()
    {
        xmlItem = "<root><a>text</a><b>text1<c>text2</c>text3</b></root>";
        ASSERT_EQ(SUCCESS, tixiImportFromString(xmlItem.c_str(), &handle));
        tixiUsePrettyPrint(handle, 0);
    }

    virtual void TearDown()
    {
        tixiCloseDocument(handle);
    }

    std::string xmlItem;
    TixiDocumentHandle handle;
};

TEST_F(SwapElementsCheck, SwapAB)
{
    ASSERT_EQ(SUCCESS, tixiSwapElements(handle, "/root/a", "/root/b"));
    EXPECT_XML_STR_EQ("<root><b>text1<c>text2</c>text3</b><a>text</a></root>", handle);
}

TEST_F(SwapElementsCheck, SwapTexts)
{
    ASSERT_EQ(SUCCESS, tixiSwapElements(handle, "/root/b/text()[1]", "/root/b/text()[2]"));
    EXPECT_XML_STR_EQ("<root><a>text</a><b>text3<c>text2</c>text1</b></root>", handle);
}

TEST_F(SwapElementsCheck, SwapWithParent)
{
    ASSERT_EQ(FAILED, tixiSwapElements(handle, "/root/b/c", "/root/b"));
}

TEST_F(SwapElementsCheck, OtherFailures)
{
    ASSERT_EQ(ELEMENT_NOT_FOUND, tixiSwapElements(handle, "/root/b/c", "/root/a/d"));
}
