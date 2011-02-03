/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Hans-Peter Kersken <Hans-Peter.Kersken@dlr.de>
* Changed: $Id: createDocument.c 4392 2011-02-03 08:52:55Z litz_ma $ 
*
* Version: $Revision: 4392 $
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
#include <stdlib.h>

#include "CUnit/CUnit.h"

#include "tixi.h"

/** 
    @test tixiCreateDocument test
    
*/

void createDocument_noElementName ( void ){
	
	char* rootElementName = NULL;
	TixiDocumentHandle handle = -1;

	CU_ASSERT( tixiCreateDocument( rootElementName, &handle ) == NO_ELEMENT_NAME );  
}


void createDocument_invalidRootNodeName( void ){
	
	char* rootElementName = "1";
	TixiDocumentHandle handle = -1;

	CU_ASSERT( tixiCreateDocument( rootElementName, &handle ) == INVALID_XML_NAME );  
}
void createDocument_success( void ){
	
	char* rootElementName = "rootElement";
	TixiDocumentHandle handle = -1;

	CU_ASSERT( tixiCreateDocument( rootElementName, &handle ) == SUCCESS );  
	CU_ASSERT( handle != -1 );
	if ( handle != -1 ) {
		CU_ASSERT( tixiSaveDocument( handle, "createDocument_success.xml" )== SUCCESS );
		CU_ASSERT( tixiCloseDocument( handle )== SUCCESS );
	}
}

void createDocument_addHeader( void ){

    char* rootElementName = "rootElement";
    TixiDocumentHandle handle = -1;

    CU_ASSERT( tixiCreateDocument( rootElementName, &handle ) == SUCCESS );  

    if ( handle != -1 ) {
        CU_ASSERT( tixiAddHeader( handle, "ToolName", "4.7.11", "Rudi Ratlos" ) == SUCCESS );
        CU_ASSERT( tixiSaveDocument( handle, "documentWithHeader.xml" )== SUCCESS );
        CU_ASSERT( tixiCloseDocument( handle )== SUCCESS );

#if __unix__ || unix
        system( "sed  /timestamp/d documentWithHeader.xml >tmp.xml");
        CU_ASSERT( system("diff -w tmp.xml TestData/documentWithHeaderReference.xml" ) == 0 );

#endif /* __unix__ */

    }
}
