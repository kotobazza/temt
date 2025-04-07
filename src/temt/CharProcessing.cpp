#include <fstream>
#include "spdlog/spdlog.h"

#include "CharProcessing.hpp"

namespace temt {
namespace CharProcess {

ReadFileOutput readFromCharFile(std::string_view path) {
    if (!FileManip::isExistingPath(path))
        return {FileManip::ActionState::NotExists, std::string{}};

    std::ifstream f{path.data()};

    if (!f.is_open()) {
        auto logger = spdlog::get("file_logger");
        logger->critical("Failed to open path on read: {}", path.data());
        return {FileManip::ActionState::Unavailable, {}};
    }

    std::string contents;
    f.seekg(0, std::ios::end);
    contents.resize(f.tellg());
    f.seekg(0, std::ios::beg);
    f.read(&contents[0], contents.size());
    f.close();

    return {FileManip::ActionState::Done, contents};
}

FileManip::ActionState appendIntoCharFile(std::string_view path, std::string_view text) {
    if (!FileManip::isExistingPath(path))
        return {FileManip::ActionState::NotExists};

    std::ofstream f(std::string(path.data()), std::ios::app);

    if (!f.is_open()) {
        auto logger = spdlog::get("file_logger");
        logger->critical("Failed to open path on write: {}", path.data());
        return FileManip::ActionState::Unavailable;
    }

    f << text;

    if (f.fail())
    {
        auto logger = spdlog::get("file_logger");
        logger->critical("Failed to write text on path: {}", path.data());
        return FileManip::ActionState::Failed;
    }

    return FileManip::ActionState::Done;
}

FileManip::ActionState writeIntoCharFile(std::string_view path, std::string_view text) {
    if (!FileManip::isExistingPath(path))
        return {FileManip::ActionState::NotExists};

    std::ofstream f{std::string(path.data()), std::ios::trunc};

    if (!f.is_open()) {
        auto logger = spdlog::get("file_logger");
        logger->critical("Failed to open path on write: {}", path.data());
        return FileManip::ActionState::Unavailable;
    }

    f << text;
    f.close();

    if (f.fail())
    {
        auto logger = spdlog::get("file_logger");
        logger->critical("Failed to write text on path: {}", path.data());
        return FileManip::ActionState::Failed;
    }
        

    return FileManip::ActionState::Done;
}

}  // namespace CharProcess
}  // namespace temt