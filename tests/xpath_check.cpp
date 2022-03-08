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
#include "tixiInternal.h"
#include "xpathFunctions.h"


static const char* xPathXmlFilename = "TestData/xpathsEvaluate.xml";

static const char* elementPathExists = "/root/a[@uID=\"test\"]/b[@uID=\"b\"]";
static const char* elementPathDoesnt = "/root/a[@uID=\"schlumpf\"]/b[@uID=\"empty_element\"]";
static const char* elementPathNoExists = "/root/a[@uID=\"schlumpf\"]/b[@uID=\"invalid_uid\"]";

class XPathChecks : public ::testing::Test
{
protected:
  void SetUp()
  {
    documentHandle = -1;
    ASSERT_TRUE( tixiOpenDocument( xPathXmlFilename, &documentHandle ) == SUCCESS );
  }

  void TearDown()
  {
    ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
    documentHandle = -1;
  }

  TixiDocumentHandle documentHandle;
};


TEST_F(XPathChecks, tixiXPathEvaluateNodeNumber)
{
  int num = 0;

  ASSERT_TRUE( tixiXPathEvaluateNodeNumber( documentHandle, "//@uID", &num) == SUCCESS );
  ASSERT_TRUE( num == 6 );
}

TEST_F(XPathChecks, tixiXPathEvaluateNodeNumber_noNodes)
{
  int num = 0;
  ASSERT_TRUE( tixiXPathEvaluateNodeNumber( documentHandle, "//@kruzifix", &num) == SUCCESS);
  EXPECT_EQ(0, num);
}


TEST_F(XPathChecks, tixiXPathExpressionGetTextByIndex_char)
{
  char* text = NULL;

  ASSERT_TRUE( tixiXPathExpressionGetTextByIndex( documentHandle, "//@uID", 4, &text) == SUCCESS );
  ASSERT_TRUE( !strcmp(text, "schlumpf"));
}

TEST_F(XPathChecks, XPathEvaluateExpression)
{
  TixiDocument* document = getDocument(documentHandle);

  xmlXPathObjectPtr res = XPathEvaluateExpression(document, elementPathExists);
  ASSERT_TRUE(res != NULL);
}

TEST_F(XPathChecks, XPathEvaluateExpression_invalidContext)
{
  xmlXPathObjectPtr res = XPathEvaluateExpression(NULL, elementPathExists);
  ASSERT_TRUE(res == NULL);
}

TEST_F(XPathChecks, XPathEvaluateExpression_invalidXPath)
{
  TixiDocument* document = getDocument(documentHandle);
  xmlXPathObjectPtr res = XPathEvaluateExpression(document, "\\");
  ASSERT_TRUE(res == NULL);
}

TEST_F(XPathChecks, getText)
{
  TixiDocument* document = getDocument(documentHandle);
  char* text = XPathExpressionGetText(document, elementPathExists, 1);
  ASSERT_STREQ("Blupp", text);
  xmlFree(text);
}
TEST_F(XPathChecks, getText_invalidIndex)
{
  TixiDocument* document = getDocument(documentHandle);
  char* text = XPathExpressionGetText(document, elementPathExists , 3);
  ASSERT_EQ(NULL, text);

  text = XPathExpressionGetText(document, elementPathExists, 0);
  ASSERT_EQ(NULL, text);
}

TEST_F(XPathChecks, getElementName)
{
  TixiDocument* document = getDocument(documentHandle);
  char* name = XPathExpressionGetElementName(document, elementPathExists, 1);
  ASSERT_STREQ("b", name);
}

TEST_F(XPathChecks, getElementName_invalidIndex)
{
  TixiDocument* document = getDocument(documentHandle);
  char* name = XPathExpressionGetElementName(document, elementPathExists, 3);
  ASSERT_EQ(NULL, name);

  name = XPathExpressionGetElementName(document, elementPathExists, 0);
  ASSERT_EQ(NULL, name);
}

TEST_F(XPathChecks, getElementName_noElement)
{
  TixiDocument* document = getDocument(documentHandle);
  char* name = XPathExpressionGetElementName(document, elementPathNoExists, 1);
  ASSERT_EQ(NULL, name);
}

