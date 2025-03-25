#include "LogInitializer.hpp"
#include "ArchiveManip.hpp"
#include "FileManip.hpp"
#include "CharProcessing.hpp"
#include "DiskInfo.hpp"

#include "mytoggle.hpp"

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/flexbox_config.hpp>
#include <ftxui/dom/node.hpp>
#include <ftxui/screen/terminal.hpp>
#include "ftxui/component/component_base.hpp"     // for ComponentBase
#include "ftxui/component/screen_interactive.hpp" // for ScreenInteractive

int main() {
    temt::Log::createFileLogger("logs/logs.txt");
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();

    bool a = false;

    auto toggle = MyToggle("Hello", "World", a);

    auto container = Container::Vertical({toggle});


    screen.Loop(container);


    
    return 0;
}
