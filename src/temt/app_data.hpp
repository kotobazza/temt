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

    std::shared_ptr<spdlog::logger> file_logger_ = spdlog::get("file_logger");

    int doubleClickDelay_parameter = 500;  // create class for settings

    int mainAppSplitLength_ = 40;  // parameter defined by settings and terminal size

    bool modalShowOptionsMenu = false;

    int& usingDirectorySelectedIndex() { return usingDirectorySelected_; }
    void setUsingDirectorySelectedIndex(int idx) { usingDirectorySelected_ = idx; }
    void NavigateToPath(std::string_view path);
    void AddListener(std::function<void()> listener) { listeners.push_back(listener); }
    void toggleFileBrowser() {
        int t = mainAppSplitLength_;
        mainAppSplitLength_ = mainAppSplitLengthSaved_;
        mainAppSplitLengthSaved_ = t;
    }

    void toggleOptionsMenu(){
        modalShowOptionsMenu = !modalShowOptionsMenu;
    }

   private:
    int usingDirectorySelected_ = 0;
    int mainAppSplitLengthSaved_ = 0;
    std::vector<std::function<void()>> listeners{};
    void notify();
};
}  // namespace temt