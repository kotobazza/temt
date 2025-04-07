#pragma once


#include "app_data.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"


ftxui::Component OptionsMenu(temt::AppData& appData, std::function<void(ftxui::Component)> optionSelectedClosure);