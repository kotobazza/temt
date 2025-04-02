#pragma once
#include <string>
#include <vector>
#include "spdlog/spdlog.h"

#include "FileManip.hpp"

namespace temt {
class AppData {
   public:
    AppData(std::string_view exec_path) : exec_path_(exec_path) { NavigateToPath(exec_path_); }
    std::string exec_path_;
    std::string current_path_;
    std::vector<temt::FileManip::FileInfo> usingDirectoryEntries_{};
    int usingDirectoriesSelected_ = 0;
    bool isBrowserPanelHidden_ = false;

    void NavigateToPath(std::string_view path);
    void add_listener(std::function<void()> listener) { listeners.push_back(listener); }

   private:
    std::vector<std::function<void()>> listeners{};
    void notify();
};
}  // namespace temt