// main_app.hpp
#pragma once
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"

#include "file_browser.hpp"
#include "file_preview.hpp"
#include "command_palette.hpp"

class MainApp {
public:
    MainApp();
    ftxui::Component GetComponent();
    void Run();
    
private:
    void SetupCommands();
    void SetupKeybindings();
    
    std::shared_ptr<FileBrowser> file_browser_;
    std::shared_ptr<FilePreview> file_preview_;
    std::shared_ptr<CommandPalette> command_palette_;
    ftxui::Component component_;
    ftxui::ScreenInteractive screen_;
};