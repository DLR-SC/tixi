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

TEST(createDocument, noElementName)
{
  char* rootElementName = NULL;
  TixiDocumentHandle handle = -1;

  ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == NO_ELEMENT_NAME );
}

TEST(createDocument, invalidRootNodeName)
{
  const char* rootElementName = "1";
  TixiDocumentHandle handle = -1;

  ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == INVALID_XML_NAME );
}

TEST(createDocument, createDocumentSuccess)
{
  const char* rootElementName = "rootElement";
  TixiDocumentHandle handle = -1;

  ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == SUCCESS );
  ASSERT_TRUE( handle != -1 );
  if ( handle != -1 ) {
    ASSERT_TRUE( tixiSaveDocument( handle, "createDocument_success.xml" )== SUCCESS );
    ASSERT_TRUE( tixiCloseDocument( handle )== SUCCESS );
  }
}

TEST(createDocument, createDocumentAddHeader)
{
  const char* rootElementName = "rootElement";
  TixiDocumentHandle handle = -1;

  ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == SUCCESS );

  if ( handle != -1 ) {
    ASSERT_TRUE( tixiAddHeader( handle, "ToolName", "4.7.11", "Rudi Ratlos" ) == SUCCESS );
    ASSERT_TRUE( tixiSaveDocument( handle, "documentWithHeader.xml" )== SUCCESS );
    ASSERT_TRUE( tixiCloseDocument( handle )== SUCCESS );

#if __unix__ || unix || __APPLE__
    system( "sed  /timestamp/d documentWithHeader.xml >tmp.xml");
    ASSERT_TRUE( system("diff -w tmp.xml TestData/documentWithHeaderReference.xml" ) == 0 );
#endif /* __unix__ */
  }
}

TEST(createDocument, createDocumentAddCPACSHeader)
{
  const char* rootElementName = "rootElement";
  const char* cpacsName = "cpacsName";
  const char* cpacsCreator = "cpacsCreator";
  const char*      version = "cpacsVersion";
  const char* cpacsVersion = "2.0";
  const char* cpacsDescription = "cpacsDescription";
  TixiDocumentHandle handle = -1;
  char* text = NULL;

  ASSERT_TRUE( tixiCreateDocument( rootElementName, &handle ) == SUCCESS );
  ASSERT_TRUE( tixiAddCpacsHeader( handle, cpacsName, cpacsCreator, version, cpacsDescription, cpacsVersion ) == SUCCESS );

  // Now check the different header values
  ASSERT_TRUE( tixiGetTextElement( handle, "/rootElement/header/name", &text ) == SUCCESS );
  ASSERT_TRUE( !strcmp(text, cpacsName));

  ASSERT_TRUE( tixiGetTextElement( handle, "/rootElement/header/creator", &text ) == SUCCESS );
  ASSERT_TRUE( !strcmp(text, cpacsCreator));

  ASSERT_TRUE( tixiGetTextElement( handle, "/rootElement/header/version", &text ) == SUCCESS );
  ASSERT_TRUE( !strcmp(text, version));

  ASSERT_TRUE( tixiGetTextElement( handle, "/rootElement/header/cpacsVersion", &text ) == SUCCESS );
  ASSERT_TRUE( !strcmp(text, cpacsVersion));

  ASSERT_TRUE( tixiGetTextElement( handle, "/rootElement/header/description", &text ) == SUCCESS );
  ASSERT_TRUE( !strcmp(text, cpacsDescription));
}

