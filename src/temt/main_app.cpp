#include "main_app.hpp"
#include "FileManip.hpp"
#include "file_browser.hpp"
#include "hideable.hpp"
#include "spdlog/spdlog.h"

#include <filesystem>

using namespace ftxui;

class Impl : public ComponentBase {
   public:
    Impl() {
        exec_path = std::filesystem::current_path().c_str();
        fileBrowser_ =
            Hideable({FileBrowser(exec_path, fileBrowserEntries_, selectedFileBrowser_, [this](){return OpenSelectedFile();})}, hiddenFileBrowserPanel_);
        upperPanel_ = Container::Horizontal(
            {Button(" < ", [&]() { hiddenFileBrowserPanel_ = !hiddenFileBrowserPanel_; }), Button("Menu", []() {}),
             Button("Help", []() {}),
             Renderer([&]() { return hbox({text(" #>") | color(ftxui::Color::Yellow1), text(exec_path)}); }) | vcenter |
                 bold,
             Renderer([]() { return filler(); }), Button(" ✖ ", []() {})});

        logPanel_ = Renderer([]() { return text("Logs:"); }) | border;

        mainPanel_ = Renderer([]() {
                         return vbox({
                                    text("HelloWrold"),
                                    text("HelloWrold"),
                                    text("HelloWrold"),
                                    text("HelloWrold"),
                                    filler(),
                                    text("HelloWrold"),
                                }) |
                                flex_grow;
                     }) |
                     border;

        Add(Container::Vertical(
                {upperPanel_, Container::Horizontal({fileBrowser_,
                                                     Container::Vertical({mainPanel_, logPanel_}) | flex_grow}) |
                                  flex}) |
            flex);
    }

    void OpenSelectedFile(){
        file_logger_->info("Selected file: {}", fileBrowserEntries_[selectedFileBrowser_].parentDirectory);
    }

   private:
    ftxui::Component fileBrowser_;
    ftxui::Component upperPanel_;
    ftxui::Component mainPanel_;
    ftxui::Component logPanel_;

    std::string exec_path;

    std::shared_ptr<spdlog::logger> file_logger_ = spdlog::get("file_logger");

    std::vector<temt::FileManip::FileInfo> fileBrowserEntries_;
    int selectedFileBrowser_=0;
    bool hiddenFileBrowserPanel_ = true;
};

Component MainApp() {
    return Make<Impl>();
}