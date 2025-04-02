#include "main_app.hpp"
#include "FileManip.hpp"
#include "file_browser.hpp"
#include "hideable.hpp"
#include "main_panel.hpp"
#include "spdlog/spdlog.h"

#include <filesystem>

using namespace ftxui;

class Impl : public ComponentBase {
   public:
    Impl(std::function<void()> exitClosure) : exitClosure_(exitClosure) {
        exec_path = std::filesystem::current_path().c_str();
        fileBrowser_ = Hideable({FileBrowser(exec_path, fileBrowserEntries_, selectedFileBrowser_,
                                             [this]() { return OpenSelectedFile(); })},
                                hiddenFileBrowserPanel_);
        upperPanel_ = Container::Horizontal(
            {Button(" < ", [&]() { hiddenFileBrowserPanel_ = !hiddenFileBrowserPanel_; }), Button("Menu", []() {}),
             Button("Help", []() {}),
             Renderer([&]() { return hbox({text(" #>") | color(ftxui::Color::Yellow1), text(exec_path)}); }) | vcenter |
                 bold,
             Renderer([]() { return filler(); }), Button(" ✖ ", [this]() { exitClosure_(); })});

        logPanel_ = Renderer([]() { return text("Logs:"); }) | border;

        mainPanel_ = MainPanel();

        Add(Container::Vertical(
                {upperPanel_,
                 Container::Horizontal({fileBrowser_, Container::Vertical({mainPanel_, logPanel_}) | flex_grow}) |
                     flex}) |
            flex);
    }

    void OpenSelectedFile() {
        file_logger_->info("Selected file: {}", fileBrowserEntries_[selectedFileBrowser_].parentDirectory);
    }

   private:
    ftxui::Component fileBrowser_;
    ftxui::Component upperPanel_;
    ftxui::Component mainPanel_;
    ftxui::Component logPanel_;

    std::function<void()> exitClosure_;

    std::shared_ptr<spdlog::logger> file_logger_ = spdlog::get("file_logger");

    std::string exec_path;
    std::vector<temt::FileManip::FileInfo> fileBrowserEntries_;
    int selectedFileBrowser_ = 0;
    bool hiddenFileBrowserPanel_ = true;
};

Component MainApp(std::function<void()> exitClosure) {
    return Make<Impl>(exitClosure);
}