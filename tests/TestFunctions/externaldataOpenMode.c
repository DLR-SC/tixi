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
#include <stdio.h>
#include "CUnit/CUnit.h"
#include "tixi.h"
/** 
    @test OpenExternalData
        
*/   

static TixiDocumentHandle documentHandle = -1;
int preOpenExternalData( void ) {
	
	char* xmlFilename = "TestData/externaldata-main.xml";
	
	assert( tixiOpenDocumentRecursive( xmlFilename, &documentHandle, OPENMODE_RECURSIVE ) == SUCCESS );
	
	return 0;
}

int postOpenExternalData( void ) {

	assert( tixiCloseDocument( documentHandle ) == SUCCESS );

	return 0;
}



void  openExternalData_getExternalData( void ){

	char* elementPath = "/plane/testNode/aVeryTest/point/z";
    char* text = NULL;
    
	/* get a path from a element from the satellite file... */
	CU_ASSERT( tixiGetTextElement( documentHandle, elementPath, &text ) == SUCCESS);
}


void  openExternalData_writeBackExternalData( void ){

	char* elementPath = "/plane/testNode/aVeryTest/point/z";
	char* saveFilename = "TestData/externaldata-save.xml";
	TixiDocumentHandle newDocumentHandle = -1;	
	char* extElementPath = "/plane/testNode/aVeryTest/point/xyc";
    char* text = NULL;
        
		/* get a path from a element from the satellite file and  */
		CU_ASSERT( tixiGetTextElement( documentHandle, elementPath, &text ) == SUCCESS );
		CU_ASSERT( tixiAddTextElement(documentHandle, "/plane/testNode/aVeryTest/point", "xyc", "test") == SUCCESS);
		CU_ASSERT( tixiSaveDocument( documentHandle, saveFilename ) == SUCCESS );

		///* Now open the satellite file and look for the new element. */
		CU_ASSERT( tixiOpenDocumentRecursive( saveFilename, &newDocumentHandle, OPENMODE_RECURSIVE ) == SUCCESS );
		CU_ASSERT( tixiGetTextElement( newDocumentHandle, extElementPath, &text ) == SUCCESS );
		CU_ASSERT( tixiCloseDocument( newDocumentHandle ) == SUCCESS );
}

