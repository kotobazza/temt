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
    int usingDirectorySelected_ = 0;
    bool isFleBrowserPanelHidden_ = false;
    std::shared_ptr<spdlog::logger> file_logger_ = spdlog::get("file_logger");

    void NavigateToPath(std::string_view path);
    void AddListener(std::function<void()> listener) { listeners.push_back(listener); }

   private:
    std::vector<std::function<void()>> listeners{};
    void notify();
};
}  // namespace temt