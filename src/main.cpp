#include "ArchiveManip.hpp"
#include "CharProcessing.hpp"
#include "DiskInfo.hpp"
#include "FileManip.hpp"
#include "LogInitializer.hpp"

#include <memory>
#include "center_panel.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "left_panel.hpp"
#include "right_panel.hpp"
#include "upper_panel.hpp"

using namespace ftxui;

int main() {
    temt::Log::createFileLogger("logs/logs.txt");
    auto screen = ScreenInteractive::Fullscreen();

    int resizable1 = 30;
    int resizable2 = 30;
    int resizable3 = 40;

    int memory_resize1 = 0;
    int memory_resize3 = 0;

    // Создаем компоненты
    auto upper_panel = std::make_shared<UpperPanel>();
    auto left_panel = std::make_shared<LeftPanel>(resizable1);
    auto center_panel = std::make_shared<CenterPanel>(resizable2);
    auto right_panel = std::make_shared<RightPanel>(resizable3);

    // Настраиваем колбэки
    upper_panel->SetLeftToggleCallback([&] {
        screen.PostEvent(Event::Custom);
        std::swap(resizable1, memory_resize1);
    });

    upper_panel->SetRightToggleCallback([&] {
        screen.PostEvent(Event::Custom);
        std::swap(resizable3, memory_resize3);
    });

    // Создаем resizable панели
    auto left_and_center = ResizableSplitLeft(left_panel, center_panel, &left_panel->GetSizeRef());

    auto all_panels = ResizableSplitRight(right_panel,left_and_center, &right_panel->GetSizeRef());


    // Главный компонент
    auto main_component = Container::Vertical({upper_panel, all_panels});

    // Рендерер
    auto renderer = Renderer(main_component, [&] {
        auto upper = hbox({upper_panel->Render()|flex}) | border | flex;

        // Логика отображения панелей
        auto lower = [&] {
            return all_panels->Render() | flex;
        }();

        return vbox({upper, lower | flex});
    });

    screen.Loop(renderer);
    return 0;
}