/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
* Changed: $Id: addAttribute.c 4392 2011-02-03 08:52:55Z litz_ma $ 
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
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "CUnit/CUnit.h"

#include "tixi.h"

/** 
    @test Tests for addElement routines.
*/   

static TixiDocumentHandle documentHandle = -1;

int preAddAttribute( void ) {

	char* rootElementName = "rootElement";
    char* elementName = "element";

	if( tixiCreateDocument( rootElementName, &documentHandle ) == SUCCESS ) {

        if ( tixiAddTextElement( documentHandle, "/rootElement", elementName, "content" ) == SUCCESS ){
		    return 0;
        }
        else {
            return 1;
        }
	}
	else {
		return 1;
	}

}

int postAddAttribute( void ) {
	return 0;
}


void addAttribute_text( void ) {
  char* elementPath = "/rootElement/element";
  char* attributeName = "attribute";
  
  CU_ASSERT( tixiAddTextAttribute( documentHandle, elementPath, attributeName, "attributeValue" ) == SUCCESS );
}
void addAttribute_double( void ) {
  
  char* elementPath = "/rootElement/element";
  double value = 3.14159;
  char* format = "%10.6f";
  
  CU_ASSERT( tixiAddDoubleAttribute( documentHandle, elementPath, "doubleAttribute", value, format ) == SUCCESS );
  CU_ASSERT( tixiAddDoubleAttribute( documentHandle, elementPath, "doubleAttribute_g", value, NULL ) == SUCCESS );
}
void addAttribute_integer( void ) {
  
  char* elementPath = "/rootElement/element";
  int value = 4711;
  char* format = "%5d";
  
  
  CU_ASSERT( tixiAddIntegerAttribute( documentHandle, elementPath, "intAttribute", value, format ) == SUCCESS );
  CU_ASSERT( tixiAddIntegerAttribute( documentHandle, elementPath, "intAttribute_d", value, NULL ) == SUCCESS );
}

#if __unix__ || unix

void addAttribute_compareResult( void ) {

	CU_ASSERT( tixiSaveDocument( documentHandle, "addAttribute.xml" ) == SUCCESS );
    CU_ASSERT( tixiCloseDocument( documentHandle ) == SUCCESS );
	CU_ASSERT( system("diff -w addAttribute.xml TestData/addAttributeReference.xml" ) == 0 );
	
}

#endif /* __unix__ */
