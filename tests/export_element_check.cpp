/*
* Copyright (C) 2022 German Aerospace Center (DLR/SC)
*
* Created: 2022-02-23 Melven Roehrig-Zoellner <Melven.Roehrig-Zoellner@DLR.de>
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


class ExportElementsCheck : public testing::Test
{
protected:
    virtual void SetUp()
    {
        const char* xmlString = "<root><a><b>text</b><c/></a><b attr=\"hello\">text2</b></root>";
        ASSERT_EQ(SUCCESS, tixiImportFromString(xmlString, &handle));
        tixiUsePrettyPrint(handle, 0);
    }

    virtual void TearDown()
    {
        tixiCloseDocument(handle);
    }

    TixiDocumentHandle handle;
};

TEST_F(ExportElementsCheck, exportElem)
{
    char* text = NULL;

    ASSERT_EQ(SUCCESS, tixiExportElementAsString(handle, "/root/a", &text));
    EXPECT_STREQ("<a><b>text</b><c/></a>", text);
}

TEST_F(ExportElementsCheck, exportElem2)
{
    char* text = NULL;

    ASSERT_EQ(SUCCESS, tixiExportElementAsString(handle, "/root/b", &text));
    EXPECT_STREQ("<b attr=\"hello\">text2</b>", text);
}

TEST_F(ExportElementsCheck, exportElem_root)
{
    char* text = NULL;

    ASSERT_EQ(SUCCESS, tixiExportElementAsString(handle, "/root", &text));
    EXPECT_STREQ("<root><a><b>text</b><c/></a><b attr=\"hello\">text2</b></root>", text);
}

TEST_F(ExportElementsCheck, exportElem_pretty)
{
    char* text = NULL;

    ASSERT_EQ(SUCCESS, tixiUsePrettyPrint(handle, 1));
    ASSERT_EQ(SUCCESS, tixiExportElementAsString(handle, "/root/a", &text));
    EXPECT_STREQ("<a>\n  <b>text</b>\n  <c/>\n</a>", text);
}

TEST_F(ExportElementsCheck, invalidHandle)
{
    char* text;

    ASSERT_EQ(INVALID_HANDLE, tixiExportElementAsString(-999, "/root", &text));
    ASSERT_EQ(NULL, text);
}

TEST_F(ExportElementsCheck, invalidXPath)
{
    char* text;

    ASSERT_EQ(INVALID_XPATH, tixiExportElementAsString(handle, "cc/plane/na|<<me/pp", &text));
    ASSERT_EQ(NULL, text);
}

TEST_F(ExportElementsCheck, elementNotFound)
{
    char* text;

    ASSERT_EQ(ELEMENT_NOT_FOUND, tixiExportElementAsString(handle, "root", &text));
    ASSERT_EQ(NULL, text);
}

TEST_F(ExportElementsCheck, elementNotUnique)
{
    char* text;

    ASSERT_EQ(ELEMENT_PATH_NOT_UNIQUE, tixiExportElementAsString(handle, "/root/node()", &text));
    ASSERT_EQ(NULL, text);
}

