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
*   http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include "CUnit/CUnit.h"
#include "tixi.h"
/** 
    @test tixiGetXXXAttribute
*/   

static TixiDocumentHandle documentHandle = -1;


int preGetAttribute( void ) {
	
	char* xmlFilename = "TestData/in.xml";

    return tixiOpenDocument( xmlFilename, &documentHandle ) != SUCCESS;
}

int postGetAttribute( void ) {

    return tixiCloseDocument( documentHandle ) != SUCCESS ;
}

void  getTextAttribute_invalidHandle( void ){

	char* text = NULL;
	char* elementPath = "/plane/name";
	char* attributeName = "non_existing_attribute";

	CU_ASSERT( tixiGetTextAttribute( -1, elementPath, attributeName, &text ) == INVALID_HANDLE );
}

void  getTextAttribute_attributeNotFound( void ){

	char* text = NULL;
	char* elementPath = "/plane/wings/wing[1]";
	char* attributeName = "non_existing_attribute";
	CU_ASSERT( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == ATTRIBUTE_NOT_FOUND );
}

void  getTextAttribute_elementNotFound( void ){

	char* text = NULL;
	char* elementPath = "/plane/name/pp";
	char* attributeName = "non_existing_attribute";
	CU_ASSERT( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == ELEMENT_NOT_FOUND );
}



void  getTextAttribute_getText( void ){

	char* text = NULL;
	char* elementPath = "/plane/wings/wing[1]";
	char* attributeName = "position";
	CU_ASSERT_FATAL( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == SUCCESS );
	if ( text ) {
		CU_ASSERT_STRING_EQUAL( text, "left");
	}
}

void  getTextAttribute_invalidXPath( void ){

	char* text = NULL;
	char* elementPath = "cc/plane/name/pp";
	char* attributeName = "position";
	CU_ASSERT( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == INVALID_XPATH );
}

void  getTextAttribute_notUnique( void ){

	char* text = NULL;
	char* elementPath = "/plane/wings/wing";
	char* attributeName = "position";
	CU_ASSERT( tixiGetTextAttribute( documentHandle, elementPath, attributeName, &text ) == ELEMENT_PATH_NOT_UNIQUE );
}



void getDoubleAttribute( void ) {

	double number = 0.;
	char* elementPath = "/plane/coordinateOrigin";
	char* attributeName = "scaling";
	CU_ASSERT( tixiGetDoubleAttribute( documentHandle, elementPath, attributeName, &number ) == SUCCESS );
	CU_ASSERT_DOUBLE_EQUAL( number, 1.3456, 1.e-10);
}

void getIntegerAttribute( void ) {

	int number = 0;
	char* elementPath = "/plane/wings";
	char* attributeName = "numberOfWings";

	CU_ASSERT( tixiGetIntegerAttribute( documentHandle, elementPath, attributeName, &number ) == SUCCESS );
	CU_ASSERT( number == 2 );
}
