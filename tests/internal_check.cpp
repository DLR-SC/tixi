/*
* Copyright (C) 2020 German Aerospace Center (DLR/SC)
*
* Created: 2020-07-17 Martin Siggel <martin.siggel@dlr.de>
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
#include "tixiInternal.h"

TEST(InternalCheck, getChildNodeCount)
{
    TixiDocumentHandle handle = 0;

    ASSERT_EQ(SUCCESS, tixiOpenDocument("TestData/textelements.xml", &handle));
    TixiDocument *document = getDocument(handle);

    xmlNodePtr element = NULL;
    ASSERT_EQ(SUCCESS, checkElement(document->xpathContext, "/root", &element));
    EXPECT_EQ(2, getChildNodeCount(element));

    ASSERT_EQ(SUCCESS, checkElement(document->xpathContext, "/root/a", &element));
    EXPECT_EQ(1, getChildNodeCount(element));

    ASSERT_EQ(SUCCESS, checkElement(document->xpathContext, "/root/a/text()", &element));
    EXPECT_EQ(0, getChildNodeCount(element));

    ASSERT_EQ(SUCCESS, checkElement(document->xpathContext, "/root/b", &element));
    EXPECT_EQ(3, getChildNodeCount(element));

    ASSERT_EQ(SUCCESS, checkElement(document->xpathContext, "/root/b/c", &element));
    EXPECT_EQ(1, getChildNodeCount(element));

    tixiCloseDocument(handle);
}
