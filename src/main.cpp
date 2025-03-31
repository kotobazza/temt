#include <filesystem>
#include <string>
#include "ArchiveManip.hpp"
#include "CharProcessing.hpp"
#include "DiskInfo.hpp"
#include "FileManip.hpp"
#include "LogInitializer.hpp"
#include "file_browser.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "main_app.hpp"
#include <cstdlib>

auto screen = ftxui::ScreenInteractive::Fullscreen();

void terminateApp(){
    auto fileLogger = spdlog::get("file_logger");
    fileLogger->flush();
    screen.Exit();
}

int main() {
    temt::Log::createFileLogger("logs/logs.txt");

    screen.Loop(MainApp(terminateApp));

    return 0;
}
