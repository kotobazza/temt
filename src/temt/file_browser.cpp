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
    FileBrowserImpl(const std::string_view path, std::vector<temt::FileManip::FileInfo>& entries, int& data)
        : usingPath_(path.data()), entries_(entries), selected_(data) {
        SetBrowserPosition(usingPath_);

        menu_ = Menu(&entriesNames_, &selected_);

        returnBtn_ = Button("◀ ", [this]() { openParentDirectory(); }) | bold;

        Add(Container::Vertical({returnBtn_, menu_}));
    }

    Element OnRender() override final {
        return vbox({hbox({returnBtn_->Render(),
                           hbox({text("/> ") | color(ftxui::Color::Cyan), text(usingPath_)}) | border | bold}),
                     menu_->Render() | vscroll_indicator | yframe | flex | reflect(menuBox_) |
                         focusPosition(0, selected_)}) |
               border;
    }

    bool OnEvent(Event event) override final {
        if (event.is_mouse() && event.mouse().button == Mouse::Left) {
            if (event.mouse().motion == Mouse::Pressed && menuBox_.Contain(event.mouse().x, event.mouse().y)) {
                if (!ComponentBase::OnEvent(event)) {
                    return false;
                }
                auto now = std::chrono::steady_clock::now();
                auto time_since_last_click =
                    std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime_).count();

                if (time_since_last_click < 500 &&
                    last_selected_ == selected_) {  // TODO: remove 500 and set it as parameter
                    OnDoubleClickEvent(selected_);
                    lastDoubleClicked_ = selected_;

                    return true;
                }
                lastClickTime_ = now;
                last_selected_ = selected_;
                return false;
            }
        }
        if (event == Event::Return) {
            if (returnBtn_->Active()) {
                return ComponentBase::OnEvent(event);
            }
            OpenPath(selected_);
            file_logger_->info("Clicked on Enter");
            return true;
        }

        return ComponentBase::OnEvent(event);
    }

    bool Focusable() const final { return true; }

   private:
    std::string usingPath_;
    std::vector<std::string> entriesNames_;
    std::vector<temt::FileManip::FileInfo>& entries_;
    int& selected_;
    int last_selected_ = 0;
    ftxui::Component menu_;
    ftxui::Box menuBox_;
    int lastDoubleClicked_ = 0;
    std::chrono::steady_clock::time_point lastClickTime_;
    std::shared_ptr<spdlog::logger> file_logger_ = spdlog::get("file_logger");
    ftxui::Component returnBtn_;

    void SetBrowserPosition(const std::string_view path) {
        usingPath_ = path.data();

        entries_.clear();
        entriesNames_.clear();
        entries_ = temt::FileManip::readDirectoryFlatEntries(path);

        for (auto ent : entries_) {
            entriesNames_.push_back(temt::emoji::emojiedFileName(ent));
        }

        file_logger_->info("FileBrowser: opened browser on new path: ", path);
        file_logger_->flush();
    }

    void OnDoubleClickEvent(const int selected) {
        if (!file_logger_) {
            spdlog::critical("no filelogger");
            return;
        }

        if (!(selected >= 0 && selected < static_cast<int>(entries_.size()))) {
            return;
        }

        OpenPath(selected);
    }

    void OpenPath(const int selected) {
        auto entry = entries_[selected];

        std::string newPath = temt::FileManip::assemblePath(entry.parentDirectory, entry.path);

        if (temt::FileManip::isExistingPath(newPath) && temt::FileManip::isDirectory(newPath)) {
            OpenDirectory(newPath);
        }
    }

    void openParentDirectory() {
        SetBrowserPosition(temt::FileManip::getParentPath(usingPath_));
        ScreenInteractive::Active()->PostEvent(Event::Custom);
    }

    void OpenDirectory(const std::string_view path) {
        SetBrowserPosition(path);
        ScreenInteractive::Active()->PostEvent(Event::Custom);
    }
};

ftxui::Component FileBrowser(std::string_view path, std::vector<temt::FileManip::FileInfo>& entries, int& a) {
    return ftxui::Make<FileBrowserImpl>(path, entries, a);
}