// upper_panel.hpp
#pragma once

#include <functional>
#include <memory>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"

class UpperPanel : public ftxui::ComponentBase {
   public:
    UpperPanel();

    ftxui::Element OnRender() override;

    void SetLeftToggleCallback(std::function<void()> callback);
    void SetRightToggleCallback(std::function<void()> callback);
    bool IsLeftPanelVisible() const;
    bool IsRightPanelVisible() const;

   private:
    bool left_panel_visible_ = true;
    bool right_panel_visible_ = true;
    std::function<void()> left_toggle_callback_;
    std::function<void()> right_toggle_callback_;

    ftxui::Component left_toggle_button_;
    ftxui::Component right_toggle_button_;

};