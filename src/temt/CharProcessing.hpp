#pragma once

#include "FileManip.hpp"

namespace temt {
namespace CharProcess {
struct ReadFileOutput {
    FileManip::ActionState state;
    std::string content;
};

ReadFileOutput readFromCharFile(std::string_view path);
FileManip::ActionState writeIntoCharFile(std::string_view path, std::string_view text);
FileManip::ActionState appendIntoCharFile(std::string_view path, std::string_view text);
}  // namespace CharProcess
}  // namespace temt