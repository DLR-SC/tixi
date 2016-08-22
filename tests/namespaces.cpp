/*
* Copyright (C) 2016 German Aerospace Center (DLR/SC)
*
* Created: 2016-08-22 Martin Siggel <martin.siggel@dlr.de>
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

class Namespaces : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    ASSERT_TRUE( tixiOpenDocument  ( "TestData/namespaces_1.xml",   &inDocumentHandle)  == SUCCESS);
  }

  virtual void TearDown()
  {
    ASSERT_TRUE ( tixiCloseDocument( inDocumentHandle ) == SUCCESS );
  }

protected:
  TixiDocumentHandle inDocumentHandle;
};

TEST_F(Namespaces, register_ns)
{
  char* text = NULL;

  // xpath not registered, html prefix not known
  ASSERT_EQ(INVALID_XPATH, tixiGetTextElement(inDocumentHandle, "/root/html:table/html:tr/html:td[1]/", &text));
  ASSERT_EQ(INVALID_XPATH, tixiGetTextElement(inDocumentHandle, "/root/h:table/h:tr/h:td[1]/", &text));

  ASSERT_EQ(SUCCESS, tixiRegisterNamespace(inDocumentHandle, "http://www.w3.org/TR/html4/", "html"));
  ASSERT_EQ(SUCCESS, tixiRegisterNamespace(inDocumentHandle, "http://www.w3.org/TR/html4/", "h"));

  ASSERT_EQ(SUCCESS, tixiGetTextElement(inDocumentHandle, "/root/html:table/html:tr/html:td[1]/", &text));
  ASSERT_STREQ("Apples", text);

  ASSERT_EQ(SUCCESS, tixiGetTextElement(inDocumentHandle, "/root/h:table/h:tr/h:td[2]/", &text));
  ASSERT_STREQ("Bananas", text);

  // check and register default cpacs namespace
  ASSERT_EQ(ELEMENT_NOT_FOUND, tixiCheckElement(inDocumentHandle, "/root/aircraft"));
  ASSERT_EQ(INVALID_XPATH, tixiCheckElement(inDocumentHandle, "/root/cpacs:aircraft"));
  ASSERT_EQ(SUCCESS, tixiRegisterNamespace(inDocumentHandle, "http://www.dlr.de/cpacs", "cpacs"));

  ASSERT_EQ(SUCCESS, tixiCheckElement(inDocumentHandle, "/root/cpacs:aircraft"));

  ASSERT_EQ(SUCCESS, tixiGetTextElement(inDocumentHandle, "/root/cpacs:aircraft/cpacs:modelname/", &text));
  ASSERT_STREQ("D150", text);
}

TEST_F(Namespaces, register_document_ns)
{
  char* text = NULL;

  // xpath not registered, html prefix not known
  ASSERT_EQ(INVALID_XPATH, tixiGetTextElement(inDocumentHandle, "/root/html:table/html:tr/html:td[1]/", &text));
  ASSERT_EQ(INVALID_XPATH, tixiGetTextElement(inDocumentHandle, "/root/h:table/h:tr/h:td[1]/", &text));

  ASSERT_EQ(SUCCESS, tixiRegisterNamespacesFromDocument(inDocumentHandle));

  ASSERT_EQ(SUCCESS, tixiGetTextElement(inDocumentHandle, "/root/h:table/h:tr/h:td[1]/", &text));
  ASSERT_STREQ("Apples", text);

  ASSERT_EQ(SUCCESS, tixiGetTextElement(inDocumentHandle, "/root/h:table/h:tr/h:td[2]/", &text));
  ASSERT_STREQ("Bananas", text);

  // check invalid prefix
  ASSERT_EQ(INVALID_XPATH, tixiGetTextElement(inDocumentHandle, "/root/html:table/html:tr/html:td[1]/", &text));

  // check and register default cpacs namespace
  ASSERT_EQ(ELEMENT_NOT_FOUND, tixiCheckElement(inDocumentHandle, "/root/aircraft"));
  ASSERT_EQ(INVALID_XPATH, tixiCheckElement(inDocumentHandle, "/root/cpacs:aircraft"));
}
