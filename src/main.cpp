#include "LogInitializer.hpp"
#include "ArchiveManip.hpp"
#include "FileManip.hpp"
#include "CharProcessing.hpp"
#include "DiskInfo.hpp"
#include "file_browser.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/component/component.hpp"
#include <filesystem>

int main() {
    temt::Log::createFileLogger("logs/logs.txt");

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    

    auto fb = ftxui::Container::Horizontal({
        FileBrowser(std::filesystem::current_path().c_str()),
        ftxui::Renderer([&](){
            return ftxui::text("hello");
        })
    });
    
    screen.Loop(fb);


    return 0;
}
