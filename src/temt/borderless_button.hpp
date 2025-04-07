#pragma once

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"

ftxui::Component BorderlessButton(std::string label, std::function<void()> on_click);