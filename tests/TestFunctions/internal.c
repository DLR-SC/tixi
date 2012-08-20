/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Arne Bachmann <Arne.Bachmann@dlr.de>
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
#include "CUnit/CUnit.h"
#include "tixi.h"
#include "tixiInternal.h"
/** 
    @test Tests for internal TIXI routines .
*/


void buildString_success(void)
{
    char* buffer = NULL;
    char* format = "%4d";
    int i = 1234;
    char* floatFormat = "%25.5f";
    double egyptianPi = 22./7.;
    
    buffer = buildString( format, i );
    CU_ASSERT( !strcmp( buffer, "1234" ) );
    free(buffer);
    
    buffer = buildString( floatFormat, egyptianPi );
    CU_ASSERT( strlen( buffer ) == 25);
    free(buffer);
}
