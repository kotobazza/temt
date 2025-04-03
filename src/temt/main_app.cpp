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

class MainAppImpl : public ComponentBase {
   public:
    MainAppImpl(std::function<void()> exitClosure)
        : appData_(std::filesystem::current_path().c_str()), exitClosure_(exitClosure) {
        appData_.AddListener([this]() { ScreenInteractive::Active()->Post(ftxui::Event::Custom); });

        fileBrowser_ = FileBrowser(appData_, [this]() { return OpenSelectedFile(); });

        upperPanel_ = Container::Horizontal(
            {Button(" < ", [&]() { appData_.toggleFileBrowser(); }),
             Button("Menu", [this]() { appData_.toggleOptionsMenu(); }), Button("Help", []() {}),
             Button("Stats", []() {}),
             Renderer([&]() { return hbox({text(" #>") | color(ftxui::Color::Yellow1), text(appData_.exec_path_)}); }) |
                 vcenter | bold,
             Renderer([]() { return filler(); }), Button(" ✖ ", [this]() { exitClosure_(); })});

        logPanel_ = Renderer([]() { return text("Logs:"); }) | border;

        mainPanel_ = MainPanel({Button("Hello", []() {}), Renderer([]() { return text("Hello2") | border; })});

        auto rightPanel = Container::Vertical({mainPanel_, logPanel_});

        auto resizableSplit = ResizableSplitLeft(fileBrowser_, rightPanel, &appData_.mainAppSplitLength_);

        auto main =
            Container::Vertical({upperPanel_, resizableSplit | flex}) |
            Modal(Container::Vertical({Button("Exit", [this](){appData_.toggleOptionsMenu();})}),
                  &appData_.modalShowOptionsMenu);

        Add(main);

        // Add(Container::Vertical(
        //         {upperPanel_,
        //          Container::Horizontal({fileBrowser_, Container::Vertical({mainPanel_, logPanel_}) | flex_grow}) |
        //              flex}) |
        //     flex);
    }

    void OpenSelectedFile() {
        appData_.file_logger_->info(
            "Selected file: {}",
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
    return Make<MainAppImpl>(exitClosure);
}