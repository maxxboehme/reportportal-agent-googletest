#include <gtest/gtest.h>

constexpr uint32_t factorial(uint32_t number)
{
    return number <= 1 ? number : factorial(number - 1) * number;
}

TEST(FactorialTest, HandlesZeroInput) {
    // We want this to fail to see what the report looks like.
    EXPECT_EQ(factorial(0), 1);
}

TEST(FactorialTest, HandlesPositiveInput) {
    EXPECT_EQ(factorial(1), 1);
    EXPECT_EQ(factorial(2), 2);
    EXPECT_EQ(factorial(3), 6);
    EXPECT_EQ(factorial(10), 3628800);
}

TEST(DisabledTest, DISABLED_ValidateDisabled) {
    EXPECT_EQ(1, 2);
}

TEST(SkipTest, ValidateSkipping) {
    GTEST_SKIP();

    EXPECT_EQ(1, 2);
}

class FixtureTest : public ::testing::Test
{
    public:
        void SetUp() override {
        }

        uint32_t fortytwo() {
            return 42;
        }
};

TEST_F(FixtureTest, TestUsingFixture) {
    EXPECT_EQ(this->fortytwo(), 42);
}
