#pragma once

#include "string_view"

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "FileManip.hpp"

ftxui::Component FileBrowser(std::string_view path, std::vector<temt::FileManip::FileInfo>& entries, int& a, std::function<void()> openClosure);