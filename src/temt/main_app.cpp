#include "main_app.hpp"
#include "file_browser.hpp"
#include "hideable.hpp"

#include <filesystem>

using namespace ftxui;

class Impl : public ComponentBase {
   public:
    Impl() {
        fileBrowser_ = FileBrowser(std::filesystem::current_path().c_str(), selectedFileBrowser_);
        upperPanel_ =
            Container::Horizontal({Button(" < ", [&]() { hiddenFileBrowserPanel_ = !hiddenFileBrowserPanel_; }),
                                   Button("Menu", []() {}), Renderer([]() { return filler(); }),
                                   Button("Help", []() {}), Button(" ✖ ", []() {})}) |
            border;

        mainPanel_ = Container::Vertical({Renderer([]() { return text("HelloWrold"); })});

        Add(Container::Vertical(
            {upperPanel_, Container::Horizontal({Hideable({fileBrowser_}, hiddenFileBrowserPanel_), mainPanel_})}));
    }

   private:
    ftxui::Component fileBrowser_;
    ftxui::Component upperPanel_;
    ftxui::Component mainPanel_;

    int selectedFileBrowser_;
    bool hiddenFileBrowserPanel_;
};

Component MainApp() {
    return Make<Impl>();
}