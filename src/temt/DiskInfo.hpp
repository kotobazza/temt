#pragma once

#include <string>
#include <vector>

namespace temt {

namespace DiskInfo {
struct StoragePointInfo {
    std::string deviceName{};
    std::string mountPoint{};
    std::string fileSystem{};
    unsigned long int totalMemory{0};
    unsigned long int freeMemory{0};
};

std::vector<StoragePointInfo> getMountedPoints();

}  // namespace DiskInfo
}  // namespace temt