#include "main_app.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/event.hpp"
#include "LogInitializer.hpp"

using namespace ftxui;

MainApp::MainApp() {
    Log("Creating subcomponent instances");
    
    // Инициализация компонентов
    file_browser_ = std::make_shared<FileBrowser>();
    file_preview_ = std::make_shared<FilePreview>();
    command_palette_ = std::make_shared<CommandPalette>();
    
    // Настройка связей между компонентами
    file_browser_->SetOnFileSelect([this](const auto& path) {
        file_preview_->SetFile(path);
    });
    
    Log("Connected preview with browser");
    
    // Настройка команд и горячих клавиш
    SetupCommands();
    SetupKeybindings();
    
    // Основной контейнер с горизонтальным расположением
    auto main_container = Container::Horizontal({
        file_browser_,
        file_preview_
    });
    
    // Обработчик переключения фокуса
    main_container |= CatchEvent([this](Event event) {
        static bool browser_focused = true;
        
        if (event == Event::Tab) {
            browser_focused = !browser_focused;
            if (browser_focused) {
                file_browser_->TakeFocus();
            } else {
                file_preview_->TakeFocus();
            }
            return true;
        }
        return false;
    });
    
    // Рендер главного компонента
    auto renderer = Renderer(main_container, [this] {
        return dbox({
            // Основной интерфейс
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
            
            // Командная палитра (поверх всего)
            command_palette_->Render() | center
        });
    });
    
    // Добавляем обработчик глобальных горячих клавиш
    renderer |= CatchEvent([this](Event event) {
        if (event == Event::F1) {
            command_palette_->Toggle();
            return true;
        }
        if (event == Event::CtrlQ) {
            ScreenInteractive::Active()->Exit();
            return true;
        }
        return false;
    });
    
    // Устанавливаем начальный фокус
    file_browser_->TakeFocus();
    
    // Добавляем все дочерние компоненты
    Add(renderer);
    
    Log("MainApp initialization complete");
}

void MainApp::SetupCommands() {
    command_palette_->AddCommand({
        "open", 
        "Open selected file",
        [this] {
            if (file_browser_->Focused()) {
                file_browser_->OnEvent(Event::Return);
            }
        },
        "Enter"
    });
    
    command_palette_->AddCommand({
        "navigate", 
        "Navigate to path", 
        [] {}, 
        "Ctrl+P"
    });
    
    Log("Commands setup complete");
}

void MainApp::SetupKeybindings() {
    // Обработка клавиш уже настроена в конструкторе
    Log("Keybindings setup complete");
}

void MainApp::Log(const std::string& message) {
    if (auto log_file = spdlog::get("file_logger")) {
        log_file->info("MainApp: {}", message);
    }
}

void MainApp::Run() {
    auto screen = ScreenInteractive::Fullscreen();
    auto app = std::make_shared<MainApp>();
    screen.Loop(app);
}