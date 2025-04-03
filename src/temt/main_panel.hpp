#pragma once

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"

ftxui::Component MainPanel(ftxui::Components childs);

void UpdateChildren(ftxui::Component main_panel, ftxui::Components new_children);