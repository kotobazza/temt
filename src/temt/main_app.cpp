// main_app.cpp
#include "main_app.hpp"
#include "ftxui/dom/elements.hpp"
#include "LogInitializer.hpp"
#include <iostream>

using namespace ftxui;

MainApp::MainApp() : screen_(ScreenInteractive::Fullscreen()) {
    auto log_file = spdlog::get("file_logger");
    log_file->info("MainApp:Creating subcomponent instances");
    file_browser_ = std::make_shared<FileBrowser>();
    file_preview_ = std::make_shared<FilePreview>();
    command_palette_ = std::make_shared<CommandPalette>();
    log_file->info("MainApp:Created subcomponent instances");
    
    // Связываем компоненты
    file_browser_->SetOnFileSelect([this](const auto& path) {
        file_preview_->SetFile(path);
    });

    log_file->info("MainApp: Connected preview with browser");
    
    SetupCommands();
    

    
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

    SetupKeybindings();

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
    spdlog::error("starting_binding");

    if (!command_palette_) {
        spdlog::error("Command palette is not initialized!");
        log_file->error("Command palette is not initialized!");
        
        return;
    }

    spdlog::error("Command palette is initialized!");

    if(!component_){
        spdlog::error("component_ is not initialized!");
    }

    component_ |= CatchEvent([this, log_file](const Event& event) {
        try {
            if (event == Event::F1) {
                if (command_palette_) {
                    command_palette_->Toggle();
                    return true;
                }
                return false;
            }
            if (event == Event::CtrlQ) {
                screen_.Exit();
                return true;
            }
        } catch (const std::exception& e) {

        }
        return false;
    });

    log_file->info("MainApp:Setup keybindings completed");
    spdlog::error("MainApp:Setup keybindings completed");

}

Component MainApp::GetComponent() {
    return component_;
}

void MainApp::Run() {
    screen_.Loop(component_);
}