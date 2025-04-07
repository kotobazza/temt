#include <gtest/gtest.h>
#include "FileManip.hpp"

TEST(HelloWorld, GTestUsage) {
    int i = 0;
    EXPECT_EQ(i, 0);
}


TEST(FileManipulation, ClearNonRelativePathTest) {
    std::string path{"../../hello"};
    EXPECT_EQ(temt::FileManip::clearNonRelativePath(path), "hello");
}
