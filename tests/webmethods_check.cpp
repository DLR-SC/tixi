#include "test.h"
#include "webMethods.h"

TEST(WebMethods, curlGetFileToLocalDisk){
    // create some local dummy file
    FILE * f = fopen("mytmpfile.sh", "w");
    fprintf(f, "hello world\n");
    fclose(f);

    int res = curlGetFileToLocalDisk("file://mytmpfile.sh", "tmp.sh");
    ASSERT_EQ(0, res);
    remove ("tmp.sh");
    remove ("mytmpfile.sh");
}

TEST(WebMethods, curlGetFileToLocalDisk_invalidPath){
    int res = curlGetFileToLocalDisk("file://invalidfile.sh", "tmp.sh");
    ASSERT_LT(res, 0);
    remove ("tmp.sh");
}

TEST(WebMethods, curlGetURLInMemory){
    // create some local dummy file
    FILE * f = fopen("mytmpfile.sh", "w");
    fprintf(f, "hello world");
    fclose(f);

    char * text = curlGetURLInMemory("file://mytmpfile.sh");
    ASSERT_STREQ("hello world", text);

    remove ("mytmpfile.sh");
}

TEST(WebMethods, myrealloc)
{
    char * a = (char*) malloc(sizeof(char) * 10);
    char * b = NULL;

    a = (char*) myrealloc((void*)a, 20);
    ASSERT_TRUE(a != NULL);

    b = (char*) myrealloc((void*)b, 20);
    ASSERT_TRUE(b != NULL);
}

