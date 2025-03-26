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

    // Создаем компоненты
    auto upper_panel = std::make_shared<UpperPanel>();
    auto left_panel = std::make_shared<LeftPanel>(resizable1);
    auto center_panel = std::make_shared<CenterPanel>(resizable2);
    auto right_panel = std::make_shared<RightPanel>(resizable3);

    // Настраиваем колбэки
    upper_panel->SetLeftToggleCallback([&] { screen.PostEvent(Event::Custom); });

    upper_panel->SetRightToggleCallback([&] { screen.PostEvent(Event::Custom); });

    // Создаем resizable панели
    auto left_and_center =
        ResizableSplitLeft(left_panel->GetComponent(), center_panel->GetComponent(), &left_panel->GetSizeRef());

    auto all_panels = ResizableSplitLeft(left_and_center, right_panel->GetComponent(), &center_panel->GetSizeRef());

    // Главный компонент
    auto main_component = Container::Vertical({upper_panel->GetComponent(), all_panels});

    // Рендерер
    auto renderer = Renderer(main_component, [&] {
        auto upper = hbox({upper_panel->GetComponent()->Render() | center}) | border | flex;

        // Логика отображения панелей
        auto lower = [&] {
            if (upper_panel->IsLeftPanelVisible() && upper_panel->IsRightPanelVisible()) {
                return all_panels->Render();
            } else if (upper_panel->IsLeftPanelVisible()) {
                return left_and_center->Render();
            } else if (upper_panel->IsRightPanelVisible()) {
                auto center_and_right = ResizableSplitLeft(center_panel->GetComponent(), right_panel->GetComponent(),
                                                           &center_panel->GetSizeRef());
                return center_and_right->Render();
            } else {
                return center_panel->Render() | flex;
            }
        }();

        return vbox({upper, lower | flex});
    });

    screen.Loop(renderer);
    return 0;
}