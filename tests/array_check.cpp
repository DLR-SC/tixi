/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2010-08-13 Markus Litz <Markus.Litz@dlr.de>
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


/**
    @test Tests for Array routines.
*/

static TixiDocumentHandle documentHandle = -1;
static TixiDocumentHandle tmpHandle = -1;
static const char* xPath = "/root/aeroPerformanceMap";
static const char* invalidXPath = "][";
static const char* wrongXPath = "/root/aEroPerformanceMap";

class ArrayTests : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    const char* xmlFilename = "TestData/arraytests.xml";
    ASSERT_EQ  (SUCCESS, tixiCloseAllDocuments());
    ASSERT_TRUE(tixiOpenDocument( xmlFilename, &documentHandle ) == SUCCESS);
  }

  // virtual void TearDown() {}
};

TEST_F(ArrayTests, array_test_positive)
{
  // define vars
  int dims = 0;
  int dim = 0;
  int pars = 0;
  int par = 0;
  int arraySize = 0;
  int sizes[] = {0, 0, 0, 0};
  int pos[] = {0, 0, 0, 0};
  double* values = NULL;
  char** names = NULL;

  // check number of dimensions
  ASSERT_TRUE ( tixiGetArrayDimensions(documentHandle, xPath, &dims) == SUCCESS );
  ASSERT_TRUE ( dims == 4 );   // four dimensions with each a different number of values

  // check sizes of dimensions and product of them
  ASSERT_TRUE ( tixiGetArrayDimensionSizes(documentHandle, xPath, sizes, &arraySize) == SUCCESS );
  // try again to ensure all pointers are still there!
  ASSERT_TRUE ( tixiGetArrayDimensionSizes(documentHandle, xPath, sizes, &arraySize) == SUCCESS );
  ASSERT_TRUE ( (sizes[0] == 1) && (sizes[1] == 2) && (sizes[2] == 3) && (sizes[3] == 8) );
  ASSERT_TRUE ( arraySize == 1 * 2 * 3 * 8 );    // check product of sizes

  // check dimension values (meanings)
  for (dim = 0; dim < dims; dim ++) {
    values = (double*) malloc(sizes[dim] * sizeof(double));     // get values temporary array
    ASSERT_TRUE (tixiGetArrayDimensionValues(documentHandle, xPath, dim, values) == SUCCESS );
    ASSERT_TRUE (
          ( (dim == 0) && (values[0] == 1.0) )
      ||  ( (dim == 1) && (values[1] == 2.0) )
      ||  ( (dim == 2) && (values[2] == 10.) )
      ||  ( (dim == 3) && (values[0] == 1.0) && (values[7] == 8.0) )
    );
    free(values);
  }

  // check dimension names
  names = (char**) malloc(dims * sizeof(char*));
  ASSERT_TRUE ( tixiGetArrayDimensionNames(documentHandle, xPath, names) == SUCCESS );
  ASSERT_TRUE ( strcmp(names[0], "machNumber") == 0);
  ASSERT_TRUE ( strcmp(names[1], "reynoldsNumber") == 0);
  ASSERT_TRUE ( strcmp(names[2], "angleOfYaw") == 0);
  ASSERT_TRUE ( strcmp(names[3], "angleOfAttack") == 0);
  for (dim = 0; dim < dims; dim ++) {
    free(names[dim]);
  }
  free(names);

  // check number of parameter
  ASSERT_TRUE ( tixiGetArrayParameters(documentHandle, xPath, &pars) == SUCCESS );
  ASSERT_TRUE ( pars == 7 );    // one invalid and one not counting (mapType="Array")

  // check parameter names
  names = (char**) malloc(pars * sizeof(char*));

  ASSERT_TRUE ( tixiGetArrayParameterNames(documentHandle, xPath, names) == SUCCESS);
  ASSERT_TRUE ( strcmp(names[0], "cfx") == 0 );
  ASSERT_TRUE ( strcmp(names[1], "cfy") == 0 );
  ASSERT_TRUE ( strcmp(names[2], "cfz") == 0 );
  ASSERT_TRUE ( strcmp(names[3], "cmx") == 0 );
  ASSERT_TRUE ( strcmp(names[4], "cmy") == 0 );
  ASSERT_TRUE ( strcmp(names[5], "cmz") == 0 );
  for (par = 0; par < pars; par ++) {
    free(names[par]);
  }
  free(names);

  // read one array
  ASSERT_TRUE ( tixiGetArray(documentHandle, xPath, "cmz", arraySize, &values) == SUCCESS );    // get array of subkey
  ASSERT_TRUE ( values[0]  == 1.0 );      // [0][0][0][0]
  ASSERT_TRUE ( values[1]  == 2.0 );      // [0][0][0][1]
  ASSERT_TRUE ( values[16] == 101.0 );    // [0][0][2][1]
  ASSERT_TRUE ( values[23] == 108.0 );    // [0][0][2][7]
  ASSERT_TRUE ( values[32] == 201.0 );    // [0][1][1][0]
  ASSERT_TRUE ( values[47] == 216.0 );    // [0][1][2][7]
  ASSERT_TRUE ( values[0 * sizes[1] * sizes[2] * sizes[3]
                     + 1 * sizes[2] * sizes[3]
                     + 0 * sizes[3]
                     + 0
                     - 1] == 108.0);    // [0][1][0][0]
  ASSERT_TRUE ( values[0 * sizes[1] * sizes[2] * sizes[3]
                     + 1 * sizes[2] * sizes[3]
                     + 1 * sizes[3]
                     + 3
                     - 1] == 203.0);    // [0][1][1][3]

  // test array value fetching
  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 0;
  pos[3] = 0;
  ASSERT_TRUE ( tixiGetArrayValue(values, sizes, pos, dims) == 1.0 );   // [0][0][0][0]
  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 0;
  pos[3] = 1;
  ASSERT_TRUE ( tixiGetArrayValue(values, sizes, pos, dims) == 2.0 );   // [0][0][0][1]
  pos[0] = 0;
  pos[1] = 0;
  pos[2] = 1;
  pos[3] = 1;
  ASSERT_TRUE ( tixiGetArrayValue(values, sizes, pos, dims) == 12. );   // [0][0][1][1]
  pos[0] = 0;
  pos[1] = 1;
  pos[2] = 1;
  pos[3] = 0;
  ASSERT_TRUE ( tixiGetArrayValue(values, sizes, pos, dims) == 201 );   // [0][1][1][0]
}

