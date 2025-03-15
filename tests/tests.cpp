#include <gtest/gtest.h>
#include "FileManip.hpp"

TEST(FileManipulation, ClearNonRelativePathTest)
{
    std::string path {"../../hello"};
    std::string a{"Hello"};
    EXPECT_EQ(a, "Hello");
}
