#include "ArchiveManip.hpp"
#include "CharProcessing.hpp"
#include "DiskInfo.hpp"
#include "FileManip.hpp"
#include "LogInitializer.hpp"

#include "mydobuleclickmenu.hpp"

#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/screen/screen.hpp"

int main() {
    temt::Log::createFileLogger("logs/logs.txt");
    using namespace ftxui;

    auto screen = ScreenInteractive::Fullscreen();
    std::vector<std::string> vals{"First", "Second", "Third", "Fourth"};

    auto menu = ftxui::Make<DoubleClickMenu>(vals, [](int index) {
        auto logger = spdlog::get("file_logger");
        logger->info("double clicked menu: {}", index);
    });

    screen.Loop(menu);

    return 0;
}
