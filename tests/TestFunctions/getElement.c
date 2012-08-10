/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id$ 
*
* Version: $Revision$
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
#include <stdio.h>
#include "CUnit/CUnit.h"
#include "tixi.h"
/** 
    @test tixiGetTextElement    
*/   

static TixiDocumentHandle documentHandle = -1;
int preGetElement( void ) {
	
	char* xmlFilename = "TestData/in.xml";

	CU_ASSERT( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS );

	return 0;
}

int postGetElement( void ) {

	CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );

	return 0;
}

void  getEmptyElement( void ){

	char* text = NULL;
	char* elementPath = "/plane/empty";
	CU_ASSERT( tixiGetTextElement( documentHandle, elementPath, &text ) == SUCCESS );
	if ( text ) {
		CU_ASSERT_STRING_EQUAL( text, "");
	}
}
void  getTextElement_getText( void ){

	char* text = NULL;
	char* elementPath = "/plane/name";
	CU_ASSERT( tixiGetTextElement( documentHandle, elementPath, &text ) == SUCCESS );
	if ( text ) {
		CU_ASSERT_STRING_EQUAL( text, "Junkers JU 52");
	}
}

void  getTextElement_elementNotFound( void ){

	char* text = NULL;
	char* elementPath = "/plane/name/pp";
	CU_ASSERT( tixiGetTextElement( documentHandle, elementPath, &text ) == ELEMENT_NOT_FOUND );
}

void  getTextElement_invalidHandle( void ){

	char* text = NULL;
	char* elementPath = "/plane/name/pp";
	CU_ASSERT( tixiGetTextElement( -1, elementPath, &text ) == INVALID_HANDLE );
}

void  getTextElement_invalidXPath( void ){

	char* text = NULL;
	char* elementPath = "cc/plane/name/pp";
	CU_ASSERT( tixiGetTextElement( documentHandle, elementPath, &text ) == INVALID_XPATH );
}

void  getTextElement_notUnique( void ){

	char* text = NULL;
	char* elementPath = "/plane/wings/wing";
	CU_ASSERT( tixiGetTextElement( documentHandle, elementPath, &text ) == ELEMENT_PATH_NOT_UNIQUE );
}

void  getTextElement_notAnElement( void ){

	char* text = NULL;
	char* elementPath = "/plane/wings/wing[1]/@position";
	CU_ASSERT( tixiGetTextElement( documentHandle, elementPath, &text ) == NOT_AN_ELEMENT );
}

void  getTextElement_useIndexedPath( void ){

	char* text = NULL;
	char* elementPath = "/plane/wings/wing[1]/centerOfGravity/x";

	CU_ASSERT( tixiGetTextElement( documentHandle, elementPath, &text ) == SUCCESS );
	if ( text ) {
		CU_ASSERT_STRING_EQUAL( text, "30.0");
	}
}

void getDoubleElement( void ) {

	double number = 0.;
	char* elementPath = "/plane/wings/wing[1]/centerOfGravity/x";

	CU_ASSERT( tixiGetDoubleElement( documentHandle, elementPath, &number ) == SUCCESS );
	CU_ASSERT_DOUBLE_EQUAL( number, 30.0, 1.e-10);
}

void getIntegerElement( void ) {

	int number = 0;
	char* elementPath = "/plane/numberOfPassengers";

	CU_ASSERT( tixiGetIntegerElement( documentHandle, elementPath, &number ) == SUCCESS );
	CU_ASSERT( number == 57);
}

void getBooleanElement( void ) {

	int number = 0;
	char* elementPathTrue = "/plane/bool/aBool1";
	char* elementPathFalse = "/plane/bool/aBool2";
	char* elementPathInvalid = "/plane/bool/aBool3";
	char* elementPathTrueB = "/plane/bool/aBool1b";
	char* elementPathFalseB = "/plane/bool/aBool2b";
	char* elementPathInvalidB = "/plane/bool/aBool3b";

	CU_ASSERT( tixiGetBooleanElement( documentHandle, elementPathTrue, &number ) == SUCCESS );
	CU_ASSERT( number == 1);
	CU_ASSERT( tixiGetBooleanElement( documentHandle, elementPathFalse, &number ) == SUCCESS );
	CU_ASSERT( number == 0);
	number = NULL;
	CU_ASSERT( tixiGetBooleanElement( documentHandle, elementPathInvalid, &number ) == FAILED );
	CU_ASSERT( number == NULL);

	CU_ASSERT( tixiGetBooleanElement( documentHandle, elementPathTrueB, &number ) == SUCCESS );
	CU_ASSERT( number == 1);
	CU_ASSERT( tixiGetBooleanElement( documentHandle, elementPathFalseB, &number ) == SUCCESS );
	CU_ASSERT( number == 0);
	number = NULL;
	CU_ASSERT( tixiGetBooleanElement( documentHandle, elementPathInvalidB, &number ) == FAILED );
	CU_ASSERT( number == NULL);
}

