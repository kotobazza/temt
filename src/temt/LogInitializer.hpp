#pragma once
#include <string_view>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/spdlog.h"

namespace temt {
namespace Log {
void createFileLogger(std::string_view log_path);

}  // namespace Log
}  // namespace temt
