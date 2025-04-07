#include <iostream>
#include <string>
#include <vector>

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#elif defined(PLATFORM_LINUX)
#include <mntent.h>
#include <stdio.h>
#include <sys/statvfs.h>
#endif

#include "DiskInfo.hpp"
#include "spdlog/spdlog.h"

namespace temt {
namespace DiskInfo {

std::vector<StoragePointInfo> getMountedPoints() {
    std::vector<StoragePointInfo> records;

#ifdef PLATFORM_WINDOWS
    DWORD drives = GetLogicalDrives();
    for (char drive = 'A'; drive <= 'Z'; ++drive) {
        if (drives & (1 << (drive - 'A'))) {
            std::string drivePath = std::string(1, drive) + ":\\";
            DWORD sectorsPerCluster, bytesPerSector, numberOfFreeClusters, totalNumberOfClusters;

            if (GetDiskFreeSpaceA(drivePath.c_str(), &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters,
                                  &totalNumberOfClusters)) {
                unsigned long long total =
                    static_cast<unsigned long long>(totalNumberOfClusters) * sectorsPerCluster * bytesPerSector;
                unsigned long long free =
                    static_cast<unsigned long long>(numberOfFreeClusters) * sectorsPerCluster * bytesPerSector;

                char fileSystemName[MAX_PATH];
                DWORD fileSystemFlags;
                if (GetVolumeInformationA(drivePath.c_str(), nullptr, 0, nullptr, nullptr, &fileSystemFlags,
                                          fileSystemName, sizeof(fileSystemName))) {
                    records.push_back({fileSystemName, drivePath, fileSystemName, total, free});
                }
            } else {
                auto logger = spdlog::get("file_logger");
                logger->critical("DiskInfo(Windows): Error getting disk space for disk {}. Error: ", drivePath, GetLastError() );
            }
        }
    }
#elif defined(PLATFORM_LINUX)
    FILE* mounts = setmntent("/proc/mounts", "r");
    auto logger = spdlog::get("file_logger");
    if (mounts == nullptr) {
        logger->critical("DiskInfo(Linux): Error getting mount points");
        return {};
    }

    struct mntent* mnt;
    while ((mnt = getmntent(mounts)) != nullptr) {
        struct statvfs stat;
        if (statvfs(mnt->mnt_dir, &stat) == 0) {
            unsigned long long total = stat.f_blocks * stat.f_frsize;
            unsigned long long free = stat.f_bfree * stat.f_frsize;

            records.push_back({mnt->mnt_fsname, mnt->mnt_dir, mnt->mnt_type, total, free});
        } else {
            logger->critical("DiskInfo(Linux): Error getting mount point info (statvfs)");
        }
    }
#endif

    return records;
}

}  // namespace DiskInfo
}  // namespace temt