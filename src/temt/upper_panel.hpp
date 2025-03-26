// upper_panel.hpp
#pragma once

#include <functional>
#include <memory>
#include "ftxui/component/component.hpp"

class UpperPanel {
public:
    UpperPanel();
    
    ftxui::Component GetComponent();
    void SetToggleCallback(std::function<void()> callback);
    bool IsLeftPanelVisible() const;

private:
    bool left_panel_visible_ = true;
    ftxui::Component toggle_button_;
    std::function<void()> toggle_callback_;
};