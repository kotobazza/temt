// upper_panel.cpp
#include "upper_panel.hpp"

using namespace ftxui;

UpperPanel::UpperPanel() {
    left_toggle_button_ = Button(
        "<",
        [this] { 
            left_panel_visible_ = !left_panel_visible_;
            if (left_toggle_callback_) left_toggle_callback_();
        }
    );
    right_toggle_button_ = Button(
        ">",
        [this] { 
            right_panel_visible_ = !right_panel_visible_;
            if (right_toggle_callback_) right_toggle_callback_();
        }
    );
}

Component UpperPanel::GetComponent() {
    return Container::Horizontal({
        left_toggle_button_,
        right_toggle_button_
    });
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