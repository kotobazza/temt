#include "mytoggle.hpp"

#include "ftxui/component/event.hpp"

class MyToggleImpl : public ftxui::ComponentBase {
   public:
    MyToggleImpl(const char* label_on, const char* label_off, bool& state)
        : label_on_(label_on), label_off_(label_off), state_(state) {}

   private:
    // Component implementation.
    ftxui::Element OnRender() override {
        bool is_focused = Focused();
        bool is_active = Active();
        auto style = (is_focused || hovered_) ? ftxui::inverted : is_active ? ftxui::bold : ftxui::nothing;
        auto focus_management = is_focused ? ftxui::focus : is_active ? ftxui::select : ftxui::nothing;

        ftxui::Element my_text = state_ ? ftxui::text(label_on_) : ftxui::text(label_off_);
        return my_text | style | focus_management | ftxui::reflect(box_);
    }

    bool OnEvent(ftxui::Event event) override {
        if (!CaptureMouse(event))
            return false;

        if (event.is_mouse())
            return OnMouseEvent(event);

        hovered_ = false;
        if (event == ftxui::Event::Character(' ') || event == ftxui::Event::Return) {
            state_ = !state_;
            TakeFocus();
            return true;
        }
        return false;
    }

    bool OnMouseEvent(ftxui::Event event) {
        hovered_ = box_.Contain(event.mouse().x, event.mouse().y);

        if (!CaptureMouse(event))
            return false;

        if (!hovered_)
            return false;

        if (event.mouse().button == ftxui::Mouse::Left && event.mouse().motion == ftxui::Mouse::Pressed) {
            state_ = !state_;
            return true;
        }

        return false;
    }

    bool Focusable() const final { return true; }

    const char* label_on_;
    const char* label_off_;
    bool& state_;
    bool hovered_ = false;
    ftxui::Box box_;
};

ftxui::Component MyToggle(const char* label_on, const char* label_off, bool& state) {
    return ftxui::Make<MyToggleImpl>(label_on, label_off, state);
}