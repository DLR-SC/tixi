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
#include "tixiInternal.h"


/**
    @test Tests for vector routines.
*/

static TixiDocumentHandle documentHandleGet = -1;
static TixiDocumentHandle documentHandleAdd = -1;
static TixiDocumentHandle tmpHandle = -1;

class VectorTests : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    const char* xmlFilenameGet = "TestData/vectorcount.xml";
    const char* xmlFilenameAdd = "TestData/vectoradd.xml";
    ASSERT_TRUE( tixiOpenDocument( xmlFilenameGet, &documentHandleGet ) == SUCCESS);
    ASSERT_TRUE( tixiOpenDocument( xmlFilenameAdd, &documentHandleAdd  ) == SUCCESS);
  }

  virtual void TearDown()
  {
    ASSERT_TRUE ( tixiCloseDocument( documentHandleGet ) == SUCCESS );
    ASSERT_TRUE ( tixiCloseDocument( documentHandleAdd ) == SUCCESS );
  }
};

TEST_F(VectorTests, tixiVectorGetTests)
{
  int count = 0;
  double* allPoints = NULL;

  // check sizes
  ASSERT_TRUE ( tixiGetVectorSize(documentHandleGet, "/a/aeroPerformanceMap/cfx", &count) == SUCCESS );
  ASSERT_TRUE ( count == 32 );
  ASSERT_TRUE ( tixiGetVectorSize(documentHandleGet, "/a/aeroPerformanceMap/cfy", &count) == SUCCESS );
  ASSERT_TRUE ( tixiGetVectorSize(documentHandleGet, "/a/aeroPerformanceMap/cfz", &count) == SUCCESS );
  ASSERT_TRUE ( count == 2 );

  // check values returned
  count = 10;
  ASSERT_TRUE ( tixiGetFloatVector(documentHandleGet, "/a/aeroPerformanceMap/cfy", &allPoints, count) == SUCCESS );
  ASSERT_TRUE ( allPoints[0] == 1.0 );
  ASSERT_TRUE ( allPoints[7] == 8.0 );

  // check "cannot parse number"
  ASSERT_TRUE ( tixiGetFloatVector(documentHandleGet, "/a/aeroPerformanceMap/cfx", &allPoints, count) == NO_NUMBER );
}

TEST_F(VectorTests, tixiVectorAddTests)
{
  int count = 0;
  double points[10] = {1, 4, 5.8, 77.0, 5, 6, 7, 8, 9, 10};
  double* newPoints = NULL;
  const char* xmlOutName = "x.xml";
  count = 3;

  // write parts of the array to an intermediate file
  ASSERT_TRUE ( tixiAddFloatVector(documentHandleAdd, "/a", "test", points, count, "%g") == SUCCESS );
  ASSERT_TRUE ( tixiSaveDocument(documentHandleAdd, xmlOutName) == SUCCESS );

  // read in again and check values
  ASSERT_TRUE ( tixiOpenDocument( xmlOutName, &tmpHandle ) == SUCCESS );
  ASSERT_TRUE ( tixiGetFloatVector(tmpHandle, "/a/test", &newPoints, count) == SUCCESS );
  ASSERT_TRUE ( (newPoints[0] == 1) && (newPoints[1] == 4) && (newPoints[2] == 5.8) );
  ASSERT_TRUE ( tixiCloseDocument( tmpHandle ) == SUCCESS );
}

TEST_F(VectorTests, tixiVectorAddNullTests)
{
  int count = 0;
  double* points = NULL;
  const char* xmlOutName = "x.xml";

  // write parts of the array to an intermediate file
  ASSERT_TRUE ( tixiAddFloatVector(documentHandleAdd, "/a", "test", points, count, "%g") == SUCCESS );
  ASSERT_TRUE ( tixiSaveDocument(documentHandleAdd, xmlOutName) == SUCCESS );
}

TEST_F(VectorTests, tixiUpdateVectorTests)
{
  int count = 0;
  double points[10] = {1, 4, 5.8, 77.0, 5, 6, 7, 8, 9, 10};
  double pointsUpdated[10] = {2, 8, 11.6, 154.0, 10, 12, 14, 16, 18, 20};
  double* newPoints = NULL;
  count = 3;

  // write parts of the array to an intermediate file
  ASSERT_TRUE ( tixiAddFloatVector(documentHandleAdd, "/a", "test", points, count, "%g") == SUCCESS );

  ASSERT_TRUE ( tixiUpdateFloatVector(documentHandleAdd, "/a/test", pointsUpdated, count, "%f") == SUCCESS);

  // check result
  ASSERT_TRUE ( tixiGetFloatVector(documentHandleAdd, "/a/test", &newPoints, count) == SUCCESS );
  ASSERT_NEAR (2., newPoints[0], 1e-10);
  ASSERT_NEAR (8., newPoints[1], 1e-10);
  ASSERT_NEAR (11.6, newPoints[2], 1e-10);

  // check invalid input data
  ASSERT_TRUE ( tixiUpdateFloatVector(documentHandleAdd, "/a/test_not_there", pointsUpdated, count, "%f") == ELEMENT_NOT_FOUND);
  ASSERT_TRUE ( tixiUpdateFloatVector(-1, "/a/test", pointsUpdated, count, "%f") == INVALID_HANDLE );
  ASSERT_TRUE ( tixiUpdateFloatVector(documentHandleAdd, "/a/test", pointsUpdated, 0, "%f") == FAILED );
}

// Make sure to run in a reasoble time
TEST(Vector, Performance)
{
    TixiDocumentHandle handle;
    ASSERT_EQ(SUCCESS, tixiCreateDocument("root", &handle));

    int n = 40000;

    double* vec = new double[n];

    for (int i = 0; i < n; ++i) {
        vec[i] = (float)i;
    }

    EXPECT_EQ(SUCCESS, tixiAddFloatVector(handle, "/root", "myvec", vec, n, "%f"));

    delete [] vec;

    tixiCloseDocument(handle);
}

TEST(Vector, vectorToString)
{
    int n = 10;
    double* vec = new double[n];
    char* myvecString = NULL;

    for (int i = 0; i < n; ++i) vec[i] = (float)i;

    myvecString = vectorToString(vec, n, "%.2f");

    EXPECT_STREQ("0.00;1.00;2.00;3.00;4.00;5.00;6.00;7.00;8.00;9.00", myvecString);

    free(myvecString);
    delete [] vec;
}
