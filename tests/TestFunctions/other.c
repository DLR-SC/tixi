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
#include <assert.h>
#include "CUnit/CUnit.h"
#include "tixi.h"

/** 
    @test other helper functions
*/
static TixiDocumentHandle inDocumentHandle = -1;
static TixiDocumentHandle outDocumentHandle = -1;
static char* xmlInputFilename = "TestData/in.xml";
static char* xmlOutputFilename = "TestData/out.xml";
static char* outRootElementName = "root";

int preOtherFunctions( void ) {
	
	
  assert( tixiOpenDocument( xmlInputFilename, &inDocumentHandle ) == SUCCESS );
  assert( tixiCreateDocument( outRootElementName, &outDocumentHandle ) == SUCCESS );
  return 0;
}

int postOtherFunctions( void ) {

  assert( tixiCloseDocument( inDocumentHandle ) == SUCCESS );
  assert( tixiSaveDocument( outDocumentHandle, xmlOutputFilename) == SUCCESS );
  assert( tixiCloseDocument( outDocumentHandle  ) == SUCCESS );
  return 0;
}

void childCount_hasNoChild(void){

  char* elementPath = "/plane/name";
  char* childName = "this_child_does_not_exist";
  int count;

  CU_ASSERT( tixiGetNamedChildrenCount( inDocumentHandle, elementPath, childName, &count ) == SUCCESS);	
  CU_ASSERT( count  == 0 );
}


void checkDocumentHandle( void ) {
  CU_ASSERT( tixiCheckDocumentHandle( inDocumentHandle ) == SUCCESS );
  CU_ASSERT( tixiCheckDocumentHandle( -1 ) == INVALID_HANDLE );
}

void childCount_childDoesNotExist(void ){
  char* elementPath = "/plane/wings";
  char* childName = "this_child_does_not_exist";
  int count;
  
  CU_ASSERT( tixiGetNamedChildrenCount( inDocumentHandle, elementPath, childName, &count ) == SUCCESS);
  CU_ASSERT( count == 0);
}


void checkElement_success(void) {
  char* elementPath1 = "/plane/wings";
  char* elementPath2 = "/plane/wings/wing";
  CU_ASSERT( tixiCheckElement( inDocumentHandle, elementPath1 ) == SUCCESS);
  CU_ASSERT( tixiCheckElement( inDocumentHandle, elementPath2 ) == SUCCESS);
}

void checkElement_failed(void) {
  char* elementPath1 = "/plane/wingss";
  char* elementPath2 = "/plane/wings/wing[]";
  CU_ASSERT( tixiCheckElement( inDocumentHandle, elementPath1 ) == ELEMENT_NOT_FOUND);
  CU_ASSERT( tixiCheckElement( inDocumentHandle, elementPath2 ) == INVALID_XPATH);
}

void childCount_oneChild(void ){
	char* elementPath = "/plane";
	char* childName = "wings";
	int count;

	CU_ASSERT( tixiGetNamedChildrenCount( inDocumentHandle, elementPath, childName, &count ) == SUCCESS );
	CU_ASSERT( count == 1 );
}
void childCount_twoChildren(void ){
	char* elementPath = "/plane/wings";
	char* childName = "wing";
	int count;

    CU_ASSERT( tixiGetNamedChildrenCount( inDocumentHandle, elementPath, childName, &count ) == SUCCESS );
    CU_ASSERT( count == 2 );
}

void addExternalLink_withAttribute( void ) {

    char* parentPath = "/root";
    CU_ASSERT( tixiAddExternalLink( outDocumentHandle, parentPath, "TestData/externalFile1", "MyFormat" ) == SUCCESS );
}

void addExternalLink_withoutAttribute( void ) {
    char* parentPath = "/root";
    CU_ASSERT( tixiAddExternalLink( outDocumentHandle, parentPath, "TestData/externalFile2", NULL ) == SUCCESS );
}


void getMatrixSize( void ) {

  char* matrixPath = "/plane/myMatrix";
  char* rowName = "blub";
  char* columnName = "bla";
  int nRows = -1;
  int nColumns = -1;
  char* pathInvalidMatrix1 =  "/plane/invalidMatrix1";
  char* pathInvalidMatrix2 =  "/plane/invalidMatrix2";

  CU_ASSERT( tixiGetMatrixSize( inDocumentHandle, matrixPath, rowName, columnName, 
				&nRows, &nColumns)
	     == 
	     SUCCESS );

  CU_ASSERT( nRows == 2 && nColumns == 2 );
  

 CU_ASSERT( tixiGetMatrixSize( inDocumentHandle, matrixPath, "gargel", columnName, 
			       &nRows, &nColumns)
	     == 
	     NON_MATCHING_NAME );

 CU_ASSERT( tixiGetMatrixSize( inDocumentHandle, matrixPath, rowName, "gargel",
			       &nRows, &nColumns)
	     == 
	     NON_MATCHING_NAME );

 CU_ASSERT( tixiGetMatrixSize( inDocumentHandle, pathInvalidMatrix1, rowName, columnName, 
			       &nRows, &nColumns)
	     == 
	     NON_MATCHING_NAME );

 CU_ASSERT( tixiGetMatrixSize( inDocumentHandle, pathInvalidMatrix2, rowName, columnName, 
			       &nRows, &nColumns)
	    == 
	    NON_MATCHING_SIZE );


}
void createMatrix( void ){

  char* parentPath = "/root";

  CU_ASSERT( 
	    tixiCreateMatrix( outDocumentHandle, parentPath, "fourbyfour", "row", "column",
				4, 4 )
	    == 
	    SUCCESS 
	   );
  
  CU_ASSERT( 
	    tixiCreateMatrix( outDocumentHandle, parentPath, "fourbyfour", "row", "column",
				4, 0 )
	    == 
	    MATRIX_DIMENSION_ERROR 
	    );

  CU_ASSERT( 
	    tixiCreateMatrix( outDocumentHandle, parentPath, "fourbyfour", "row", "column",
				0, 1 )
	    == 
	    MATRIX_DIMENSION_ERROR
	   );

  CU_ASSERT( 
	    tixiCreateMatrix( outDocumentHandle, parentPath, "threebyone", NULL, NULL,
				3, 1 )
	    == 
	    SUCCESS 
	   );

   CU_ASSERT( 
	     tixiCreateMatrix( outDocumentHandle, parentPath, "onebyfour", "row", "column",
				 1, 4 )
	     == 
	     SUCCESS 
	    );

  CU_ASSERT( 
	     tixiCreateMatrix( outDocumentHandle, parentPath, "fourby fourCol", "row", "column",
				4, 4 )
	     == 
	     INVALID_XML_NAME 
	    );


}
