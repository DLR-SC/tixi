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
#include <stdlib.h>
#include <assert.h>
#include "CUnit/CUnit.h"
#include "tixi.h"
#include "fortrantests.h"
#include "../testfunctions.h"
/** 
    @test Unit test for the fortran interface
*/   
char* makeCString( char *fortranString, int fortranStringLength );

#if FORTRAN

void fTest_OpenAndCloseDocument_success( void ) {

  int error;
  ftest_open_document_success( &error );
  CU_ASSERT( error == SUCCESS );

  ftest_close_document_success( &error );
  CU_ASSERT( error == SUCCESS );

}

void test_makeCString_success( void ) {

  char* fString = "fortranString             ";
  size_t fortranLength = 13;

  char* cString = makeCString( fString, fortranLength );

  CU_ASSERT( strlen(cString) == fortranLength );
  CU_ASSERT( !strcmp( cString, "fortranString" ) );

  free(cString);

}

void test_makeCString_emptyString( void ) {

  char* fString = "";
  size_t fortranLength = 0;

  char* cString = makeCString( fString, fortranLength );

  CU_ASSERT( strlen(cString) == fortranLength );
  CU_ASSERT( !strcmp( cString, "" ) );

  free(cString);

}

void fTest_getElement_success(void) {

  int error;
  int length=100;
  char* cName;
  char name[100];

  ftest_get_element_success( name, &error, length );
  CU_ASSERT( error == SUCCESS );

  cName = makeCString( name, 15 );
  CU_ASSERT_STRING_EQUAL( cName, "Junkers JU 52");
  free(cName);
}

void fTest_addElement_addTree(void) {

  int error; 

  ftest_add_element_add_tree( &error );
  CU_ASSERT( error == SUCCESS );

}


void  fTest_get_point(void) {

  int error; 
  double x = 0.;
  double y = 0.;
  double z = 0.;

  ftest_get_point( &x, &y, &z, &error );
  CU_ASSERT( error == SUCCESS );
  CU_ASSERT( x == 11.0 && y == 12.2 && z == 31.3453 );
 
}
void  fTest_get_not_a_point(void) {

  int error; 
 
  ftest_get_not_a_point( &error );
  CU_ASSERT( error == COORDINATE_NOT_FOUND );
 
}
void fTest_add_point(void) {
  
  int error;
  double x = 0.;
  double y = 0.;
  double z = 0.;

  ftest_add_point( &error );
  CU_ASSERT( error == SUCCESS );

  ftest_get_added_point( &x, &y, &z, &error );
  
  CU_ASSERT( error == SUCCESS );
  CU_ASSERT( x == 1.0 && y == 2.34 && z == 3.1416 );

}

void fTest_add_matrix( void ) {

  int error = -1;
  int nRows = 0;
  int nColumns = 0;
  double array[9] = { 0., 0., 0., 0., 0., 0., 0., 0., 0.};

  ftest_add_matrix( &error );
  CU_ASSERT( error == SUCCESS );

  ftest_get_matrix_size( &nRows, &nColumns, &error );
  
  CU_ASSERT( error == SUCCESS );
  CU_ASSERT( nRows == 3 && nColumns == 3 );


  ftest_get_float_matrix( array, &error );

  CU_ASSERT( error == SUCCESS );
  CU_ASSERT( array[0] == 11. && 
	     array[1] == 12. && 
	     array[2] == 13. && 
	     array[3] == 21. && 
	     array[4] == 22. && 
	     array[5] == 23. && 
	     array[6] == 31. && 
	     array[7] == 32. &&
	     array[8] == 33.   );
  

}

void  fTest_check_document(void) {

	int error = -1;
	ftest_check_document_success( &error);
	CU_ASSERT( error == SUCCESS );

	ftest_check_document_failed( &error);
	CU_ASSERT( error == INVALID_HANDLE );
}

void  fTest_check_element(void) {
	
	int error = -1;
	ftest_check_element_success( &error);
	CU_ASSERT( error == SUCCESS );

	ftest_check_element_failed( &error);
	CU_ASSERT( error == ELEMENT_NOT_FOUND );
}

#endif /* FORTRAN */
void ____DUMMY___(void){}
