#include "main_app.hpp"
#include "ftxui/dom/elements.hpp"
#include "LogInitializer.hpp"
#include "ftxui/component/screen_interactive.hpp"

using namespace ftxui;

MainApp::MainApp() {
    Log("Initializing MainApp");
    
    // 1. Создаем компоненты
    file_browser_ = std::make_shared<FileBrowser>();
    file_preview_ = std::make_shared<FilePreview>();
    command_palette_ = std::make_shared<CommandPalette>();
    
    // 2. Настраиваем связи
    file_browser_->SetOnFileSelect([this](const auto& path) {
        file_preview_->SetFile(path);
    });
    
    // 3. Создаем основной контейнер
    auto container_ = Container::Horizontal({
        file_browser_,
        file_preview_
    });
    
    // 4. Настраиваем рендер
    auto renderer_ = Renderer(container_, [this] {
        return dbox({
            vbox({
                text("File Manager") | bold | center,
                separator(),
                hbox({
                    file_browser_->Render() | flex | frame | border | focus,
                    file_preview_->Render() | flex | frame | border
                }) | flex,
                separator(),
                text("F1: Help • Ctrl+Q: Quit • Tab: Switch focus") | center | dim
            }),
            command_palette_->Render() | center
        });
    });
    
    // 5. Настраиваем обработчики событий
    
    SetupCommands();
    
    // 6. Добавляем все в корневой компонент
    Add(container_);
    Add(renderer_);
    
    // 7. Устанавливаем начальный фокус
    file_browser_->TakeFocus();

    Component base = CatchEvent(renderer_, [this](Event event) {
        if (event == Event::F1) {
            command_palette_->Toggle();
            return true;
        }
        if (event == Event::CtrlQ) {
            ScreenInteractive::Active()->Exit();
            return true;
        }
        if (event == Event::Tab) {
            if (file_browser_->Focused()) {
                file_preview_->TakeFocus();
            } else {
                file_browser_->TakeFocus();
            }
            return true;
        }
        return ComponentBase::OnEvent(event)||false;
    });
    
    Add(base);
    
    Log("MainApp initialized");
}


void MainApp::SetupCommands() {
    command_palette_->AddCommand({
        "open", "Open selected file",
        [this] { file_browser_->OnEvent(Event::Return); },
        "Enter"
    });
    

    
    Log("Commands initialized");
}

void MainApp::Log(const std::string& message) {
    if (auto log = spdlog::get("file_logger")) {
        log->info("MainApp: {}", message);
    }
}

void MainApp::Run() {
    auto screen = ScreenInteractive::Fullscreen();
    auto app = std::make_shared<MainApp>();
    screen.Loop(app);
}