/* 
* Copyright (C) 2007-2011 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Arne Bachmann <Arne.Bachmann@dlr.de>
* Changed: $Id: arrays.c 4392 2011-02-03 08:52:55Z litz_ma $ 
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
#include "CUnit/CUnit.h"
#include "tixi.h"
/** 
    @test tixiSchemaValidate
        
*/   

static TixiDocumentHandle documentHandle = -1;
static TixiDocumentHandle tmpHandle = -1;
static const char *xPath = "/root/aeroPerformanceMap";
static const char *invalidXPath = "][";
static const char *wrongXPath = "/root/aEroPerformanceMap";

int preArrayTests( void ) {
    
    char* xmlFilename = "TestData/arraytests.xml";

    assert( tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS );
    return 0;
}

int postArrayTests( void ) {

    assert( tixiCloseDocument( documentHandle ) == SUCCESS );
    return 0;
}

/****
 * 1. Get number of dimensions (if not known in advance)
 * 2. Get sizes of alle dimensions / pre-calculated product of all those dimensions' sizes
 * 3. Meta-data: Get numerical "meanings" of all dimensions positions (0..n-1 of a dimension's size)
 * 4. Get one horizontal cut over all dimensions for one parameter
 ****/
void tixiArrayTestsPositive( void ){

    // define vars
    int dims = 0;
    int dim = 0;
    int pars = 0;
    int par = 0;
    int arraySize = 0;
    int sizes[] = {0, 0, 0, 0};
    int pos[] = {0, 0, 0, 0};
    double *values = NULL;
    char **names = NULL;

    // check number of dimensions
    CU_ASSERT ( tixiGetArrayDimensions(documentHandle, xPath, &dims) == SUCCESS );
    CU_ASSERT ( dims == 4 );   // four dimensions with each a different number of values

    // check sizes of dimensions and product of them
    CU_ASSERT ( tixiGetArrayDimensionSizes(documentHandle, xPath, sizes, &arraySize) == SUCCESS );
    // try again to ensure all pointers are still there!
    CU_ASSERT ( tixiGetArrayDimensionSizes(documentHandle, xPath, sizes, &arraySize) == SUCCESS );
    CU_ASSERT ( (sizes[0] == 1) && (sizes[1] == 2) && (sizes[2] == 3) && (sizes[3] == 8) );
    CU_ASSERT ( arraySize == 1 * 2 * 3 * 8 );    // check product of sizes

    // check dimension values (meanings)
    for (dim = 0; dim < dims; dim ++) {
        values = (double *) malloc(sizes[dim] * sizeof(double));    // get values temporary array
        CU_ASSERT (tixiGetArrayDimensionValues(documentHandle, xPath, dim, values) == SUCCESS );
        CU_ASSERT (
                ( (dim == 0) && (values[0] == 1.0) )
            ||  ( (dim == 1) && (values[1] == 2.0) )
            ||  ( (dim == 2) && (values[2] == 10.) )
            ||  ( (dim == 3) && (values[0] == 1.0) && (values[7] == 8.0) )
        );
        free(values);
    }

    // check dimension names
    names = (char **) malloc(dims * sizeof(char *));
    CU_ASSERT ( tixiGetArrayDimensionNames(documentHandle, xPath, names) == SUCCESS );
    CU_ASSERT ( strcmp(names[0], "machNumber") == 0);
    CU_ASSERT ( strcmp(names[1], "reynoldsNumber") == 0);
    CU_ASSERT ( strcmp(names[2], "angleOfYaw") == 0);
    CU_ASSERT ( strcmp(names[3], "angleOfAttack") == 0);
    for (dim = 0; dim < dims; dim ++) {
        free(names[dim]);
    }
    free(names);

    // check number of parameter
    CU_ASSERT ( tixiGetArrayParameters(documentHandle, xPath, &pars) == SUCCESS );
    CU_ASSERT ( pars == 7 );    // one invalid and one not counting (mapType="Array")

    // check parameter names
    names = (char **) malloc(pars * sizeof(char *));
    for (par = 0; par < pars; par ++) {
        names[par] = (char*) malloc((3 + 1) * sizeof(char));
    }
    CU_ASSERT ( tixiGetArrayParameterNames(documentHandle, xPath, names) == SUCCESS);
    CU_ASSERT ( strcmp(names[0], "cfx") == 0 );
    CU_ASSERT ( strcmp(names[1], "cfy") == 0 );
    CU_ASSERT ( strcmp(names[2], "cfz") == 0 );
    CU_ASSERT ( strcmp(names[3], "cmx") == 0 );
    CU_ASSERT ( strcmp(names[4], "cmy") == 0 );
    CU_ASSERT ( strcmp(names[5], "cmz") == 0 );
    for (par = 0; par < pars; par ++) {
        free(names[par]);
    }
    free(names);

    // read one array
    values = (double *) malloc(arraySize * sizeof(double));    // get values temporary array
    CU_ASSERT ( tixiGetArray(documentHandle, xPath, "cmz", values) == SUCCESS );    // get array of subkey
    CU_ASSERT ( values[0] == 1.0 );    // [0][0][0][0]
    CU_ASSERT ( values[1] == 2.0 );    // [0][0][0][1]
    CU_ASSERT ( values[16] == 101.0 );    // [0][0][2][1]
    CU_ASSERT ( values[23] == 108.0 );    // [0][0][2][7]
    CU_ASSERT ( values[32] == 201.0 );    // [0][1][1][0]
    CU_ASSERT ( values[47] == 216.0 );    // [0][1][2][7]
    CU_ASSERT ( values[0 * sizes[1] * sizes[2] * sizes[3]
                     + 1 * sizes[2] * sizes[3]
                     + 0 * sizes[3]
                     + 0
                     - 1] == 108.0);    // [0][1][0][0]
    CU_ASSERT ( values[0 * sizes[1] * sizes[2] * sizes[3]
                     + 1 * sizes[2] * sizes[3]
                     + 1 * sizes[3]
                     + 3
                     - 1] == 203.0);    // [0][1][1][3]

    // test array value fetching
    pos[0] = 0; pos[1] = 0; pos[2] = 0; pos[3] = 0; CU_ASSERT ( tixiGetArrayValue(values, sizes, pos, dims) == 1.0 );   // [0][0][0][0]
    pos[0] = 0; pos[1] = 0; pos[2] = 0; pos[3] = 1; CU_ASSERT ( tixiGetArrayValue(values, sizes, pos, dims) == 2.0 );   // [0][0][0][1]
    pos[0] = 0; pos[1] = 0; pos[2] = 1; pos[3] = 1; CU_ASSERT ( tixiGetArrayValue(values, sizes, pos, dims) == 12. );   // [0][0][1][1]
    pos[0] = 0; pos[1] = 1; pos[2] = 1; pos[3] = 0; CU_ASSERT ( tixiGetArrayValue(values, sizes, pos, dims) == 201 );   // [0][1][1][0]

    // clean up
    free(values);

}

