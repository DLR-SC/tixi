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
#include <stdlib.h>
#include "CUnit/CUnit.h"
#include "tixi.h"
#include "tixiInternal.h"

/** 
    @test tixiXSLTransformationToFile test
*/

static const char *xmlFilename = "TestData/valid_CPACS_dokumentiert.xml";
static const char *stylesheetFilename = "TestData/mappingInputRaw.xsl";
static const char *resultFileName = "TestData/result.xml";


void xslTransformationToFile(void)
{
    TixiDocumentHandle documentHandle = -1;
	CU_ASSERT( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS );
	CU_ASSERT( tixiXSLTransformationToFile( documentHandle, stylesheetFilename, resultFileName ) == SUCCESS );
	
    /* finish */
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );
}
