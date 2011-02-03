/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: xpaths.c 4392 2011-02-03 08:52:55Z litz_ma $ 
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
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "tixi.h"
#include "tixiInternal.h"

/** 
    @test generateXpathFromNode test
*/

static const char* xmlFilename = "TestData/xpaths.xml";
static const char *elementPathExists = "/root/a[@uID=\"test\"]/b[@uID=\"b\"]";
static const char *elementPathDoesnt = "/root/a[@uID=\"schlumpf\"]/b[@uID=\"empty_element\"]";


void xPathGeneration(void)
{
    TixiDocument *document = NULL;
    xmlDocPtr xmlDocument = NULL;
    xmlXPathObjectPtr xpathObject = NULL;
    xmlNodePtr element = NULL;
    TixiDocumentHandle documentHandle = -1;
    char *result = NULL;

    /* init */
	CU_ASSERT( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS );
    document = getDocument(documentHandle);
    CU_ASSERT( document );
    xmlDocument = document->docPtr;

    /* test the xPath creation: get xmlNodePtr by replicating code from tixiImpl.c */
    CU_ASSERT( checkElement(xmlDocument, elementPathExists, &element, &xpathObject) == SUCCESS );
    result = generateXPathFromNodePtr(documentHandle, element);
    CU_ASSERT( strcmp(result, elementPathExists) == 0 );   // must return the same as provided

    CU_ASSERT( checkElement(xmlDocument, elementPathDoesnt, &element, &xpathObject) == SUCCESS );
    result = generateXPathFromNodePtr(documentHandle, element);
    CU_ASSERT( strcmp(result, elementPathDoesnt) == 0 );   // must return the same as provided

    /* finish */
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS )
}

void xPathExpressionEvaluation(void)
{
    TixiDocumentHandle documentHandle = -1;
    char *result = NULL;
	int count = 0;
	char *text = NULL;

    /* init */
	CU_ASSERT( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS );

	/* Test counting of number of node selected via xpath */
	CU_ASSERT( tixiXPathEvaluateNodeNumber( documentHandle, "//@uID", &count ) == SUCCESS );
	CU_ASSERT( count == 5 );

	/* selecting a secific element */
	CU_ASSERT( tixiXPathExpressionGetTextByIndex( documentHandle, "//@uID", 4, &text) == SUCCESS );
    CU_ASSERT( strcmp(text, "schlumpf") == 0 );   // must return the same as provided

	/* selecting a secific element, but index too high */
	CU_ASSERT( tixiXPathExpressionGetTextByIndex( documentHandle, "//@uID", 99, &text) == FAILED );
}

