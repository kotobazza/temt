#include "main_app.hpp"
#include "FileManip.hpp"
#include "file_browser.hpp"
#include "options_menu.hpp"
#include "spdlog/spdlog.h"

#include "ftxui/component/event.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "app_data.hpp"
#include "text_writer.hpp"

#include <filesystem>

using namespace ftxui;

class MainAppImpl : public ComponentBase {
   public:
    MainAppImpl(std::function<void()> exitClosure)
        : appData_(std::filesystem::current_path().string()), exitClosure_(exitClosure) {
        appData_.AddListener([this]() { ScreenInteractive::Active()->Post(ftxui::Event::Custom); });

        fileBrowser_ = FileBrowser(appData_, [this]() { OpenSelectedFile(); });

        upperPanel_ = Container::Horizontal(
            {Button(" < ", [&]() { appData_.toggleFileBrowser(); }),
             Button("Menu", [this]() { appData_.toggleOptionsMenu(); }),
             Renderer([&]() { return hbox({text(" #>") | color(ftxui::Color::Yellow1), text(appData_.exec_path_)}); }) |
                 vcenter | bold,
             Renderer([]() { return filler(); }), Button(" ✖ ", [this]() { exitClosure_(); })});

        logPanel_ = Renderer([]() { return text("Logs:"); });

        mainPanel_ = Container::Vertical({Renderer([]() { return vbox(text("Nothing selected")) | center; }) | flex});

        auto rightPanel = Container::Vertical({mainPanel_ | border | flex, logPanel_ | border});

        auto resizableSplit = ResizableSplitLeft(fileBrowser_, rightPanel, &appData_.mainAppSplitLength_);

        auto main = Container::Vertical({upperPanel_, resizableSplit | flex}) |
                    Modal(OptionsMenu(appData_,
                                      [this](Component cmp) {
                                          mainPanel_->DetachAllChildren();
                                          mainPanel_->Add(cmp | flex);
                                      }),
                          &appData_.modalShowOptionsMenu);

        Add(main);
    }

    void OpenSelectedFile() {
        appData_.file_logger_->info("Selected file: {}",
                                    appData_.usingDirectoryEntries_[appData_.usingDirectorySelectedIndex()].path);

        std::string path =
            temt::FileManip::assemblePath(appData_.selectedEntry().parentDirectory, appData_.selectedEntry().path);

        mainPanel_->DetachAllChildren();
        mainPanel_->Add(TextWriter(path, [this]() { ExitFromTextEditor(); }) | flex);
    }

    void SetMainPanelChild() {}

    void ExitFromTextEditor() {
        appData_.file_logger_->info("Exited From TextViewer");
        appData_.file_logger_->flush();
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