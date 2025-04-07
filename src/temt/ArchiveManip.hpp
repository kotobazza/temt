#pragma once
#include <string>
#include <vector>

#include "FileManip.hpp"

namespace temt {
namespace ArchiveManip {
struct ArchiveReadingResults {
    FileManip::ActionState state;
    std::vector<std::string> entries;
};

ArchiveReadingResults readArchiveEntries(std::string_view path);
FileManip::ActionState createNewZipArchive(std::vector<FileManip::FileInfo> entries, std::string_view path);
FileManip::ActionState unzipArchive(std::string_view path, std::string_view pathToExtract);

}  // namespace ArchiveManip
}  // namespace temt