#include "ftxui/component/screen_interactive.hpp"

#include "LogInitializer.hpp"
#include "main_app.hpp"

auto screen = ftxui::ScreenInteractive::Fullscreen();

void terminateApp() {
    auto fileLogger = spdlog::get("file_logger");
    fileLogger->flush();
    screen.Exit();
}

int main() {
    temt::Log::createFileLogger("logs/logs.txt");

    screen.Loop(MainApp(terminateApp));

    return 0;
}
