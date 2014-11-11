/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* � � http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "tixi.h"


int main()
{

  char *docString = NULL;
  double * myFloat = NULL;
  int i = 0;
  TixiDocumentHandle handle;

  printf("TIXI Demo\n\n");
  printf("Using TIXI %s\n", tixiGetVersion());

  //create xml doc
  tixiCreateDocument("MyRoot", &handle);
  tixiAddHeader(handle, "tixiDemo", tixiGetVersion(), "Martin Siggel");
  tixiAddIntegerElement(handle ,"/MyRoot", "MyInt", 1234, "%d");

  //add float vector
  myFloat = (double*) malloc(sizeof(double)*20);
  for(i = 0; i < 20; ++i) myFloat[i] = (double)i*i;
  tixiAddFloatVector(handle, "/MyRoot","MyFloatVector", myFloat, 20, "%g");
  free(myFloat);

  //print doc
  tixiExportDocumentAsString(handle, &docString);
  printf("%s", docString);

  //write to file
  tixiSaveDocument(handle, "MyFile.xml");
  tixiCloseDocument(handle);
  handle = 0;

  tixiCleanup();

  return (0);
}
