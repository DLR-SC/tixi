/*
* Copyright (C) 2007-2012 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: add_attribute_check.cpp 173 2012-10-08 20:06:49Z markus.litz $
*
* Version: $Revision: 173 $
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
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
    @test Tests for getting attributes.
*/

static TixiDocumentHandle documentHandle = -1;

class GetElementTests : public ::testing::Test {
 protected:
  virtual void SetUp() {
     char* xmlFilename = "TestData/in.xml";

     ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS);
  }

  virtual void TearDown() {
      ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
  }
};

TEST_F(GetElementTests, getEmptyElement)
{
    char* text = NULL;
    char* elementPath = "/plane/empty";
    ASSERT_TRUE( tixiGetTextElement( documentHandle, elementPath, &text ) == SUCCESS );
    if ( text ) {
        ASSERT_TRUE( !strcmp(text, ""));
    }
}

TEST_F(GetElementTests, getElementText)
{
    char* text = NULL;
    char* elementPath = "/plane/name";
    ASSERT_TRUE( tixiGetTextElement( documentHandle, elementPath, &text ) == SUCCESS );
    if ( text ) {
        ASSERT_TRUE( !strcmp(text, "Junkers JU 52"));
    }
}

TEST_F(GetElementTests, elementNotFound)
{
    char* text = NULL;
    char* elementPath = "/plane/name/pp";
    ASSERT_TRUE( tixiGetTextElement( documentHandle, elementPath, &text ) == ELEMENT_NOT_FOUND );
}

TEST_F(GetElementTests, invalidHandle)
{
    char* text = NULL;
    char* elementPath = "/plane/name/pp";
    ASSERT_TRUE( tixiGetTextElement( -1, elementPath, &text ) == INVALID_HANDLE );
}

TEST_F(GetElementTests, invalidXPath)
{
    char* text = NULL;
    char* elementPath = "cc/plane/na|<<me/pp";
    ASSERT_TRUE( tixiGetTextElement( documentHandle, elementPath, &text ) == INVALID_XPATH );
}

TEST_F(GetElementTests, notUnique)
{
    char* text = NULL;
    char* elementPath = "/plane/wings/wing";
    ASSERT_TRUE( tixiGetTextElement( documentHandle, elementPath, &text ) == ELEMENT_PATH_NOT_UNIQUE );
}

TEST_F(GetElementTests, notAnElement)
{
    char* text = NULL;
    char* elementPath = "/plane/wings/wing[1]/@position";
    ASSERT_TRUE( tixiGetTextElement( documentHandle, elementPath, &text ) == NOT_AN_ELEMENT );
}

TEST_F(GetElementTests, usedIndexedPath)
{
    char* text = NULL;
    char* elementPath = "/plane/wings/wing[1]/centerOfGravity/x";

    ASSERT_TRUE( tixiGetTextElement( documentHandle, elementPath, &text ) == SUCCESS );
    if ( text ) {
        ASSERT_TRUE( !strcmp(text, "30.0"));
    }
}

TEST_F(GetElementTests, getDoubleElement)
{
    double number = 0.;
    char* elementPath = "/plane/wings/wing[1]/centerOfGravity/x";

    ASSERT_TRUE( tixiGetDoubleElement( documentHandle, elementPath, &number ) == SUCCESS );
    ASSERT_TRUE( number == 30.0);
}

TEST_F(GetElementTests, getIntegerElement)
{
    int number = 0;
    char* elementPath = "/plane/numberOfPassengers";

    ASSERT_TRUE( tixiGetIntegerElement( documentHandle, elementPath, &number ) == SUCCESS );
    ASSERT_TRUE( number == 57);
}


TEST_F(GetElementTests, getBooleanElement)
{
    int number = 0;
    char* elementPathTrue = "/plane/bool/aBool1";
    char* elementPathFalse = "/plane/bool/aBool2";
    char* elementPathInvalid = "/plane/bool/aBool3";
    char* elementPathTrueB = "/plane/bool/aBool1b";
    char* elementPathFalseB = "/plane/bool/aBool2b";
    char* elementPathInvalidB = "/plane/bool/aBool3b";

    ASSERT_TRUE( tixiGetBooleanElement( documentHandle, elementPathTrue, &number ) == SUCCESS );
    ASSERT_TRUE( number == 1);
    ASSERT_TRUE( tixiGetBooleanElement( documentHandle, elementPathFalse, &number ) == SUCCESS );
    ASSERT_TRUE( number == 0);
    number = 0;
    ASSERT_TRUE( tixiGetBooleanElement( documentHandle, elementPathInvalid, &number ) == FAILED );
    ASSERT_TRUE( number == 0);

    ASSERT_TRUE( tixiGetBooleanElement( documentHandle, elementPathTrueB, &number ) == SUCCESS );
    ASSERT_TRUE( number == 1);
    ASSERT_TRUE( tixiGetBooleanElement( documentHandle, elementPathFalseB, &number ) == SUCCESS );
    ASSERT_TRUE( number == 0);
    number = 0;
    ASSERT_TRUE( tixiGetBooleanElement( documentHandle, elementPathInvalidB, &number ) == FAILED );
    ASSERT_TRUE( number == 0);
}