void tixiArrayTestsNegative( void ){

    // define vars
    int dims, dim = 0;
    int pars,par = 0;
    int arraySize = 4;
    int sizes[] = {0, 0, 0, 0};
    int pos[] = {0, 0, 0, 0};
    double *values = NULL;
    char *names[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

    // check number of dimensions
    CU_ASSERT ( tixiGetArrayDimensions(123, xPath, &dims) == INVALID_HANDLE );
    CU_ASSERT ( tixiGetArrayDimensions(documentHandle, invalidXPath, &dims) == INVALID_XPATH );
    CU_ASSERT ( tixiGetArrayDimensions(documentHandle, wrongXPath, &dims) == ELEMENT_NOT_FOUND );

    CU_ASSERT ( tixiGetArrayDimensionSizes(123, xPath, sizes, &arraySize) == INVALID_HANDLE );
    CU_ASSERT ( tixiGetArrayDimensionSizes(documentHandle, invalidXPath, sizes, &arraySize) == INVALID_XPATH );
    CU_ASSERT ( tixiGetArrayDimensionSizes(documentHandle, wrongXPath, sizes, &arraySize) == ELEMENT_NOT_FOUND );

    CU_ASSERT ( tixiGetArrayDimensionNames(123, xPath, names) == INVALID_HANDLE );
    CU_ASSERT ( tixiGetArrayDimensionNames(documentHandle, invalidXPath, names) == INVALID_XPATH );
    CU_ASSERT ( tixiGetArrayDimensionNames(documentHandle, wrongXPath, names) == ELEMENT_NOT_FOUND );

    values = (double *) malloc(sizes[4] * sizeof(double));    // get values temporary array
    CU_ASSERT (tixiGetArrayDimensionValues(123, xPath, dim, values) == INVALID_HANDLE );
    CU_ASSERT (tixiGetArrayDimensionValues(documentHandle, invalidXPath, dim, values) == INVALID_XPATH );
    CU_ASSERT (tixiGetArrayDimensionValues(documentHandle, wrongXPath, dim, values) == ELEMENT_NOT_FOUND );
    CU_ASSERT (tixiGetArrayDimensionValues(documentHandle, xPath, 5, values) == ELEMENT_NOT_FOUND );

    CU_ASSERT ( tixiGetArray(123, xPath, "cmz", values) == INVALID_HANDLE );
    CU_ASSERT ( tixiGetArray(documentHandle, invalidXPath, "cmz", values) == INVALID_XPATH );
    CU_ASSERT ( tixiGetArray(documentHandle, wrongXPath, "cmz", values) == ELEMENT_NOT_FOUND );
    CU_ASSERT ( tixiGetArray(documentHandle, xPath, "abc", values) == ATTRIBUTE_NOT_FOUND );
    CU_ASSERT ( tixiGetArray(documentHandle, xPath, "def", values) == ATTRIBUTE_NOT_FOUND );

    free(values);

}
