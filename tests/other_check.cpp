/*
* Copyright (C) 2007-2012 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: add_attribute_check.cpp 176 2012-10-08 21:20:23Z markus.litz $
*
* Version: $Revision: 176 $
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


/**
    @test Tests for helper routines.
*/

static TixiDocumentHandle inDocumentHandle = -1;
static TixiDocumentHandle outDocumentHandle = -1;
static const char* xmlInputFilename = "TestData/in.xml";
static const char* xmlOutputFilename = "TestData/out.xml";
static const char* outRootElementName = "root";

class OtherTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
     ASSERT_TRUE( tixiOpenDocument  ( xmlInputFilename,   &inDocumentHandle)  == SUCCESS);
     ASSERT_TRUE( tixiCreateDocument( outRootElementName, &outDocumentHandle) == SUCCESS);
  }

  virtual void TearDown() {
      ASSERT_TRUE ( tixiCloseDocument( inDocumentHandle ) == SUCCESS );
      ASSERT_TRUE ( tixiSaveDocument( outDocumentHandle, xmlOutputFilename) == SUCCESS );
      ASSERT_TRUE ( tixiCloseDocument( outDocumentHandle  ) == SUCCESS );
  }
};

TEST_F(OtherTests, childCount_hasNoChild)
{
    const char* elementPath = "/plane/name";
    const char* childName = "this_child_does_not_exist";
    int count;

    ASSERT_TRUE( tixiGetNamedChildrenCount( inDocumentHandle, elementPath, childName, &count ) == SUCCESS);
    ASSERT_TRUE( count  == 0 );
}

TEST_F(OtherTests, checkDocumentHandle)
{
    ASSERT_TRUE( tixiCheckDocumentHandle( inDocumentHandle ) == SUCCESS );
    ASSERT_TRUE( tixiCheckDocumentHandle( -1 ) == INVALID_HANDLE );
}

TEST_F(OtherTests, childCount_childDoesNotExist)
{
    const char* elementPath = "/plane/wings";
    const char* childName = "this_child_does_not_exist";
    int count;

    ASSERT_TRUE( tixiGetNamedChildrenCount( inDocumentHandle, elementPath, childName, &count ) == SUCCESS);
    ASSERT_TRUE( count == 0);
}

TEST_F(OtherTests, checkElement_success)
{
    const char* elementPath1 = "/plane/wings";
    const char* elementPath2 = "/plane/wings/wing";
    ASSERT_TRUE( tixiCheckElement( inDocumentHandle, elementPath1 ) == SUCCESS);
    ASSERT_TRUE( tixiCheckElement( inDocumentHandle, elementPath2 ) == SUCCESS);
}

TEST_F(OtherTests, checkElement_failed)
{
    const char* elementPath1 = "/plane/wingss";
    const char* elementPath2 = "/plane/wings/wing[]";
    ASSERT_TRUE( tixiCheckElement( inDocumentHandle, elementPath1 ) == ELEMENT_NOT_FOUND);
    ASSERT_TRUE( tixiCheckElement( inDocumentHandle, elementPath2 ) == INVALID_XPATH);
}

TEST_F(OtherTests, childCount_oneChild)
{
    const char* elementPath = "/plane";
    const char* childName = "wings";
    int count;

    ASSERT_TRUE( tixiGetNamedChildrenCount( inDocumentHandle, elementPath, childName, &count ) == SUCCESS );
    ASSERT_TRUE( count == 1 );
}

TEST_F(OtherTests, childCount_twoChildren)
{
    const char* elementPath = "/plane/wings";
    const char* childName = "wing";
    int count;

    ASSERT_TRUE( tixiGetNamedChildrenCount( inDocumentHandle, elementPath, childName, &count ) == SUCCESS );
    ASSERT_TRUE( count == 2 );
}

TEST_F(OtherTests, addExternalLink_withAttribute)
{
    const char* parentPath = "/root";
    ASSERT_TRUE( tixiAddExternalLink( outDocumentHandle, parentPath, "TestData/externalFile1", "MyFormat" ) == SUCCESS );
}

TEST_F(OtherTests, addExternalLink_withoutAttribute)
{
    const char* parentPath = "/root";
    ASSERT_TRUE( tixiAddExternalLink( outDocumentHandle, parentPath, "TestData/externalFile2", NULL ) == SUCCESS );
}

TEST_F(OtherTests, getMatrixSize)
{
    const char* matrixPath = "/plane/myMatrix";
    const char* rowName = "blub";
    const char* columnName = "bla";
    int nRows = -1;
    int nColumns = -1;
    const char* pathInvalidMatrix1 = "/plane/invalidMatrix1";
    const char* pathInvalidMatrix2 = "/plane/invalidMatrix2";

    ASSERT_TRUE(tixiGetMatrixSize(inDocumentHandle, matrixPath, rowName, columnName, &nRows, &nColumns) == SUCCESS);
    ASSERT_TRUE(nRows == 2 && nColumns == 2);
    ASSERT_TRUE(tixiGetMatrixSize(inDocumentHandle, matrixPath, "gargel", columnName, &nRows, &nColumns) == NON_MATCHING_NAME);
    ASSERT_TRUE(tixiGetMatrixSize(inDocumentHandle, matrixPath, rowName, "gargel", &nRows, &nColumns) == NON_MATCHING_NAME);
    ASSERT_TRUE(tixiGetMatrixSize(inDocumentHandle, pathInvalidMatrix1, rowName, columnName, &nRows, &nColumns) == NON_MATCHING_NAME);
    ASSERT_TRUE(tixiGetMatrixSize(inDocumentHandle, pathInvalidMatrix2, rowName, columnName, &nRows, &nColumns) == NON_MATCHING_SIZE);
}

TEST_F(OtherTests, createMatrix)
{
    const char* parentPath = "/root";

    ASSERT_TRUE(tixiCreateMatrix(outDocumentHandle, parentPath, "fourbyfour", "row", "column", 4, 4) == SUCCESS);
    ASSERT_TRUE(tixiCreateMatrix(outDocumentHandle, parentPath, "fourbyfour", "row", "column", 4, 0) == MATRIX_DIMENSION_ERROR);
    ASSERT_TRUE(tixiCreateMatrix(outDocumentHandle, parentPath, "fourbyfour", "row", "column", 0, 1) == MATRIX_DIMENSION_ERROR);
    ASSERT_TRUE(tixiCreateMatrix(outDocumentHandle, parentPath, "threebyone", NULL, NULL, 3, 1) == SUCCESS);
    ASSERT_TRUE(tixiCreateMatrix(outDocumentHandle, parentPath, "onebyfour", "row", "column", 1, 4) == SUCCESS);
    ASSERT_TRUE(tixiCreateMatrix(outDocumentHandle, parentPath, "fourby fourCol", "row", "column", 4, 4) == INVALID_XML_NAME);
}

TEST(VersionTests, get_Version_notNull) {
	char *version = tixiGetVersion();
	ASSERT_TRUE(version != NULL);
	ASSERT_GE(atof(version), 0.1);
	ASSERT_LT(atof(version), 100.0);
}


