/*
 * 
 */
//#include "this/package/foo.h"
#include "gtest/gtest.h"
#include "tixi.h"

#ifdef HAVE_VLD
#include <vld.h>
#endif

// make tixi quiet
void tixiSilentMessage(MessageType , const char *, ...){}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  tixiSetPrintMsgFunc(tixiSilentMessage);
  int ret =  RUN_ALL_TESTS();
  tixiCleanup();
  return ret;
}
