/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
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
#include <stdlib.h>

#include "CUnit/CUnit.h"

#include "tixi.h"

/** 
    @test Tests for addElement routines.
*/   

static TixiDocumentHandle documentHandle = -1;

int preAddElement( void ) {

	char* rootElementName = "rootElement";
	if( tixiCreateDocument( rootElementName, &documentHandle ) == SUCCESS ) {
		return 0;
	}
	else {
		return 1;
	}
}

int postAddElement( void ) {
	return 0;
}

void addElement_parentRoot( void ){
	
	char* parentPath = "/rootElement";
	char* elementName = "child";
	char* text = "This is my content.";

	CU_ASSERT( tixiAddTextElement( documentHandle, parentPath, elementName, text ) == SUCCESS );
}
void addElement_addTree( void ){
	
	char* parentPath = "/rootElement";
	char* elementName = "level1";

	CU_ASSERT( tixiAddTextElement( documentHandle, parentPath, elementName,  NULL ) == SUCCESS );

	CU_ASSERT( tixiAddTextElement( documentHandle, "/rootElement/level1", "level2", "level2" ) == SUCCESS );
	CU_ASSERT( tixiAddTextElement( documentHandle, "/rootElement/level1", "level2", "level2" ) == SUCCESS );
}
void addElement_addElementAndParents( void ){
	
	char* parentPath = "/rootElement/newParent";
	char* elementName = "withParent";

	CU_ASSERT( tixiAddTextElement( documentHandle, parentPath, elementName,  NULL ) == SUCCESS );

}

void addElement_invalidName( void ){
	
	char* parentPath = "/parents/parent";
	char* elementName = "4711";
	char* text = "This is my content.";

	CU_ASSERT( tixiAddTextElement( documentHandle, parentPath, elementName, text ) == INVALID_XML_NAME );
}
void addElement_parentNotUnique( void ){
	
	char* parentPath = "/rootElement/level1/level2";
	char* elementName = "i_am_a_child";
	char* text = "This is my content.";

	CU_ASSERT( tixiAddTextElement( documentHandle, parentPath, elementName, text ) == ELEMENT_PATH_NOT_UNIQUE );
}

void addElement_parentNotFound( void ){

	char* parentPath = "/this_is_not_a_parent";
	char* elementName = "i_am_a_child";
	char* text = "This is my content.";

	CU_ASSERT( tixiAddTextElement( documentHandle, parentPath, elementName, text ) == ELEMENT_NOT_FOUND );
}

void addElement_alreadySaved( void ){

	char* parentPath = "/parent";
	char* elementName = "i_am_a_child";
	char* text = "This is my content.";

	CU_ASSERT( tixiSaveDocument( documentHandle, "addElement.xml" ) == SUCCESS );
	CU_ASSERT( tixiAddTextElement( documentHandle, parentPath, elementName, text ) == ALREADY_SAVED );
}
void addElement_addDouble( void ) {
	char* parentPath = "/rootElement";
	double number = 3.14159265;
	char* format = "%12.8f";
	CU_ASSERT( tixiAddDoubleElement( documentHandle, parentPath, "double", number, format ) == SUCCESS );
	CU_ASSERT( tixiAddDoubleElement( documentHandle, parentPath, "double_g", number, NULL ) == SUCCESS );
}

void addElement_addInteger( void ) {
	char* parentPath = "/rootElement";
	int number = 4711;
	char* format = "%5d";
	CU_ASSERT( tixiAddIntegerElement( documentHandle, parentPath, "integer", number, format ) == SUCCESS );
	CU_ASSERT( tixiAddIntegerElement( documentHandle, parentPath, "integer_d", number, NULL ) == SUCCESS );
}

void addElement_addBoolean( void ) {
	char* parentPath = "/rootElement";
	int val_true = 1;
	int val_false = 0;
	CU_ASSERT( tixiAddBooleanElement( documentHandle, parentPath, "boolean_t", val_true ) == SUCCESS );
	CU_ASSERT( tixiAddBooleanElement( documentHandle, parentPath, "boolean_f", val_false ) == SUCCESS );	
	CU_ASSERT( tixiAddBooleanElement( documentHandle, parentPath, "boolean_x", '2' ) == FAILED );
}

void addElement_addPoint( void ){
  char* parentPath = "/rootElement";
  double x = 1.;
  double y = 2.;
  double z = 3.14159265;
  
  char* format = "%10.3f";
  
  CU_ASSERT( tixiAddPoint( documentHandle, parentPath, x, y, z, format ) == SUCCESS );
  CU_ASSERT( tixiAddPoint( documentHandle, parentPath, x, y, z, NULL ) == SUCCESS );
  
}

void addElement_addMatrix( void ){
  char* parentPath = "/rootElement";
  double array[16] = { 1., 2., 3., 4., 5., 6., 7., 8., 
		       9., 10., 11., 12., 13., 14., 15., 16. };
  
  char* format = "%10.3f";
  CU_ASSERT( 
	    tixiAddFloatMatrix( documentHandle, parentPath, "fourbyfour", "row", "column",
				4, 4, ROW_WISE, array, format )
	    == 
	    SUCCESS 
	   );

  CU_ASSERT( 
	    tixiAddFloatMatrix( documentHandle, parentPath, "fourbyfour", NULL, NULL,
				3, 3, ROW_WISE, array, format )
	    == 
	    SUCCESS 
	   );

   CU_ASSERT( 
	     tixiAddFloatMatrix( documentHandle, parentPath, "fourbyfourCol", "row", "column",
				4, 4, COLUMN_WISE, array, "%5.1f" )
	     == 
	     SUCCESS 
	    );

  CU_ASSERT( 
	     tixiAddFloatMatrix( documentHandle, parentPath, "fourby fourCol", "row", "column",
				4, 4, COLUMN_WISE, array, "%5.1f" )
	     == 
	     INVALID_XML_NAME 
	    );


}

void addElement_addMatrixOfPoints( void ){
  char* parentPath = "/rootElement";
  int i;
  int nPoints = 10;
  int nRows = nPoints;
  int nColumns = 1;
  char entryPath[50];
  size_t insertPosition = -1;

  strcpy( entryPath, "/rootElement/matrixOfPoints/r[" );
  insertPosition=strlen(entryPath);
  
  CU_ASSERT( 
	    tixiCreateMatrix( documentHandle, parentPath, "matrixOfPoints", NULL, NULL,
				nRows, nColumns )
	    == 
	    SUCCESS 
	    );
  
  
  for ( i = 0; i < nPoints; i++ ) {
    double x = i;
    double y = 2. * i;
    double z = 3. * i;

    sprintf( &(entryPath[insertPosition]), "%02d]/c[1]", i+1 );
    CU_ASSERT( tixiAddPoint( documentHandle, entryPath, x, y, z, NULL ) == SUCCESS );
  }
}
#if __unix__ || unix
void addElement_compareResult( void ) {

	CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );
	CU_ASSERT( system("diff -w addElement.xml TestData/addElementReference.xml" ) == 0 );
	
}

#endif /* __unix__ */
