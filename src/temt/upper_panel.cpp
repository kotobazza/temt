// upper_panel.cpp
#include "upper_panel.hpp"

using namespace ftxui;

UpperPanel::UpperPanel() {
    left_toggle_button_ = Button("<", [this] {
        left_panel_visible_ = !left_panel_visible_;
        if (left_toggle_callback_)
            left_toggle_callback_();
    });
    Add(left_toggle_button_);
    right_toggle_button_ = Button(">", [this] {
        right_panel_visible_ = !right_panel_visible_;
        if (right_toggle_callback_)
            right_toggle_callback_();
    });
    Add(right_toggle_button_);
}

ftxui::Element UpperPanel::OnRender(){
    return hbox({
        left_toggle_button_->Render(),
        ftxui::filler(),
        right_toggle_button_->Render()
    })|flex;
}

void UpperPanel::SetLeftToggleCallback(std::function<void()> callback) {
    left_toggle_callback_ = callback;
}

void UpperPanel::SetRightToggleCallback(std::function<void()> callback) {
    right_toggle_callback_ = callback;
}

bool UpperPanel::IsLeftPanelVisible() const {
    return left_panel_visible_;
}

bool UpperPanel::IsRightPanelVisible() const {
    return right_panel_visible_;
}