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

// tests, if updating a text element that contains comments (ignored by GetText) does not prepend to existing text
TEST(Bugs, textelement_with_comment_in_the_middle)
{
  const char* s = "<?xml version=\"1.0\"?><root><elem>text_before <!-- comment_text --> text_after</elem></root>";
  TixiDocumentHandle handle;
  ASSERT_EQ(SUCCESS, tixiImportFromString(s, &handle));

  char* text = NULL;
  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/root/elem", &text));
  ASSERT_STREQ("text_before  text_after", text);

  ASSERT_EQ(FAILED, tixiUpdateTextElement(handle, "/root/elem", "new text"));

  /*
  // this could also be desirable behavior but the documentation currently states that this should not work!
  ASSERT_EQ(SUCCESS, tixiUpdateTextElement(handle, "/root/elem", "new text"));

  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/root/elem", &text));
  ASSERT_STREQ("new text", text);
  */

  tixiCloseDocument(handle);
}

TEST(Bugs, textelement_before_comment)
{
  const char* s = "<?xml version=\"1.0\"?><root><elem>some_text <!-- comment_text --></elem></root>";
  TixiDocumentHandle handle;
  ASSERT_EQ(SUCCESS, tixiImportFromString(s, &handle));

  char* text = NULL;
  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/root/elem", &text));
  ASSERT_STREQ("some_text ", text);

  ASSERT_EQ(FAILED, tixiUpdateTextElement(handle, "/root/elem", "new text"));

  /*
  // this could also be desirable behavior but the documentation currently states that this should not work!
  ASSERT_EQ(SUCCESS, tixiUpdateTextElement(handle, "/root/elem", "new text"));

  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/root/elem", &text));
  ASSERT_STREQ("new text", text);
  */

  tixiCloseDocument(handle);
}

TEST(Bugs, textelement_after_comment)
{
  const char* s = "<?xml version=\"1.0\"?><root><elem><!-- comment_text --> text_after</elem></root>";
  TixiDocumentHandle handle;
  ASSERT_EQ(SUCCESS, tixiImportFromString(s, &handle));

  char* text = NULL;
  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/root/elem", &text));
  ASSERT_STREQ(" text_after", text);

  ASSERT_EQ(FAILED, tixiUpdateTextElement(handle, "/root/elem", "new text"));

  /*
  // this could also be desirable behavior but the documentation currently states that this should not work!
  ASSERT_EQ(SUCCESS, tixiUpdateTextElement(handle, "/root/elem", "new text"));

  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/root/elem", &text));
  ASSERT_STREQ("new text", text);
  */

  tixiCloseDocument(handle);
}

// directly access the 'text()' of an element (should behave the same for GetText and UpdateText)
TEST(Bugs, textelement_directly_access_text)
{
  const char* s = "<?xml version=\"1.0\"?><root><elem>some text</elem></root>";
  TixiDocumentHandle handle;
  ASSERT_EQ(SUCCESS, tixiImportFromString(s, &handle));

  char* text = NULL;
  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/root/elem/text()", &text));
  ASSERT_STREQ("some text", text);

  ASSERT_EQ(NOT_AN_ELEMENT, tixiUpdateTextElement(handle, "/root/elem/text()", "new text"));

  /*
  // this could also be desirable behavior but the documentation currently states that this should not work!
  ASSERT_EQ(SUCCESS, tixiUpdateTextElement(handle, "/root/elem/text()", "new text"));

  ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/root/elem/text()", &text));
  ASSERT_STREQ("new text", text);
  */

  tixiCloseDocument(handle);
}


extern "C" void printMsg(MessageType type, const char* message, ...);

TEST(Bugs, printMessageCrash)
{
    char msg[3000];
    for (int i = 0; i < 3000; ++i) {
        msg[i] = 'a';
    }
    msg[2999] = '\0';

    printMsg(MESSAGETYPE_ERROR, "%s", msg);
}
