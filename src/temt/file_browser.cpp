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
    FileBrowserImpl(std::string_view path) : usingPath_(path.data()) {
        if (temt::FileManip::isExistingPath(usingPath_) && temt::FileManip::isDirectory(usingPath_)) {
            auto fileEnts = temt::FileManip::readDirectoryFlatEntries(usingPath_);

            for (auto ent : fileEnts) {
                entries_.push_back(temt::emoji::emojiedFileName(ent));
            }

            menu_ = Menu(&entries_, &selected_);

            Add(menu_);
        }
    }

    Element OnRender() override final {
        return vbox({text(usingPath_) | border, menu_->Render() | vscroll_indicator | yframe | flex}) | border;
    }

    bool OnEvent(Event event) override final { return ComponentBase::OnEvent(event); }

    bool Focusable() const final { return true; }

   private:
    std::vector<std::string> entries_;
    std::string usingPath_;
    ftxui::Component menu_;
    int selected_ = 0;
};

ftxui::Component FileBrowser(std::string_view path) {
    return ftxui::Make<FileBrowserImpl>(path);
}