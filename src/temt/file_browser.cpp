#include "file_browser.hpp"
#include <vector>
#include "FileManip.hpp"
#include "emoji_util.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"
#include "spdlog/spdlog.h"

using namespace ftxui;

class FileBrowserImpl : public ComponentBase {
   public:
    FileBrowserImpl(const std::string_view path) : usingPath_(path.data()) {
        
        if (temt::FileManip::isExistingPath(usingPath_) && temt::FileManip::isDirectory(usingPath_)) {
            entries_ = temt::FileManip::readDirectoryFlatEntries(usingPath_);
            
            for (auto ent : entries_) {
                entriesNames_.push_back(temt::emoji::emojiedFileName(ent));
            }

            menu_ = Menu(&entriesNames_, &selected_);

            Add(menu_);
        }
    }

    Element OnRender() override final {
        return vbox({text(usingPath_) | border, menu_->Render() | vscroll_indicator | yframe | flex | reflect(box_)}) |
               border;
    }

    bool OnEvent(Event event) override final {
        if (event.is_mouse() && event.mouse().button == Mouse::Left) {
            if (event.mouse().motion == Mouse::Pressed && box_.Contain(event.mouse().x, event.mouse().y)) {
                Box menu_box = box_;
                // menu_box.y_min += 1;  // Учитываем заголовок

                if (menu_box.Contain(event.mouse().x, event.mouse().y)) {
                    int local_y = event.mouse().y - menu_box.y_min;

                    if (local_y >= 0 && local_y < static_cast<int>(entriesNames_.size())) {
                        auto now = std::chrono::steady_clock::now();
                        auto time_since_last_click =
                            std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime_).count();

                        if (time_since_last_click < 500 && local_y == selected_) {
                            OnDoubleClickEvent(selected_);
                            lastDoubleClicked_ = selected_;
                            lastClickTime_ = now;
                            return true;
                        }

                        selected_ = local_y;
                        lastClickTime_ = now;
                        return true;
                    }
                }
            }
        }
        return ComponentBase::OnEvent(event);
    }

    bool Focusable() const final { return true; }

   private:
    std::vector<std::string> entriesNames_;
    std::vector<temt::FileManip::FileInfo> entries_;
    std::string usingPath_;
    ftxui::Component menu_;
    int selected_ = 0;
    int lastDoubleClicked_ = 0;
    std::chrono::steady_clock::time_point lastClickTime_;
    ftxui::Box box_;

    void OnDoubleClickEvent(const int selected) {
        auto file_logger = spdlog::get("file_logger");
        if (!file_logger) {
            spdlog::critical("no filelogger");
            return;
        }

        if (selected >= 0 && selected < static_cast<int>(entries_.size())) {
            file_logger->info("Double clicked value: {}", entries_[selected].path);
            file_logger->flush();

            // Здесь можно добавить логику обработки двойного клика
            // Например, открытие файла/папки
        } else {
            file_logger->critical("Error in selected, {}, {}", selected, static_cast<int>(entries_.size()));
            file_logger->flush();
        }
    }
};

ftxui::Component FileBrowser(std::string_view path) {
    return ftxui::Make<FileBrowserImpl>(path);
}