void getPointElement( void ) {
  
//  char* parentPath = "/plane/aPoint/point";
//  char* noPointParentPath = "/plane";
  char* notAPointParentPath = "/plane/notAPoint/point";
  char* notAPoint2ParentPath = "/plane/notAPoint2/point";
  char* pointsParentPath = "/plane/points/point[3]";
  char* incompletePoint = "/plane/points/point[4]";
  double x,y,z;
  
  CU_ASSERT( tixiGetPoint( documentHandle, notAPointParentPath, &x, &y, &z ) == NO_POINT_FOUND );   // internally a COORDINATE_NOT_FOUND error
  CU_ASSERT( tixiGetPoint( documentHandle, notAPoint2ParentPath, &x, &y, &z ) == SUCCESS );
  CU_ASSERT( tixiGetPoint( documentHandle, pointsParentPath, &x, &y, &z ) == SUCCESS );
  CU_ASSERT( x == 3.1 && y == 3.2 && z == 3.3 );
  CU_ASSERT( tixiGetPoint( documentHandle, incompletePoint, &x, &y, &z ) == SUCCESS );
  y = -1.0; // init with some "undef" value
  CU_ASSERT( x == 4.1 && y == -1 && z == 4.3 );

}

void getFloatMatrixElement( void ) {

  char* matrixPath = "/plane/myMatrix";
  char* rowName = "blub";
  char* columnName = "bla";
  int nRows = 2;
  int nColumns = 2;
  double array22[2][2];
  char* pathInvalidMatrix1 =  "/plane/invalidMatrix1";
  char* pathInvalidMatrix2 =  "/plane/invalidMatrix2";

  CU_ASSERT( tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, 
				 nRows, nColumns, ROW_WISE, (double*) array22 )
	     == 
	     SUCCESS );
  
  CU_ASSERT( array22[0][0] == 11. &&
	     array22[0][1] == 12. &&
	     array22[1][0] == 21. &&
	     array22[1][1] == 22. );
  
  CU_ASSERT( tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, 
				 nRows, nColumns, COLUMN_WISE, (double*) array22 )
	     == 
	     SUCCESS );
  
  CU_ASSERT( array22[0][0] == 11. &&
	     array22[0][1] == 21. &&
	     array22[1][0] == 12. &&
	     array22[1][1] == 22. );
  
 CU_ASSERT( tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, 
				 nRows, 3, COLUMN_WISE, (double*) array22 )
	     == 
	     NON_MATCHING_SIZE );

 CU_ASSERT( tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, 
				 nRows, 1, COLUMN_WISE, (double*) array22 )
	     == 
	     NON_MATCHING_SIZE );
 CU_ASSERT( tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, 
				 1, nColumns, COLUMN_WISE, (double*) array22 )
	     == 
	     NON_MATCHING_SIZE );
 
 CU_ASSERT( tixiGetFloatMatrix( documentHandle, matrixPath, rowName, columnName, 
				 3, 1, COLUMN_WISE, (double*) array22 )
	     == 
	     NON_MATCHING_SIZE );

 CU_ASSERT( tixiGetFloatMatrix( documentHandle, matrixPath, "gargel", columnName, 
				 nRows, nColumns, COLUMN_WISE, (double*) array22 )
	     == 
	     NON_MATCHING_NAME );

 CU_ASSERT( tixiGetFloatMatrix( documentHandle, matrixPath, rowName, "gargel",
				nRows, nColumns, COLUMN_WISE, (double*) array22 )
	     == 
	     NON_MATCHING_NAME );

 CU_ASSERT( tixiGetFloatMatrix( documentHandle, pathInvalidMatrix1, rowName, columnName, 
				nRows, nColumns, COLUMN_WISE, (double*) array22 )
	     == 
	     NON_MATCHING_NAME );

 CU_ASSERT( tixiGetFloatMatrix( documentHandle, pathInvalidMatrix2, rowName, columnName, 
				nRows, nColumns, COLUMN_WISE, (double*) array22 )
	     == 
	     NON_MATCHING_SIZE );

}

void getElement_getMatrixOfPoints( void ){
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
    CU_ASSERT( tixiGetPoint( documentHandle, entryPath, &x, &y, &z ) == SUCCESS );

    CU_ASSERT( x == i && y == 2.*i && z == 3.*i );
  }

}
