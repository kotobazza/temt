#include <gtest/gtest.h>
#include "LogInitializer.hpp"
#include "ArchiveManip.hpp"
#include "FileManip.hpp"
#include "CharProcessing.hpp"
#include "DiskInfo.hpp"

TEST(HelloWorld, GTestUsage) {
    int i = 0;
    EXPECT_EQ(i, 0);
}


TEST(FileManipulation, ClearNonRelativePathTest) {
    std::string path{"../../hello"};
    EXPECT_EQ(temt::FileManip::clearNonRelativePath(path), "hello");
}
