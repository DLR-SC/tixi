/*
* Copyright (C) 2007-2012 German Aerospace Center (DLR/SC)
*
* Created: 2013-05-14 Martin Siggel <Martin.Siggel@dlr.de>
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

#include "tixiUtils.h"

/**
    @test Tests for addAttribute routines.
*/

static TixiDocumentHandle documentHandle = -1;

class UtilsTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }
};

TEST_F(UtilsTest, string_stripLeft){
    const char * mystring = "    my new string";
    char * newstring = NULL;
    newstring = string_stripLeft(mystring, 4);
    ASSERT_STREQ("my new string", newstring);
    free(newstring);
}

TEST_F(UtilsTest, string_stripLeft_error){

    const char * mystring = " string";
    const char * newstring = string_stripLeft(mystring, 8);
    ASSERT_EQ(NULL,newstring);
}

TEST_F(UtilsTest, my_strncasecmp){
    const char * a = "string1";
    const char * b = "string1";
    const char * c = "sTring1";
    const char * d = "sTring2";

    ASSERT_EQ(0, my_strncasecmp(a, b, 7));
    ASSERT_EQ(0, my_strncasecmp(a, c, 7));
    ASSERT_EQ(0, my_strncasecmp(a, c, 4));

    ASSERT_LT(my_strncasecmp(a, d, 7), 0);

}

TEST_F(UtilsTest, my_strncasecmp_invalid ){
    const char * a = "string1";
    const char * b = "string1";

    ASSERT_EQ(0, my_strncasecmp(a,b,0));

    ASSERT_EQ(0, my_strncasecmp(a,b,10));
}

TEST_F(UtilsTest, create_local_directory){

    const char * path = "tmp/";

    ASSERT_EQ(SUCCESS,create_local_directory(path));

    rmdir(path);

}
TEST_F(UtilsTest, create_local_directory_failed){

    const char * path = "tmp2/temp2";

    ASSERT_EQ(FAILED,create_local_directory(path));

}

TEST_F(UtilsTest, string_startsWith){
    const char * a = "string1";
    const char * b = "str";
    const char * c = "nosubst";


    ASSERT_EQ( 0, string_startsWith(a, b));
    ASSERT_EQ(-1, string_startsWith(a, c));
}

TEST_F(UtilsTest, isPathRelative){
    // check relative path
    ASSERT_EQ(0, isPathRelative("file://tmp"));

    // check absolute path
    ASSERT_NE(0, isPathRelative("file:///usr/local"));

    ASSERT_NE(0, isPathRelative("file:usr/local"));

    ASSERT_NE(0, isPathRelative("usr/local"));
}

TEST_F(UtilsTest, isPathRelative_invalidPath){
    ASSERT_NE(0, isPathRelative(NULL));
}
