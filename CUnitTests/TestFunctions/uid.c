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
    @test tixiUIDCheckDuplicates
*/   

static TixiDocumentHandle documentHandle = -1;

const char* filename_uid_correct = "TestData/uid_correct.xml";
const char* filename_uid_duplicated = "TestData/uid_duplicated.xml";
const char* filename_without_uids = "TestData/uid_no_uids.xml";


void tixiUIDCheck_ok( void )
{
	TixiDocumentHandle documentHandle = -1;
	CU_ASSERT( tixiOpenDocument( filename_uid_correct, &documentHandle ) == SUCCESS );

	CU_ASSERT( tixiUIDCheckDuplicates( documentHandle ) == SUCCESS);

    /* finish */
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );
}



void tixiUIDCheck_duplicates( void )
{
	TixiDocumentHandle documentHandle = -1;
	CU_ASSERT( tixiOpenDocument( filename_uid_duplicated, &documentHandle ) == SUCCESS );

	CU_ASSERT( tixiUIDCheckDuplicates( documentHandle ) == UID_NOT_UNIQUE);

    /* finish */
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );


	/* check a document without uids */
	CU_ASSERT( tixiOpenDocument( filename_without_uids, &documentHandle ) == SUCCESS );
	CU_ASSERT( tixiUIDCheckDuplicates( documentHandle ) == SUCCESS);
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );
}



void tixiUIDCheck_getXPath( void )
{
	TixiDocumentHandle documentHandle = -1;
	char *xpath = NULL;

	CU_ASSERT( tixiOpenDocument( filename_uid_correct, &documentHandle ) == SUCCESS );

	CU_ASSERT( tixiUIDGetXPath( documentHandle, "b", &xpath ) == SUCCESS );

	CU_ASSERT( !strcmp(xpath, "/root/a/b") == SUCCESS );

    /* finish */
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );
}



void tixiUIDCheck_getXPathFailed( void )
{
	TixiDocumentHandle documentHandle = -1;
	char *xpath = NULL;

	CU_ASSERT( tixiOpenDocument( filename_uid_correct, &documentHandle ) == SUCCESS );

	CU_ASSERT( tixiUIDGetXPath( documentHandle, "bc", &xpath ) == SUCCESS );

	CU_ASSERT( strlen(xpath) == 0 );

    /* finish */
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );
}


void tixiUIDCheck_checkExists( void )
{
	TixiDocumentHandle documentHandle = -1;

	CU_ASSERT( tixiOpenDocument( filename_uid_correct, &documentHandle ) == SUCCESS );

	CU_ASSERT( tixiUIDCheckExists( documentHandle, "schlumpf" ) == SUCCESS );

	CU_ASSERT( tixiUIDCheckExists( documentHandle, "i_do_not_exist" ) == UID_DONT_EXISTS );

    /* finish */
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );
}

void tixiUIDCheck_setToXpath( void )
{
	TixiDocumentHandle documentHandle = -1;
	char *text = NULL;
	char *new_uid_text = "new_uid_text";

	CU_ASSERT( tixiOpenDocument( filename_uid_correct, &documentHandle ) == SUCCESS );

	CU_ASSERT( tixiUIDSetToXPath(documentHandle, "/root/a[2]/b[1]", new_uid_text) == SUCCESS );

	CU_ASSERT( tixiGetTextAttribute(documentHandle, "/root/a[2]/b[1]", "uID", &text) == SUCCESS );

	CU_ASSERT( !strcmp(text, new_uid_text) );

	/* finish */
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );
}




