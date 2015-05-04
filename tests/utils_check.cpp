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

TEST_F(UtilsTest, isLocalPathRelative){
    ASSERT_EQ(0, isLocalPathRelative("./test.txt"));
    
    ASSERT_EQ(0, isLocalPathRelative("test.txt"));
    
    ASSERT_EQ(0, isLocalPathRelative("../test.txt"));
    
    // check absolute paths
#ifdef WIN32
    ASSERT_NE(0, isLocalPathRelative("d:/data/test.txt"));
#else
    ASSERT_NE(0, isLocalPathRelative("/usr/bin/test.txt"));
#endif
}

TEST_F(UtilsTest, isURIPath) {
    ASSERT_EQ(0, isURIPath("file://test.txt"));
    ASSERT_EQ(0, isURIPath("file:///data/test.txt"));
    ASSERT_EQ(0, isURIPath("file:///c:/data/test.txt"));
    ASSERT_EQ(0, isURIPath("https://www.data.com/test.txt"));
    ASSERT_EQ(0, isURIPath("http://www.data.com/test.txt"));
    ASSERT_EQ(0, isURIPath("ftp://www.data.com/test.txt"));
    ASSERT_EQ(0, isURIPath("ssh://www.data.com/test.txt"));
    
    ASSERT_EQ(1, isURIPath("c:/data.txt"));
    ASSERT_EQ(1, isURIPath("./data.txt"));
    ASSERT_EQ(1, isURIPath("../data.txt"));
    ASSERT_EQ(1, isURIPath("data/data.txt"));
    ASSERT_EQ(1, isURIPath("/home/user/data.txt"));
}

TEST_F(UtilsTest, isPathRelative_invalidPath){
    ASSERT_NE(0, isPathRelative(NULL));
}

TEST_F(UtilsTest, uriToLocalPath) {
    char* result = NULL;

    // a relative path
    result = uriToLocalPath("file://data/test.txt");
    ASSERT_STREQ("data/test.txt", result);
    free(result);

    // an absolute path
#ifdef _WIN32
    result = uriToLocalPath("file:///c:/data/test.txt");
    ASSERT_STREQ("c:/data/test.txt", result);
#else
    result = uriToLocalPath("file:///data/test.txt");
    ASSERT_STREQ("/data/test.txt", result);
#endif
    free(result);

    // an non local URI
    result = uriToLocalPath("ssh:///c:/data/test.txt");
    ASSERT_EQ(NULL, result);
}

TEST_F(UtilsTest, loadFileToString) {
    const char* expected = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<root/>\n";
    char* string = loadFileToString("TestData/out.xml");
    ASSERT_STREQ(expected, string);
    free(string);
}
