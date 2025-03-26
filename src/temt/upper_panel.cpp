// upper_panel.cpp
#include "upper_panel.hpp"

using namespace ftxui;

UpperPanel::UpperPanel() {
    toggle_button_ = Button(
        "^",
        [this] { 
            left_panel_visible_ = !left_panel_visible_;
            if (toggle_callback_) toggle_callback_();
        }
    );
}

Component UpperPanel::GetComponent() {
    return toggle_button_;
}

void UpperPanel::SetToggleCallback(std::function<void()> callback) {
    toggle_callback_ = callback;
}

bool UpperPanel::IsLeftPanelVisible() const {
    return left_panel_visible_;
}