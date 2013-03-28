/* 
* Copyright (C) 2007-2012 German Aerospace Center (DLR/SC)
*
* Created: 2013-03-27 Martin Siggel <Martin.Siggel@dlr.de>
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


TEST(Bugs,cdatatext_segfault){
    TixiDocumentHandle handle;
    ReturnCode ret = tixiOpenDocument("TestData/bug_segfault.xml", &handle);
    ASSERT_EQ(SUCCESS, ret);
    
    char * text = NULL;
    int nChilds = 0;
    
    ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/Assembly", &text));
    
    tixiGetNumberOfChilds(handle, "/Assembly/BodySkeleton[1]/BodyFrame[1]", &nChilds);
    ASSERT_EQ(1, nChilds);
    
    ASSERT_EQ(SUCCESS, tixiGetTextElement(handle, "/Assembly/BodySkeleton[1]/BodyFrame[1]", &text));
    
    // this query segfaultet due to its cdata section
    ASSERT_EQ(SUCCESS, tixiGetChildElementName(handle, "/Assembly/BodySkeleton[1]/BodyFrame[1]", 1, &text));
    ASSERT_EQ(NULL, text);
    
}
