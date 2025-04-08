#include <chrono>
#include <vector>

#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "spdlog/spdlog.h"

#include "FileManip.hpp"
#include "borderless_button.hpp"
#include "emoji_util.hpp"
#include "file_browser.hpp"

using namespace ftxui;

class FileBrowserImpl : public ComponentBase {
   public:
    FileBrowserImpl(temt::AppData& appData, std::function<void()> openClosure)
        : appData_(appData), openFileClosure_(openClosure) {
        for (auto entry : appData_.usingDirectoryEntries_) {
            entriesNames_.push_back(temt::emoji::emojiedFileName(entry));
        }

        menu_ = Menu(&entriesNames_, &appData_.usingDirectorySelectedIndex());

        returnBtn_ = BorderlessButton("  ⬅  ../", [this]() {OpenParentDirectory(); }) | bold;

        Add(Container::Vertical({returnBtn_, menu_}));
    }

    Element OnRender() override final {
        entriesNames_.clear();
        for (auto entry : appData_.usingDirectoryEntries_) {
            entriesNames_.push_back(temt::emoji::emojiedFileName(entry));
        }

        return vbox({hbox({text("/> ") | color(ftxui::Color::Cyan), text(appData_.current_path_) | underlined}) |
                         borderEmpty | bold,
                     returnBtn_->Render(),
                     menu_->Render() | vscroll_indicator | yframe | flex | reflect(menuBox_) |
                         focusPosition(0, appData_.usingDirectorySelectedIndex())}) |
               border | yflex;
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

                int appdata_selected = appData_.usingDirectorySelectedIndex();

                if (time_since_last_click < appData_.doubleClickDelay_parameter &&
                    last_selected_ == appdata_selected) {  // TODO: parameter
                    OpenSelectedEntry(appdata_selected);
                    lastDoubleClicked_ = appdata_selected;

                    return true;
                }
                lastClickTime_ = now;
                last_selected_ = appData_.usingDirectorySelectedIndex();
                return false;
            }
        }
        if (event == Event::Return) {
            if (returnBtn_->Active()) {
                return ComponentBase::OnEvent(event);
            }
            OpenSelectedEntry(appData_.usingDirectorySelectedIndex());
            return true;
        }

        return ComponentBase::OnEvent(event);
    }

    bool Focusable() const final { return true; }

   private:
    temt::AppData& appData_;
    std::function<void()> openFileClosure_;

    std::vector<std::string> entriesNames_;
    int last_selected_ = 0;
    int lastDoubleClicked_ = 0;
    std::chrono::steady_clock::time_point lastClickTime_;

    ftxui::Component menu_;
    ftxui::Component returnBtn_;

    ftxui::Box menuBox_;

    void OpenSelectedEntry(const int selected) {
        if (!(selected >= 0 && selected < static_cast<int>(appData_.usingDirectoryEntries_.size()))) {
            return;
        }
        auto entry = appData_.usingDirectoryEntries_[selected];

        std::string newPath = temt::FileManip::assemblePath(entry.parentDirectory, entry.path);

        if (temt::FileManip::isExistingPath(newPath) && temt::FileManip::isDirectory(newPath)) {
            OpenDirectory(newPath);
        } else {
            appData_.file_logger_->info("FileBrowser: asks openFileClosure_ to open non-directory path: {}", newPath);
            openFileClosure_();
        }
    }

    void OpenParentDirectory() { appData_.NavigateToPath(temt::FileManip::getParentPath(appData_.current_path_)); }

    void OpenDirectory(const std::string_view path) { appData_.NavigateToPath(path); }
};

ftxui::Component FileBrowser(temt::AppData& appData_, std::function<void()> openClosure) {
    return ftxui::Make<FileBrowserImpl>(appData_, openClosure);
}