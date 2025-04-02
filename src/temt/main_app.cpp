#include "main_app.hpp"
#include "FileManip.hpp"
#include "file_browser.hpp"
#include "hideable.hpp"
#include "main_panel.hpp"
#include "spdlog/spdlog.h"

#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "app_data.hpp"

#include <filesystem>

using namespace ftxui;

class Impl : public ComponentBase {
   public:
    Impl(std::function<void()> exitClosure)
        : appData_(std::filesystem::current_path().c_str()), exitClosure_(exitClosure) {
        appData_.AddListener([this]() { ScreenInteractive::Active()->Post(ftxui::Event::Custom); });

        fileBrowser_ = Hideable({FileBrowser(appData_, [this]() { return OpenSelectedFile(); })},
                                appData_.isFleBrowserPanelHidden_);

        upperPanel_ = Container::Horizontal(
            {Button(" < ", [&]() { appData_.toggleFileBrowser(); }),
             Button("Menu", []() {}), Button("Help", []() {}), Button("Stats", []() {}),
             Renderer([&]() { return hbox({text(" #>") | color(ftxui::Color::Yellow1), text(appData_.exec_path_)}); }) |
                 vcenter | bold,
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
        appData_.file_logger_->info("Selected file: {}",
                           appData_.usingDirectoryEntries_[appData_.usingDirectorySelectedIndex()].parentDirectory);
    }

   private:
    ftxui::Component fileBrowser_;
    ftxui::Component upperPanel_;
    ftxui::Component mainPanel_;
    ftxui::Component logPanel_;

    temt::AppData appData_;

    std::function<void()> exitClosure_;
};

Component MainApp(std::function<void()> exitClosure) {
    return Make<Impl>(exitClosure);
}