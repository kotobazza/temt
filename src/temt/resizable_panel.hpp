// resizable_panel.hpp
#pragma once

#include <memory>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"

class ResizablePanel : public ftxui::ComponentBase {
   public:
    ResizablePanel(const std::string& title, int& size);

    ftxui::Element OnRender() override = 0;

    int& GetSizeRef();

   protected:
    std::string title_;
    int& size_;  // размер в процентах
};