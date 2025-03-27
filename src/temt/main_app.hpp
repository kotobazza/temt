#pragma once
#include <memory>
#include "ftxui/component/component_base.hpp"
#include "file_browser.hpp"
#include "file_preview.hpp"
#include "command_palette.hpp"

class MainApp : public ftxui::ComponentBase {
public:
    MainApp();
    
    static void Run();

private:
    void SetupCommands();
    void SetupKeybindings();
    
    std::shared_ptr<FileBrowser> file_browser_;
    std::shared_ptr<FilePreview> file_preview_;
    std::shared_ptr<CommandPalette> command_palette_;
    bool browser_focused = false;

    void Log(const std::string& message);
};