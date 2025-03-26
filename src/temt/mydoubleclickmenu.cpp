#include "mydobuleclickmenu.hpp"

#include "ftxui/component/animation.hpp"

using namespace ftxui;

Element DoubleClickMenu::OnRender() {
    // Уменьшаем прогресс подсветки каждый кадр
    if (highlight_progress_ > 0) {
        highlight_progress_ -= 0.05f;
        if (highlight_progress_ < 0) highlight_progress_ = 0;
    }

    Elements elements;
    for (size_t i = 0; i < entries_.size(); ++i) {
        bool is_selected = (static_cast<int>(i) == selected_);
        bool is_highlighted = (static_cast<int>(i) == last_double_clicked_);

        // Плавное изменение цвета подсветки
        auto bg_color = Color::RGB(
            100 + 155 * highlight_progress_,
            100 + 155 * highlight_progress_,
            255);
        
        auto style = (is_highlighted && highlight_progress_ > 0)
            ? bgcolor(bg_color) | bold
            : is_selected
                ? underlined
                : nothing;

        elements.push_back(hbox({text(is_selected ? "> " : "  "), text(entries_[i])}) | style);
    }

    return vbox(elements) | border | vscroll_indicator;
}

bool DoubleClickMenu::OnEvent(Event event) {
    if (event.is_mouse() && event.mouse().button == Mouse::Left && 
        event.mouse().motion == Mouse::Pressed) {
        
        if (event.mouse().y >= 1 && event.mouse().y - 1 < static_cast<int>(entries_.size())) {
            int clicked_item = event.mouse().y - 1;
            auto now = std::chrono::steady_clock::now();
            auto time_since_last_click = 
                std::chrono::duration_cast<std::chrono::milliseconds>(now - last_click_time_).count();

            if (time_since_last_click < 500 && clicked_item == selected_) {
                on_double_click_(selected_);
                last_double_clicked_ = selected_;
                highlight_progress_ = 1.0f;  // Запускаем подсветку
                return true;
            }

            selected_ = clicked_item;
            last_click_time_ = now;
        }
        return true;
    }

    
    return ComponentBase::OnEvent(event);
}