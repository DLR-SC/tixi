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
    @test tixiSchemaValidate
*/   

static TixiDocumentHandle documentHandleGet = -1;
static TixiDocumentHandle documentHandleAdd = -1;
static TixiDocumentHandle tmpHandle = -1;

int preVectorTests( void ) {
	
	char* xmlFilenameGet = "TestData/vectorcount.xml";
	char* xmlFilenameAdd = "TestData/vectoradd.xml";

    return ( tixiOpenDocument( xmlFilenameGet, &documentHandleGet ) != SUCCESS )
       ||  ( tixiOpenDocument( xmlFilenameAdd, &documentHandleAdd ) != SUCCESS );
}

int postVectorTests( void ) {

    return ( tixiCloseDocument( documentHandleGet ) != SUCCESS )
       ||  ( tixiCloseDocument( documentHandleAdd ) != SUCCESS );
}

void tixiVectorGetTests( void ){

    int count = 0;
    double *allPoints = NULL;

    // check sizes
    CU_ASSERT ( tixiGetVectorSize(documentHandleGet, "/a/aeroPerformanceMap/cfx", &count) == SUCCESS );
    CU_ASSERT ( count == 32 );
    CU_ASSERT ( tixiGetVectorSize(documentHandleGet, "/a/aeroPerformanceMap/cfy", &count) == ELEMENT_NOT_FOUND );
    CU_ASSERT ( tixiGetVectorSize(documentHandleGet, "/a/aeroPerformanceMap/cfz", &count) == SUCCESS );
    CU_ASSERT ( count == 2 );

    // check values returned
    count = 10;
    CU_ASSERT ( tixiGetFloatVector(documentHandleGet, "/a/aeroPerformanceMap/cfx", &allPoints, count) == SUCCESS );
    CU_ASSERT ( allPoints[0] == 1.0 ); printf("\n\n==>%f\n\n", allPoints[0]);
    CU_ASSERT ( allPoints[7] == 8.0 );
}

void tixiVectorAddTests( void ){
    
    int count = 0;
    double points[10] = {1, 4, 5.8, 77.0, 5, 6, 7, 8, 9, 10};
    double *newPoints = NULL;
	char* xmlOutName = "x.xml";
    count = 3;

    // write parts of the array to an intermediate file
    CU_ASSERT ( tixiAddFloatVector(documentHandleAdd, "/a", "test", points, count) == SUCCESS );
    CU_ASSERT ( tixiSaveDocument(documentHandleAdd, xmlOutName) == SUCCESS );

    // read in again and check values
    CU_ASSERT ( tixiOpenDocument( xmlOutName, &tmpHandle ) == SUCCESS );
    CU_ASSERT ( tixiGetFloatVector(tmpHandle, "/a/test", &newPoints, count) == SUCCESS );
    CU_ASSERT ( (newPoints[0] == 1) && (newPoints[1] == 4) && (newPoints[2] == 5.8) );
	CU_ASSERT ( tixiCloseDocument( tmpHandle ) == SUCCESS );

}
