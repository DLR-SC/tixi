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
*     http://www.apache.org/licenses/LICENSE-2.0
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
    @test tixiRemoveAttribute
*/   

static TixiDocumentHandle documentHandle = -1;
int preRemoveAttribute( void ) {
	
	char* xmlFilename = "TestData/in.xml";

    return ( tixiOpenDocument( xmlFilename, &documentHandle ) != SUCCESS );
}

int postRemoveAttribute( void ) {

    return ( tixiCloseDocument( documentHandle ) != SUCCESS );
}



void  getRemoveAttribute_singleAttribute( void ){

		char* elementPath = "/plane/wings";		
		char* attributeName = "numberOfWings";		
		char* fullElementPath = "/plane/wings[@numberOfWings]";
        char* text = NULL;

        CU_ASSERT( tixiCheckElement( documentHandle, fullElementPath ) == SUCCESS);

        CU_ASSERT( tixiRemoveAttribute( documentHandle, elementPath, attributeName ) == SUCCESS );

        CU_ASSERT( tixiCheckElement( documentHandle, fullElementPath ) != SUCCESS);
}


/*
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
*/
