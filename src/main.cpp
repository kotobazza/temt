#include "LogInitializer.hpp"
#include "ArchiveManip.hpp"
#include "FileManip.hpp"
#include "CharProcessing.hpp"
#include "DiskInfo.hpp"


#include <memory>
#include "ftxui/component/screen_interactive.hpp"
#include "upper_panel.hpp"
#include "left_panel.hpp"
#include "right_panel.hpp"

using namespace ftxui;

int main() {
    temt::Log::createFileLogger("logs/logs.txt");
    auto screen = ScreenInteractive::Fullscreen();
    
    // Создаем наши компоненты
    auto upper_panel = std::make_shared<UpperPanel>();
    auto left_panel = std::make_shared<LeftPanel>();
    auto right_panel = std::make_shared<RightPanel>();
    
    // Настраиваем обратный вызов для кнопки
    upper_panel->SetToggleCallback([&] {
        screen.PostEvent(Event::Custom); // Принудительно обновляем экран
    });
    
    // Создаем resizable split
    auto resizable_panels = ResizableSplitLeft(
        left_panel->GetComponent(),
        right_panel->GetComponent(),
        &left_panel->GetSizeRef() // Нужно добавить этот метод в ResizablePanel
    );
    
    // Главный компонент
    auto main_component = Container::Vertical({
        upper_panel->GetComponent(),
        resizable_panels
    });
    
    // Рендерер
    auto renderer = Renderer(main_component, [&] {
        auto upper = hbox({
            upper_panel->GetComponent()->Render() | center
        }) | border | flex;
        
        auto lower = upper_panel->IsLeftPanelVisible()
            ? resizable_panels->Render()
            : right_panel->Render() | flex;
        
        return vbox({
            upper,
            lower | flex
        });
    });
    
    screen.Loop(renderer);
    return 0;
}