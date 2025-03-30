#include <chrono>
#include <vector>

#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "spdlog/spdlog.h"

#include "FileManip.hpp"
#include "emoji_util.hpp"
#include "file_browser.hpp"

using namespace ftxui;

class FileBrowserImpl : public ComponentBase {
   public:
    FileBrowserImpl(const std::string_view path) : usingPath_(path.data()) {
        SetBrowserPosition(usingPath_);

        menu_ = Menu(&entriesNames_, &selected_);
        Add(menu_);
    }

    Element OnRender() override final {
        return vbox({text(usingPath_) | border,
                     menu_->Render() | vscroll_indicator | yframe | flex | reflect(menuBox_) | focusPosition(0, selected_+menuBox_.y_min)}) |
               border;
    }

    bool OnEvent(Event event) override final {
        if (event.is_mouse() && event.mouse().button == Mouse::Left) {
            if (event.mouse().motion == Mouse::Pressed && menuBox_.Contain(event.mouse().x, event.mouse().y)) {
                int local_y = event.mouse().y - menuBox_.y_min;

                if (local_y >= 0 && local_y < static_cast<int>(entriesNames_.size())) {
                    auto now = std::chrono::steady_clock::now();
                    auto time_since_last_click =
                        std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime_).count();

                    if (time_since_last_click < 500 && local_y == selected_) {
                        OnDoubleClickEvent(selected_);
                        lastDoubleClicked_ = selected_;
                        
                        return ComponentBase::OnEvent(event);
                    }

                    selected_ = local_y;
                    lastClickTime_ = now;
                    
                }
            }
        }
        return ComponentBase::OnEvent(event);
    }

    bool Focusable() const final { return true; }

   private:
    std::string usingPath_;
    std::vector<std::string> entriesNames_;
    std::vector<temt::FileManip::FileInfo> entries_;
    int selected_ = 0;
    ftxui::Component menu_;
    ftxui::Box menuBox_;
    int lastDoubleClicked_ = 0;
    std::chrono::steady_clock::time_point lastClickTime_;

    void SetBrowserPosition(const std::string_view path) {
        usingPath_ = path.data();

        entries_.clear();
        entriesNames_.clear();
        entries_ = temt::FileManip::readDirectoryFlatEntries(path);

        for (auto ent : entries_) {
            entriesNames_.push_back(temt::emoji::emojiedFileName(ent));
        }
    }

    void OnDoubleClickEvent(const int selected) {
        auto file_logger = spdlog::get("file_logger");
        if (!file_logger) {
            spdlog::critical("no filelogger");
            return;
        }

        if (!(selected >= 0 && selected < static_cast<int>(entries_.size()))) {
            file_logger->critical("Error in selected, {}, {}", selected, static_cast<int>(entries_.size()));
            file_logger->flush();
            return;
        }

        file_logger->info("Double clicked value: {}", entries_[selected].path);
        file_logger->flush();

        auto entry = entries_[selected];

        std::string newPath = temt::FileManip::assemblePath(entry.parentDirectory, entry.path);  // TODO: true path assembling

        if (temt::FileManip::isExistingPath(newPath) && temt::FileManip::isDirectory(newPath)) {
            OpenDirectory(newPath);
        }
    }

    void OpenDirectory(const std::string_view path) {
        SetBrowserPosition(path);
        ScreenInteractive::Active()->PostEvent(Event::Custom);
    }
};

ftxui::Component FileBrowser(std::string_view path) {
    return ftxui::Make<FileBrowserImpl>(path);
}