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

#include "test.h" // Brings in the GTest framework

#include "tixi.h"


TEST(opendocument_checks, non_existing) 
{
    TixiDocumentHandle documentHandle = -1;
    const char* xmlFilename = "____HOPEFULLY_THIS_FILE_NAME_DOES_NOT_EXIST~~~~";
    
    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == OPEN_FAILED );
}

TEST(opendocument_checks, not_well_formed) 
{
    TixiDocumentHandle documentHandle = -1;
    const char* xmlFilename = "TestData/illformed.xml";

    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == NOT_WELL_FORMED );
}

TEST(opendocument_checks, is_well_formed) 
{
    TixiDocumentHandle documentHandle = -1;
    const char* xmlFilename = "TestData/in.xml";

    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle )  == SUCCESS );
    tixiCloseDocument( documentHandle );
}


TEST(opendocument_checks, check_handle) 
{
    TixiDocumentHandle documentHandle = -1;
    const char* xmlFilename = "TestData/in.xml";

    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS );
    ASSERT_TRUE( documentHandle != -1 );
    tixiCloseDocument( documentHandle );
}

TEST(opendocument_checks, open_http_url) 
{
    TixiDocumentHandle documentHandle = -1;
    const char* xmlUrl = "http://www.w3schools.com/XML/note.xml";

    ASSERT_TRUE( tixiOpenDocumentFromHTTP( xmlUrl, &documentHandle ) == SUCCESS );
    ASSERT_TRUE( documentHandle != -1 );
    tixiCloseDocument( documentHandle );
}

TEST(opendocument_checks, open_http_url_not_valid)
{
    TixiDocumentHandle documentHandle = -1;
    const char* xmlUrl = "http://a/b";

    ASSERT_TRUE( tixiOpenDocumentFromHTTP( xmlUrl, &documentHandle ) != SUCCESS );
    ASSERT_TRUE( documentHandle == -1 );
    tixiCloseDocument( documentHandle );
}



