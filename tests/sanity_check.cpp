/*
* Copyright (C) 2015 German Aerospace Center (DLR/SC)
*
* Created: 2012-10-08 Markus Litz <Markus.Litz@dlr.de>
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

TEST(common_checks, sanity_check) // Declares a test named "sanity_check"
{
    ASSERT_TRUE(true); // We certainly hope that true is true
    EXPECT_EQ(2,1+1); // The value 1+1 should equal 2
}
