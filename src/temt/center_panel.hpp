// center_panel.hpp
#pragma once
#include "resizable_panel.hpp"

class CenterPanel : public ResizablePanel {
public:
    CenterPanel(int& size);
    
    ftxui::Component GetComponent() override;
    ftxui::Element Render() override;
};