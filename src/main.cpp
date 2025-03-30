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

int main() {
    temt::Log::createFileLogger("logs/logs.txt");

    auto screen = ftxui::ScreenInteractive::Fullscreen();
    int a = 0;

    auto fbb = FileBrowser(std::filesystem::current_path().c_str(), a);

    auto fb = ftxui::Container::Horizontal({fbb, ftxui::Renderer([&]() { return ftxui::text(std::to_string(a)); })});

    screen.Loop(fb);

    return 0;
}
