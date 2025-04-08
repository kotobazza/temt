#include "borderless_button.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"
using namespace ftxui;

class BorderlessButtonImpl : public ComponentBase {
   public:
    BorderlessButtonImpl(std::string label, std::function<void()> on_click) : label_(label), on_click_(on_click), hovered_(false), active_(false) {}

    Element OnRender() override {
        bool is_hovered = hovered_ || active_;
        bool is_focused = Focused();

        return text(label_) | (is_hovered || is_focused ? inverted : nothing) | (is_focused ? focus : nothing) |
               (!(is_hovered || is_focused) ? dim : nothing) | reflect(box_);
    }

    // Обработка событий
    bool OnEvent(Event event) override {
        if (event.is_mouse()) {
            return OnMouseEvent(event);
        }

        // Enter или Space активируют кнопку
        if (event == Event::Return || event == Event::Character(' ')) {
            on_click_();
            return true;
        }

        return false;
    }

    bool OnMouseEvent(Event event) {
        hovered_ = box_.Contain(event.mouse().x, event.mouse().y);

        if (event.mouse().button == Mouse::Left) {
            if (event.mouse().motion == Mouse::Pressed && hovered_) {
                active_ = true;
                return true;
            }

            if (event.mouse().motion == Mouse::Released && active_) {
                active_ = false;
                if (hovered_) {
                    on_click_();
                    return true;
                }
            }
        }

        return false;
    }

    bool Focusable() const final { return true; }

   private:
    bool hovered_;
    bool active_;
    std::string label_;
    std::function<void()> on_click_;
    Box box_;
};

Component BorderlessButton(std::string label, std::function<void()> on_click) {
    return Make<BorderlessButtonImpl>(label, on_click);
}