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
 
#include "test.h" // Brings in the GTest framework

#include "tixi.h"


static char* xPathXmlFilename = "TestData/xpathsEvaluate.xml";

static char *elementPathExists = "/root/a[@uID=\"test\"]/b[@uID=\"b\"]";
static char *elementPathDoesnt = "/root/a[@uID=\"schlumpf\"]/b[@uID=\"empty_element\"]";


TEST(xpath_evaluate_checks, tixiXPathEvaluateNodeNumber)
{
    TixiDocumentHandle documentHandle = -1;
    int num = 0;

    ASSERT_TRUE( tixiOpenDocument( xPathXmlFilename, &documentHandle ) == SUCCESS );
    ASSERT_TRUE( tixiXPathEvaluateNodeNumber( documentHandle, "//@uID", &num) == SUCCESS );
    ASSERT_TRUE( num == 6 );
    ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
}

TEST(xpath_evaluate_checks, tixiXPathEvaluateNodeNumber_fail)
{
    TixiDocumentHandle documentHandle = -1;
    int num = 0;

    ASSERT_TRUE( tixiOpenDocument( xPathXmlFilename, &documentHandle ) == SUCCESS );
    ASSERT_TRUE( tixiXPathEvaluateNodeNumber( documentHandle, "//@kruzifix", &num) == FAILED );
    ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
}


TEST(xpath_evaluate_checks, tixiXPathExpressionGetTextByIndex_char)
{
    TixiDocumentHandle documentHandle = -1;
    char *text = NULL;

    ASSERT_TRUE( tixiOpenDocument( xPathXmlFilename, &documentHandle ) == SUCCESS );
    ASSERT_TRUE( tixiXPathExpressionGetTextByIndex( documentHandle, "//@uID", 4, &text) == SUCCESS );
    if ( text ) {
        ASSERT_TRUE( !strcmp(text, "schlumpf"));
    }
    ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
}

