// component_base.hpp
#pragma once
#include <functional>
#include <memory>
#include <vector>
#include "ftxui/component/component_base.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/component/event.hpp"

class CustomComponent : public ftxui::ComponentBase {
public:
    using OnEventCallback = std::function<void()>;
    
    
    void SetOnFocus(OnEventCallback callback) { on_focus_ = callback; }
    void SetOnBlur(OnEventCallback callback) { on_blur_ = callback; }
    
    bool Focused() const {
        return focused_;
    }
    
    bool OnEvent(ftxui::Event event) override {
        if (event.is_mouse() && CaptureMouse(event)) {
            return true;
        }
        
        bool handled = ComponentBase::OnEvent(event);
        
        if (event == ftxui::Event::Tab) {
            focused_ = !focused_;
            if (focused_ && on_focus_) on_focus_();
            if (!focused_ && on_blur_) on_blur_();
            return true;
        }
        
        return handled;
    }
    
protected:
    bool focused_ = false;
    OnEventCallback on_focus_;
    OnEventCallback on_blur_;
};