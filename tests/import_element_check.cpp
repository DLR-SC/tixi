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


class ImportElementsCheck : public testing::Test
{
protected:
    virtual void SetUp()
    {
        const char* xmlString = "<?xml version=\"1.0\"?>\n<root><a/><b/></root>\n";
        ASSERT_EQ(SUCCESS, tixiImportFromString(xmlString, &handle));
        tixiUsePrettyPrint(handle, 0);
    }

    virtual void TearDown()
    {
        tixiCloseDocument(handle);
    }

    TixiDocumentHandle handle;
};

TEST_F(ImportElementsCheck, importElem)
{
    char* text = NULL;

    ASSERT_EQ(SUCCESS, tixiImportElementFromString(handle, "/root/a", "<x>text</x>"));
    ASSERT_EQ(SUCCESS, tixiExportDocumentAsString(handle, &text));
    EXPECT_STREQ("<?xml version=\"1.0\"?>\n<root><a><x>text</x></a><b/></root>\n", text);
}

TEST_F(ImportElementsCheck, importElem2)
{
    char* text = NULL;

    ASSERT_EQ(SUCCESS, tixiImportElementFromString(handle, "/root/b", "<x attr=\"hello\"><c/></x>"));
    ASSERT_EQ(SUCCESS, tixiExportDocumentAsString(handle, &text));
    EXPECT_STREQ("<?xml version=\"1.0\"?>\n<root><a/><b><x attr=\"hello\"><c/></x></b></root>\n", text);
}

TEST_F(ImportElementsCheck, invalidHandle)
{
    ASSERT_EQ(INVALID_HANDLE, tixiImportElementFromString(-999, "/root/b", "<x attr=\"hello\"><c/></x>"));
}

TEST_F(ImportElementsCheck, invalidXPath)
{
    ASSERT_EQ(INVALID_XPATH, tixiImportElementFromString(handle, "cc/bla/na|<<me/blu", "<x attr=\"hello\"><c/></x>"));
}

TEST_F(ImportElementsCheck, elementNotFound)
{
    ASSERT_EQ(ELEMENT_NOT_FOUND, tixiImportElementFromString(handle, "/root/xyz", "<x attr=\"hello\"><c/></x>"));
}

TEST_F(ImportElementsCheck, elementNotUnique)
{
    ASSERT_EQ(ELEMENT_PATH_NOT_UNIQUE, tixiImportElementFromString(handle, "/root/node()", "<x attr=\"hello\"><c/></x>"));
}

TEST_F(ImportElementsCheck, notWellFormed)
{
    ASSERT_EQ(NOT_WELL_FORMED, tixiImportElementFromString(handle, "/root/a", "sdaf<||/sdfasf>"));
}