TEST_F(ArrayTests, array_test_negative)
{
  // define vars
  int dims, dim = 0;
  int pars,par = 0;
  int arraySize = 4;
  int sizes[] = {0, 0, 0, 0};
  int pos[] = {0, 0, 0, 0};
  double* values = NULL;
  char* names[6] = {NULL, NULL, NULL, NULL, NULL, NULL};

  // check number of dimensions
  ASSERT_TRUE ( tixiGetArrayDimensions(123, xPath, &dims) == INVALID_HANDLE );
  ASSERT_TRUE ( tixiGetArrayDimensions(documentHandle, invalidXPath, &dims) == INVALID_XPATH );
  ASSERT_TRUE ( tixiGetArrayDimensions(documentHandle, wrongXPath, &dims) == ELEMENT_NOT_FOUND );

  ASSERT_TRUE ( tixiGetArrayDimensionSizes(123, xPath, sizes, &arraySize) == INVALID_HANDLE );
  ASSERT_TRUE ( tixiGetArrayDimensionSizes(documentHandle, invalidXPath, sizes, &arraySize) == INVALID_XPATH );
  ASSERT_TRUE ( tixiGetArrayDimensionSizes(documentHandle, wrongXPath, sizes, &arraySize) == ELEMENT_NOT_FOUND );

  ASSERT_TRUE ( tixiGetArrayDimensionNames(123, xPath, names) == INVALID_HANDLE );
  ASSERT_TRUE ( tixiGetArrayDimensionNames(documentHandle, invalidXPath, names) == INVALID_XPATH );
  ASSERT_TRUE ( tixiGetArrayDimensionNames(documentHandle, wrongXPath, names) == ELEMENT_NOT_FOUND );

  ASSERT_TRUE (tixiGetArrayDimensionValues(123, xPath, dim, values) == INVALID_HANDLE );
  ASSERT_TRUE (tixiGetArrayDimensionValues(documentHandle, invalidXPath, dim, values) == INVALID_XPATH );
  ASSERT_TRUE (tixiGetArrayDimensionValues(documentHandle, wrongXPath, dim, values) == ELEMENT_NOT_FOUND );
  ASSERT_TRUE (tixiGetArrayDimensionValues(documentHandle, xPath, 5, values) == ELEMENT_NOT_FOUND );

  ASSERT_TRUE ( tixiGetArray(123, xPath, "cmz", arraySize, &values) == INVALID_HANDLE );
  ASSERT_TRUE ( tixiGetArray(documentHandle, invalidXPath, "cmz", arraySize, &values) == INVALID_XPATH );
  ASSERT_TRUE ( tixiGetArray(documentHandle, wrongXPath, "cmz", arraySize, &values) == ELEMENT_NOT_FOUND );
  ASSERT_TRUE ( tixiGetArray(documentHandle, xPath, "abc", arraySize, &values) == ATTRIBUTE_NOT_FOUND );
  ASSERT_TRUE ( tixiGetArray(documentHandle, xPath, "def", arraySize, &values) == ATTRIBUTE_NOT_FOUND );

  //check getArray, cmx is too short
  ASSERT_TRUE ( tixiGetArrayDimensionSizes(documentHandle, xPath, sizes, &arraySize) == SUCCESS );
  ASSERT_EQ   ( NON_MATCHING_SIZE, tixiGetArray(documentHandle, xPath, "cmx", arraySize, &values));
  ASSERT_EQ   ( NON_MATCHING_SIZE, tixiGetArray(documentHandle, xPath, "cmz", 0, &values));
  ASSERT_EQ   ( NON_MATCHING_SIZE, tixiGetArray(documentHandle, xPath, "cmz", 1, &values));
  ASSERT_EQ   ( NON_MATCHING_SIZE, tixiGetArray(documentHandle, xPath, "cmz", 49, &values));
  ASSERT_EQ   ( NON_MATCHING_SIZE, tixiGetArray(documentHandle, xPath, "cmz", 47, &values));
}
