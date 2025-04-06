#pragma once

#include "app_data.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"


ftxui::Component TextWriter(temt::AppData& appData, std::function<void()> exitClosure);
ftxui::Component TextWriter(std::string& content, std::function<void()> exitClosure);