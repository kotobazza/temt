// right_panel.hpp
#pragma once
#include "resizable_panel.hpp"

class RightPanel : public ResizablePanel {
public:
    RightPanel(int& size);
    
    ftxui::Element OnRender() override;
};