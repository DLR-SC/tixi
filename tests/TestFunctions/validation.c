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
    @test tixiSchemaValidate
*/   

static TixiDocumentHandle documentHandle = -1;
int preValidateSchema( void ) {
	
	char* xmlFilename = "TestData/valid_CPACS_dokumentiert.xml";

	assert( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS );

	return 0;
}

int postValidateSchema( void ) {

	assert( tixiCloseDocument( documentHandle ) == SUCCESS );

	return 0;
}



void  tixiValidateSchema( void ){

	char* schemaFilename = "TestData/valid_cpacs_schema.xsd";

	CU_ASSERT( tixiSchemaValidateFromFile( documentHandle, schemaFilename ) == SUCCESS);
}

