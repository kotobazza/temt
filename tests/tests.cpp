#include <gtest/gtest.h>
#include "FileManip.hpp"

TEST(FileManipulation, ClearNonRelativePathTest) {
    std::string path{"../../hello"};
    EXPECT_EQ(temt::FileManip::clearNonRelativePath(path), "hello");
}
