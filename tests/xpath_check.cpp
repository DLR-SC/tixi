/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: xpaths.c 126 2012-08-12 14:55:28Z martinsiggel@gmail.com $ 
*
* Version: $Revision: 126 $
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
// #include "tixiInternal.h"
 
static char* xmlFilename = "TestData/xpaths.xml";
static char *elementPathExists = "/root/a[@uID=\"test\"]/b[@uID=\"b\"]";
static char *elementPathDoesnt = "/root/a[@uID=\"schlumpf\"]/b[@uID=\"empty_element\"]";


TEST(xpath_checks, xpath_generation) 
{
    TixiDocumentHandle documentHandle = -1;
    char *result = NULL;
    
    /* init */
    ASSERT_TRUE(tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS); 
}

