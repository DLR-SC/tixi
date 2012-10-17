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


TEST(closedocument_checks, close_success) 
{
    TixiDocumentHandle documentHandle = -1;
    const char* xmlFilename = "TestData/in.xml";

    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS ); 
    ASSERT_TRUE( tixiCloseDocument( documentHandle ) == SUCCESS );
}


TEST(closedocument_checks, invalid_handle) 
{
    TixiDocumentHandle documentHandle = -1;
    ASSERT_TRUE( tixiCloseDocument( documentHandle ) == INVALID_HANDLE );
}


TEST(closedocument_checks, close_multiple) 
{
    TixiDocumentHandle documentHandle1 = -1;
    TixiDocumentHandle documentHandle2 = -1;
    TixiDocumentHandle documentHandle3 = -1;
    const char* xmlFilename = "TestData/in.xml";

    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle1 ) == SUCCESS );
    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle2 ) == SUCCESS );
    ASSERT_TRUE( tixiOpenDocument( xmlFilename, &documentHandle3 ) == SUCCESS );

    ASSERT_TRUE( tixiCloseDocument( documentHandle1 ) == SUCCESS );
    ASSERT_TRUE( tixiCloseDocument( documentHandle3 ) == SUCCESS );
    ASSERT_TRUE( tixiCloseDocument( documentHandle2 ) == SUCCESS );
}
