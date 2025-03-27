#include "LogInitializer.hpp"
// #include "ArchiveManip.hpp"
// #include "FileManip.hpp"
// #include "CharProcessing.hpp"
// #include "DiskInfo.hpp"

// int main() {
//     temt::Log::createFileLogger("logs/logs.txt");
//     return 0;
// }

void terminate_handler() {
    spdlog::critical("Program closed with error");
    spdlog::shutdown();
    std::abort();
}

// main.cpp
#include "main_app.hpp"

int main() {
    std::set_terminate(terminate_handler);
    temt::Log::createFileLogger("logs/logs.txt");
    auto log_file = spdlog::get("file_logger");
    log_file->info("main: created logger");
    auto app = MainApp();
    log_file->info("main: created mainapp");
    app.Run();
    return 0;
}