TEST_F(GetElementTests, getPointElement)
{
    //  char* parentPath = "/plane/aPoint/point";
    //  char* noPointParentPath = "/plane";
    char* notAPointParentPath = "/plane/notAPoint/point";
    char* notAPoint2ParentPath = "/plane/notAPoint2/point";
    char* pointsParentPath = "/plane/points/point[3]";
    char* incompletePoint = "/plane/points/point[4]";
    double x,y,z;

    ASSERT_TRUE( tixiGetPoint( documentHandle, notAPointParentPath, &x, &y, &z ) == NO_POINT_FOUND );   // internally a COORDINATE_NOT_FOUND error
    ASSERT_TRUE( tixiGetPoint( documentHandle, notAPoint2ParentPath, &x, &y, &z ) == SUCCESS );
    ASSERT_TRUE( tixiGetPoint( documentHandle, pointsParentPath, &x, &y, &z ) == SUCCESS );
    ASSERT_TRUE( x == 3.1 && y == 3.2 && z == 3.3 );
    ASSERT_TRUE( tixiGetPoint( documentHandle, incompletePoint, &x, &y, &z ) == SUCCESS );
    y = -1.0; // init with some "undef" value
    ASSERT_TRUE( x == 4.1 && y == -1 && z == 4.3 );
}


TEST_F(GetElementTests, getMatrixElement) {
    char* matrixPath = "/plane/myMatrix";
    char* rowName = "blub";
    char* columnName = "bla";
    int nRows = 2;
    int nColumns = 2;
    double array22[2][2];
    char* pathInvalidMatrix1 = "/plane/invalidMatrix1";
    char* pathInvalidMatrix2 = "/plane/invalidMatrix2";

    ASSERT_TRUE( tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, nRows, nColumns, ROW_WISE, (double*) array22 ) == SUCCESS);

    ASSERT_TRUE( array22[0][0] == 11. && array22[0][1] == 12. && array22[1][0] == 21. && array22[1][1] == 22.);

    ASSERT_TRUE( tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, nRows, nColumns, COLUMN_WISE, (double*) array22 ) == SUCCESS);

    ASSERT_TRUE( array22[0][0] == 11. && array22[0][1] == 21. && array22[1][0] == 12. && array22[1][1] == 22.);

    ASSERT_TRUE(
            tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, nRows, 3, COLUMN_WISE, (double*) array22 ) == NON_MATCHING_SIZE);

    ASSERT_TRUE(
            tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, nRows, 1, COLUMN_WISE, (double*) array22 ) == NON_MATCHING_SIZE);
    ASSERT_TRUE(
            tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, 1, nColumns, COLUMN_WISE, (double*) array22 ) == NON_MATCHING_SIZE);

    ASSERT_TRUE( tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, 3, 1, COLUMN_WISE, (double*) array22 ) == NON_MATCHING_SIZE);

    ASSERT_TRUE(
            tixiGetFloatMatrix( documentHandle, matrixPath, "gargel", columnName, nRows, nColumns, COLUMN_WISE, (double*) array22 ) == NON_MATCHING_NAME);

    ASSERT_TRUE(
            tixiGetFloatMatrix( documentHandle, matrixPath, rowName, "gargel", nRows, nColumns, COLUMN_WISE, (double*) array22 ) == NON_MATCHING_NAME);

    ASSERT_TRUE(
            tixiGetFloatMatrix( documentHandle, pathInvalidMatrix1, rowName, columnName, nRows, nColumns, COLUMN_WISE, (double*) array22 ) == NON_MATCHING_NAME);

    ASSERT_TRUE(
            tixiGetFloatMatrix( documentHandle, pathInvalidMatrix2, rowName, columnName, nRows, nColumns, COLUMN_WISE, (double*) array22 ) == NON_MATCHING_SIZE);
}


TEST_F(GetElementTests, getMatrixOfPoints)
{
    int i;
    int nPoints = 10;
    char entryPath[50];
    int insertPosition = -1;

    strcpy( entryPath, "/plane/matrixOfPoints/r[" );
    insertPosition=strlen(entryPath);

    for ( i = 0; i < nPoints; i++ ) {
      double x;
      double y;
      double z;

      sprintf( &(entryPath[insertPosition]), "%02d]/c[1]/point", i+1 );
      ASSERT_TRUE( tixiGetPoint( documentHandle, entryPath, &x, &y, &z ) == SUCCESS );

      ASSERT_TRUE( x == i && y == 2.*i && z == 3.*i );
    }
}
