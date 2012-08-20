/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Arne Bachmann <Arne.Bachmann@dlr.de>
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
#include "CUnit/CUnit.h"
#include "tixi.h"

/** 
    $Id$
    @test tixiCloseDocument
    A name of an XML-file and a test case number is passed  to the routine. 
    The following test cases can be run:
    Test0 tries to open file and close it.
    Test1 passes an invalid handle to tixiCloseDocument after having opened a file successfully
    Test2 tries to close a document which his not managaged by the list
*/   

void initCloseDocument( void ) {
}

void closeDocument_close( void ) {
  
	TixiDocumentHandle documentHandle = -1;
	char* xmlFilename = "TestData/in.xml";

	CU_ASSERT_FATAL( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS );
	
	CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );
}
void closeDocument_invalidHandle( void ){

	TixiDocumentHandle documentHandle = -1;

	CU_ASSERT( tixiCloseDocument( documentHandle ) == INVALID_HANDLE );
}

void closeDocument_closeMultiple( void ) {
  
	TixiDocumentHandle documentHandle1 = -1;
	TixiDocumentHandle documentHandle2 = -1;
	TixiDocumentHandle documentHandle3 = -1;
	char* xmlFilename = "TestData/in.xml";

	CU_ASSERT_FATAL( tixiOpenDocument( xmlFilename, &documentHandle1 ) == SUCCESS );
	CU_ASSERT_FATAL( tixiOpenDocument( xmlFilename, &documentHandle2 ) == SUCCESS );
	CU_ASSERT_FATAL( tixiOpenDocument( xmlFilename, &documentHandle3 ) == SUCCESS );
	
	 CU_ASSERT( tixiCloseDocument( documentHandle1 ) == SUCCESS );
    CU_ASSERT( tixiCloseDocument( documentHandle3 ) == SUCCESS );
    CU_ASSERT( tixiCloseDocument( documentHandle2 ) == SUCCESS );



}
