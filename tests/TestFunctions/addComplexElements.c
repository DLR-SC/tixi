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
#include "CUnit/CUnit.h"
#include "tixi.h"

/** 
    @test Tests for adding composite elements.
*/   

static TixiDocumentHandle documentHandle = -1;

int preAddElementList( void ) {

	char* rootElementName = "root";
	if( tixiCreateDocument( rootElementName, &documentHandle ) == SUCCESS ) {
		return (0);
	}
	else {
		return 1;
	}
}

void addElementList_success( void ) {
    
#define N_VALUES 5

    double values[N_VALUES] = {0., 1., 2., 3., 4.};
    char* attributes[N_VALUES] = { "a", "b", "c", "d", "e"};
    char* parentPath = "/root";
    char* listName = "numbers";
    char* childName = "number";
    char* format = "%08.2f";
    char* childAttributeName = "letter";
    int nValues = N_VALUES;

    CU_ASSERT( tixiAddDoubleListWithAttributes( documentHandle, parentPath,  listName, childName, childAttributeName, 
                                           values,  format,  attributes,  nValues ) == SUCCESS );

    CU_ASSERT( tixiSaveDocument( documentHandle, "list.xml" )== SUCCESS);
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS);
}
