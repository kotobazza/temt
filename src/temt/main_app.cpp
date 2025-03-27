// main_app.cpp
#include "main_app.hpp"
#include "ftxui/dom/elements.hpp"
#include "LogInitializer.hpp"

using namespace ftxui;

MainApp::MainApp() : screen_(ScreenInteractive::Fullscreen()) {
    auto log_file = spdlog::get("file_logger");
    file_browser_ = std::make_shared<FileBrowser>();
    file_preview_ = std::make_shared<FilePreview>();
    command_palette_ = std::make_shared<CommandPalette>();
    log_file->info("MainApp:Created subcomponent instances");
    
    // Связываем компоненты
    file_browser_->SetOnFileSelect([this](const auto& path) {
        file_preview_->SetFile(path);
    });

    log_file->info("MainApp:Connected preview with browser");
    
    SetupCommands();
    SetupKeybindings();
    
    // Основной layout
    auto main_container = Container::Horizontal({
        file_browser_,
        file_preview_
    });
    
    component_ = Renderer(main_container, [this] {
        return dbox({
            // Основной интерфейс
            vbox({
                text("File Manager") | bold | center,
                separator(),
                hbox({
                    file_browser_->Render() | flex,
                    file_preview_->Render() | flex
                }) | flex,
                separator(),
                text("F1: Help • Ctrl+Q: Quit • Tab: Switch focus") | center | dim
            }),
            
            // Командная палитра (поверх всего)
            command_palette_->Render() | center
        });
    });

    log_file->info("MainApp:Created component");
}

void MainApp::SetupCommands() {
    auto log_file = spdlog::get("file_logger");
    command_palette_->AddCommand({
        "open", "Open selected file", [this]{
            if (file_browser_->Focused()) {
                file_browser_->OnEvent(Event::Return);
            }
        }, "Enter"
    });
    
    command_palette_->AddCommand({
        "navigate", "Navigate to path", []{}, "Ctrl+P"
    });

    log_file->info("MainApp:Setted up commands");
}

void MainApp::SetupKeybindings() {
    auto log_file = spdlog::get("file_logger");
    component_ = CatchEvent(component_, [this](Event event) {
        if (event == Event::F1) {
            command_palette_->Toggle();
            return true;
        }
        if (event == ftxui::Event::CtrlQ) {
            screen_.Exit();
            return true;
        }
        return false;
    });

    log_file->info("MainApp:Created subcomponent instances");
}

Component MainApp::GetComponent() {
    return component_;
}

void MainApp::Run() {
    screen_.Loop(component_);
}