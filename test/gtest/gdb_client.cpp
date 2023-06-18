#include "gtest/gtest.h"

#include "BaseClient.h"

// Tests factorial of 0.
TEST(succesful, test) {
    EXPECT_EQ(0, 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
