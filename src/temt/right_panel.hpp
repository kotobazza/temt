// right_panel.hpp
#pragma once
#include "resizable_panel.hpp"

class RightPanel : public ResizablePanel {
public:
    RightPanel(int& size);
    
    ftxui::Component GetComponent() override;
    ftxui::Element Render() override;
};