/*
* Copyright (C) 2017 German Aerospace Center (DLR/SC)
*
* Created: 2017-03-29 Martin Siggel <Martin.Siggel@dlr.de>
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

#include "tixi.h"

#include <stdio.h>

int main()
{
    TixiDocumentHandle handle;
    char* text = 0;

    tixiCreateDocument("cpacs", &handle);
    tixiDeclareNamespace(handle, "/cpacs", "http://www.w3.org/2001/XMLSchema-instance", "xsi");
    tixiRegisterNamespace(handle, "http://www.w3.org/2001/XMLSchema-instance", "xsi");
    tixiAddTextAttribute(handle, "/cpacs", "xsi:noNamespaceSchemaLocation", "cpacs_schema.xsd");
    tixiCreateElement(handle, "/cpacs", "header");

    // print xml file to screen
    tixiExportDocumentAsString(handle, &text);
    printf("The content of the cpacs file is:\n\n%s\n", text);

    return 0;
}
