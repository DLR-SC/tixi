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

#include "test.h"
#include "webMethods.h"

TEST(WebMethods, curlGetFileToLocalDisk)
{
  // create some local dummy file
  FILE* f = fopen("mytmpfile.sh", "w");
  fprintf(f, "hello world\n");
  fclose(f);

  int res = curlGetFileToLocalDisk("file://mytmpfile.sh", "tmp.sh");
  ASSERT_EQ(0, res);
  remove ("tmp.sh");
  remove ("mytmpfile.sh");
}

TEST(WebMethods, curlGetFileToLocalDisk_invalidPath)
{
  int res = curlGetFileToLocalDisk("file://invalidfile.sh", "tmp.sh");
  ASSERT_LT(res, 0);
  remove ("tmp.sh");
}

TEST(WebMethods, curlGetURLInMemory)
{
  // create some local dummy file
  FILE* f = fopen("mytmpfile.sh", "w");
  fprintf(f, "hello world");
  fclose(f);

  char* text = curlGetURLInMemory("file://mytmpfile.sh");
  ASSERT_STREQ("hello world", text);

  remove ("mytmpfile.sh");
  free(text);
}

TEST(WebMethods, myrealloc)
{
  char* a = (char*) malloc(sizeof(char) * 10);
  char* b = NULL;

  a = (char*) myrealloc((void*)a, 20);
  ASSERT_TRUE(a != NULL);

  b = (char*) myrealloc((void*)b, 20);
  ASSERT_TRUE(b != NULL);
  free(a);
  free(b);
}

