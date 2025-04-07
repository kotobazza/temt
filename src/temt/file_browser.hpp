#pragma once

#include "string_view"

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "FileManip.hpp"
#include "app_data.hpp"

ftxui::Component FileBrowser(temt::AppData& appData, std::function<void()> openClosure);