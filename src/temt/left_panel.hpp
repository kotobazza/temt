// left_panel.hpp
#pragma once
#include "resizable_panel.hpp"

class LeftPanel : public ResizablePanel {
public:
    LeftPanel();
    
    ftxui::Component GetComponent() override;
    ftxui::Element Render() override;
};