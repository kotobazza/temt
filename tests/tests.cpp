#include <gtest/gtest.h>
#include "FileManip.hpp"
#include "DiskInfo.hpp"

TEST(FileManipulation, ClearNonRelativePathTest) {
    std::string path{"../../hello"};
    EXPECT_EQ(temt::FileManip::clearNonRelativePath(path), "hello");
}
