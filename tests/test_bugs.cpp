/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2013-03-27 Martin Siggel <Martin.Siggel@dlr.de>
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


TEST(Bugs,cdatatext_segfault)
{
  TixiDocumentHandle handle;
  ReturnCode ret = tixiOpenDocument("TestData/bug_segfault.xml", &handle);
  ASSERT_EQ(SUCCESS, ret);

  char* text = NULL;
  int nChilds = 0;

  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/Assembly", &text));

  tixiGetNumberOfChilds(handle, "/Assembly/BodySkeleton[1]/BodyFrame[1]", &nChilds);
  ASSERT_EQ(1, nChilds);

  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/Assembly/BodySkeleton[1]/BodyFrame[1]", &text));

  // this query segfaultet due to its cdata section
  ASSERT_EQ(SUCCESS, tixiGetChildNodeName(handle, "/Assembly/BodySkeleton[1]/BodyFrame[1]", 1, &text));
  ASSERT_STREQ("#cdata-section", text);

}

// tests, if an empty text element can be updated
TEST(Bugs, empty_textelement)
{
  TixiDocumentHandle handle;
  ASSERT_EQ(SUCCESS, tixiCreateDocument("Root", &handle));

  // the first test always worked... creating empty text element
  ASSERT_EQ(SUCCESS, tixiAddTextElement(handle, "/Root", "TextEl1", ""));
  ASSERT_EQ(SUCCESS, tixiUpdateTextElement(handle, "/Root/TextEl1", "mytext1"));

  char* text = NULL;
  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/Root/TextEl1", &text));
  ASSERT_STREQ("mytext1", text);

  // the second test does not create a text element, but an empty node
  ASSERT_EQ(SUCCESS, tixiCreateElement(handle, "/Root", "TextEl2"));
  ASSERT_EQ(SUCCESS, tixiUpdateTextElement(handle, "/Root/TextEl2", "mytext2"));

  // here the test failed due to wrong implementation of tixiUpdateTextElement
  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/Root/TextEl2", &text));
  ASSERT_STREQ("mytext2", text);
}
