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
#include "CUnit/CUnit.h"
#include "tixi.h"

/** 
    $Id$
    @test tixiOpenDocument
    A name of an XML-file and a test case number is passed  to the routine. 
    The following test cases can be run:
    Test0 tries to open an non existing file
    Test1 opens an non wellformed file
    Test2 tests if the file can be opened and is well formed
    Test3 tests if a valid handle is returned
*/
void openDocument_nonExisting( void ) {
 
	TixiDocumentHandle documentHandle = -1;
	char* xmlFilename = "____HOPEFULLY_THIS_FILE_NAME_DOES_NOT_EXIST~~~~";
	
	CU_ASSERT( tixiOpenDocument( xmlFilename, &documentHandle ) == OPEN_FAILED );
}
void openDocument_notWellFormed( void ){

	TixiDocumentHandle documentHandle = -1;
	char* xmlFilename = "TestData/illformed.xml";

	CU_ASSERT( tixiOpenDocument( xmlFilename, &documentHandle ) == NOT_WELL_FORMED );
}
void openDocument_isWellFormed( void ) {
	TixiDocumentHandle documentHandle = -1;
	char* xmlFilename = "TestData/in.xml";

	CU_ASSERT( tixiOpenDocument( xmlFilename, &documentHandle )  == SUCCESS );
	tixiCloseDocument( documentHandle );
}
	 
void openDocument_checkHandle( void ) {
	TixiDocumentHandle documentHandle = -1;
	char* xmlFilename = "TestData/in.xml";

    CU_ASSERT( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS );
	CU_ASSERT( documentHandle != -1 );
	tixiCloseDocument( documentHandle );
}


void openDocument_openHttpURL( void ) {
	TixiDocumentHandle documentHandle = -1;
	char* xmlUrl = "http://www.w3schools.com/XML/note.xml";

    CU_ASSERT( tixiOpenDocumentFromHTTP( xmlUrl, &documentHandle ) == SUCCESS );
	CU_ASSERT( documentHandle != -1 );
	tixiCloseDocument( documentHandle );
}
