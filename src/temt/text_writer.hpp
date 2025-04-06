#pragma once

#include "app_data.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"


ftxui::Component TextWriter(std::string_view file_path, std::function<void()> exitClosure);
ftxui::Component TextWriter(std::string_view content, std::string_view current_location, std::function<void()> exitClosure);