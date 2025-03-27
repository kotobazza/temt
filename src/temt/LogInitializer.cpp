
#include "LogInitializer.hpp"
#include <iostream>

namespace temt {
namespace Log {
void createFileLogger(std::string_view log_path) {
    std::cout<<"start creating file_logger\n";
    std::cout<<"log_path: " << log_path.data()<<"\n";
    try {
        auto logger = spdlog::get("file_logger");
        std::cout<<"asked for file_logger\n";
        if (!logger) {
            std::cout<<"--creating new file_logger1";
            logger = spdlog::basic_logger_mt("file_logger", log_path.data());
            std::cout<<logger->name();
            std::cout<<"--creating new file_logger2";
            spdlog::set_level(spdlog::level::debug);
            std::cout<<"--created new file_logger";
        }
        logger->info("Loaded basic file_logger logger mutex");
    } catch (const spdlog::spdlog_ex& ex) {
        spdlog::error("Log init failed {}", ex.what());
    } catch(const std::exception& ex){
        spdlog::error("Got any other errors {}", ex.what());
    }
}
}  // namespace Log

}  // namespace temt