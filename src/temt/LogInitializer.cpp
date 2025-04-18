
#include "LogInitializer.hpp"

namespace temt {
namespace Log {
void createFileLogger(std::string_view log_path) {
    try {
        auto logger = spdlog::get("file_logger");
        if (!logger) {
            logger = spdlog::basic_logger_mt("file_logger", log_path.data());
            spdlog::set_level(spdlog::level::debug);
        }

        logger->info("Loaded basic file_logger logger mutex");
    } catch (const spdlog::spdlog_ex& ex) {
        spdlog::error("Log init failed {}", ex.what());
    }
}
}  // namespace Log

}  // namespace temt