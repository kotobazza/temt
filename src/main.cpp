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

void terminateApp(){
    auto fileLogger = spdlog::get("file_logger");
    fileLogger->flush();
}


int main() {
    temt::Log::createFileLogger("logs/logs.txt");

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    

    screen.Loop(MainApp(terminateApp));

    return 0;
}
