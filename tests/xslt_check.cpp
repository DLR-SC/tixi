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

static const char* xmlFilename = "TestData/valid_CPACS_dokumentiert.xml";
static const char* stylesheetFilename = "TestData/mappingInputRaw.xsl";
static const char* resultFileName = "TestData/result.xml";

class XsltChecks : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    documentHandle = -1;
    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS );
  }

  virtual void TearDown()
  {
    tixiCloseDocument(documentHandle);
    documentHandle  = -1;
  }

  TixiDocumentHandle documentHandle;
};

TEST_F(XsltChecks, xslTransformationToString)
{
  char* string;
  
  tixiXSLTransformationToString(documentHandle, stylesheetFilename, &string);
  
  ASSERT_STRNE("", string);
}

TEST_F(XsltChecks, xslTransformationToString_invalidStylesheet)
{
  char* string;

  tixiXSLTransformationToString(documentHandle, "", &string);
  ASSERT_EQ(NULL, string);
}

TEST_F(XsltChecks, xslTransformationToFile)
{
  ASSERT_TRUE( tixiXSLTransformationToFile( documentHandle, stylesheetFilename, resultFileName ) == SUCCESS );

  /* finish */
  ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
}

TEST_F(XsltChecks, xslTransformationToFile_invalidStylesheet)
{
  ReturnCode ret = tixiXSLTransformationToFile(documentHandle, "", resultFileName);
  ASSERT_EQ(FAILED, ret);
}

TEST_F(XsltChecks, xslTransformationToFile_invalidOutfile)
{
  ReturnCode ret = tixiXSLTransformationToFile(documentHandle, stylesheetFilename, NULL);
  ASSERT_EQ(FAILED, ret);

  ret = tixiXSLTransformationToFile(documentHandle, NULL, NULL);
  ASSERT_EQ(FAILED, ret);
}

