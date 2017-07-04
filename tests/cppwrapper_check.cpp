/*
* Copyright (C) 2017 German Aerospace Center (DLR/SC)
*
* Created: 2017-06-30 Jan Kkleinert <Jan.Kleinert@dlr.de>
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
#include "tixicpp.h"

static TixiDocumentHandle documentHandle = -1;

TEST(CppWrapper, createDocumentInvalidRootNodeName)
{
    std::string rootElementName = "1";

    try {
        documentHandle =  tixi3::TixiCreateDocument( rootElementName );
        FAIL() << "Expected tixi3::helper::TixiError";
    }
    catch(tixi3::helper::TixiError const & err) {
        ASSERT_TRUE(err.returnCode()==INVALID_XML_NAME);
    }
}

TEST(CppWrapper, createDocumentAddCPACSHeader)
{
  std::string rootElementName = "rootElement";
  std::string cpacsName = "cpacsName";
  std::string cpacsCreator = "cpacsCreator";
  std::string      version = "cpacsVersion";
  std::string cpacsVersion = "2.0";
  std::string cpacsDescription = "cpacsDescription";
  std::string text;

  ASSERT_NO_THROW( documentHandle = tixi3::TixiCreateDocument( rootElementName ) );
  ASSERT_NO_THROW( tixi3::TixiAddCpacsHeader( documentHandle, cpacsName, cpacsCreator, version, cpacsDescription, cpacsVersion ) );

  // Now check the different header values
  ASSERT_NO_THROW( text = tixi3::TixiGetTextElement( documentHandle, "/rootElement/header/name" ) );
  EXPECT_EQ(text, cpacsName);

  ASSERT_NO_THROW( text = tixi3::TixiGetTextElement( documentHandle, "/rootElement/header/creator") );
  EXPECT_EQ(text, cpacsCreator);

  ASSERT_NO_THROW( text = tixi3::TixiGetTextElement( documentHandle, "/rootElement/header/version") );
  EXPECT_EQ(text, version);

  ASSERT_NO_THROW( text = tixi3::TixiGetTextElement( documentHandle, "/rootElement/header/cpacsVersion") );
  EXPECT_EQ(text, cpacsVersion);

  ASSERT_NO_THROW( text = tixi3::TixiGetTextElement( documentHandle, "/rootElement/header/description") );
  EXPECT_EQ(text, cpacsDescription);
}

TEST(CppWrapper,getAttribute)
{
    std::string xmlFilename = "TestData/in.xml";
    ASSERT_NO_THROW( documentHandle=tixi3::TixiOpenDocument(xmlFilename) );

    std::string text;
    std::string elementPath = "plane/name";
    std::string attributeName = "non_existing_attribute";

    try{
        text = tixi3::TixiGetTextAttribute(-1,elementPath,attributeName);
    }
    catch(tixi3::helper::TixiError const & err) {
        ASSERT_TRUE(err.returnCode()==INVALID_HANDLE);
    }

    elementPath = "/plane/wings/wing[1]";
    attributeName = "non_existing_attribute";
    try{
        text = tixi3::TixiGetTextAttribute(documentHandle,elementPath,attributeName);
    }
    catch(tixi3::helper::TixiError const & err) {
        ASSERT_TRUE(err.returnCode()==ATTRIBUTE_NOT_FOUND);
    }
}
