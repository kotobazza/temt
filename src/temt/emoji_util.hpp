#pragma once
#include <string>
#include "FileManip.hpp"

namespace temt {
namespace emoji {
std::string emojiedFileName(FileManip::FileInfo file) {
    switch (file.type) {
        case FileManip::FileType::Directory:
            return "📁 " + file.path;
        case FileManip::FileType::Character:
            return "📝 " + file.path;
        default:
            return "❓ " + file.path;
    }
}
}  // namespace emoji
}  // namespace temt
