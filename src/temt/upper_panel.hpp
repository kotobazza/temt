// upper_panel.hpp
#pragma once

#include <functional>
#include <memory>
#include "ftxui/component/component.hpp"

class UpperPanel {
public:
    UpperPanel();
    
    ftxui::Component GetComponent();
    void SetLeftToggleCallback(std::function<void()> callback);
    void SetRightToggleCallback(std::function<void()> callback);
    bool IsLeftPanelVisible() const;
    bool IsRightPanelVisible() const;

private:
    bool left_panel_visible_ = true;
    bool right_panel_visible_ = true;
    ftxui::Component left_toggle_button_;
    ftxui::Component right_toggle_button_;
    std::function<void()> left_toggle_callback_;
    std::function<void()> right_toggle_callback_;
};