TEST_F(XPathChecks, getElementName_selectAttribute)
{
  TixiDocument* document = getDocument(documentHandle);
  char* name = XPathExpressionGetElementName(document, "/root/a[1]/@uID", 1);
  ASSERT_EQ(NULL, name);
}

TEST_F(XPathChecks, getParentNodeToXPath)
{
  xmlNodePtr node = getParentNodeToXPath(documentHandle, "/root/a[1]/@uID");
  ASSERT_TRUE(node != NULL);
}

TEST_F(XPathChecks, getParentNodeToXPath_invalidHandle)
{
  xmlNodePtr node = getParentNodeToXPath(-1, "/root/a[1]/@uID");
  ASSERT_TRUE(node == NULL);
}

TEST_F(XPathChecks, getParentNodeToXPath_pathNotUnique)
{
  xmlNodePtr node = getParentNodeToXPath(documentHandle, "/root/a/@uID");
  ASSERT_TRUE(node == NULL);
}

TEST_F(XPathChecks, getParentNodeToXPath_invalidXPath)
{
  xmlNodePtr node = getParentNodeToXPath(documentHandle, "/root/a/@\\uID");
  ASSERT_TRUE(node == NULL);

  node = getParentNodeToXPath(documentHandle, "/invalidroot/a/@uID");
  ASSERT_TRUE(node == NULL);
}

TEST_F(XPathChecks, generateXPathFromNodePtr)
{
  xmlNodePtr node = getParentNodeToXPath(documentHandle, "/root/a[1]/@uID");
  ASSERT_TRUE(node != NULL);

  char* xpath = generateXPathFromNodePtr(documentHandle, node);
  ASSERT_STREQ("/root/a[@uID=\"test\"]", xpath);
}

TEST_F(XPathChecks, generateXPathFromNodePtr_invalidHandle)
{
  char* xpath = generateXPathFromNodePtr(-1, NULL);
  ASSERT_TRUE(xpath == NULL);
}

TEST_F(XPathChecks, generateXPathFromNodePtr_invalidNode)
{
  char* xpath = generateXPathFromNodePtr(documentHandle, NULL);
  ASSERT_STREQ("", xpath);
}

TEST_F(XPathChecks, xPathExpressionGetElementPath)
{
  char* xpath = NULL;
  int number = 0;
  TixiDocument* document = getDocument(documentHandle);

  tixiXPathEvaluateNodeNumber(documentHandle, "//a", &number);
  ASSERT_EQ(2, number);
  xpath = XPathExpressionGetElementPath(document, "//a", 1);
  ASSERT_STREQ("/root/a[1]", xpath);
  free(xpath);
  xpath = XPathExpressionGetElementPath(document, "//a", 2);
  ASSERT_STREQ("/root/a[2]", xpath);
  free(xpath);
}

// same test but api
TEST_F(XPathChecks, tixiXPathExpressionGetXPath)
{
  char* xpath = NULL;
  int number = 0;

  tixiXPathEvaluateNodeNumber(documentHandle, "//a", &number);
  ASSERT_EQ(2, number);

  ASSERT_EQ(SUCCESS, tixiXPathExpressionGetXPath(documentHandle, "//a", 1, &xpath));
  ASSERT_STREQ("/root/a[1]", xpath);

  ASSERT_EQ(SUCCESS, tixiXPathExpressionGetXPath(documentHandle, "//a", 2, &xpath));
  ASSERT_STREQ("/root/a[2]", xpath);

  // invalid indices
  ASSERT_EQ(FAILED, tixiXPathExpressionGetXPath(documentHandle, "//a", 3, &xpath));
  ASSERT_EQ(FAILED, tixiXPathExpressionGetXPath(documentHandle, "//a", 0, &xpath));
  ASSERT_EQ(FAILED, tixiXPathExpressionGetXPath(documentHandle, "//c", 1, &xpath));

  // invalid handle
  ASSERT_EQ(INVALID_HANDLE, tixiXPathExpressionGetXPath(-1, "//a", 1, &xpath));
}

