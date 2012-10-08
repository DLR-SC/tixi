/*
 * This is an example unit test that doesn't really do anything useful.
 * It is here as a reference for you when creating additional unit tests.
 */
 
#include "test.h" // Brings in the GTest framework
 
TEST(common_checks, sanity_check) // Declares a test named "sanity_check"
{
    ASSERT_TRUE(true); // We certainly hope that true is true
    EXPECT_EQ(2,1+1); // The value 1+1 should equal 2
}
