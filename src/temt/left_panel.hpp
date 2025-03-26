// left_panel.hpp
#pragma once
#include "resizable_panel.hpp"

class LeftPanel : public ResizablePanel {
public:
    LeftPanel(int& size);
    
    ftxui::Element OnRender() override;
};