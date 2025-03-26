#include "mydobuleclickmenu.hpp"



using namespace ftxui;



Element DoubleClickMenu::OnRender() {
    return menu_->Render() | border | vscroll_indicator;
}

bool DoubleClickMenu::OnEvent(Event event) {
    // Обработка двойного клика
    if (event.is_mouse() && event.mouse().button == Mouse::Left && event.mouse().motion == Mouse::Pressed) {
        auto now = std::chrono::steady_clock::now();
        auto time_since_last_click =
            std::chrono::duration_cast<std::chrono::milliseconds>(now - last_click_time_).count();

        // Если интервал между кликами < 500 мс — это двойной клик
        if (time_since_last_click < 500) {
            on_double_click_(selected_);  // Вызываем колбэк
            last_click_time_ = {};        // Сбрасываем таймер
            return true;
        }

        last_click_time_ = now;
    }

    // Передаём событие дочернему меню
    return ComponentBase::OnEvent(event);
}


