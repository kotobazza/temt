#include <chrono>

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"

class DoubleClickMenu : public ftxui::ComponentBase {
   public:
    DoubleClickMenu(std::vector<std::string>& vals) {
        entries_ = vals;
        menu_ = ftxui::Menu(&entries_, &selected_);
        Add(menu_);
    };
    ftxui::Element OnRender() override;
    bool OnEvent(ftxui::Event event) override;

    std::function<void(int)> on_double_click_ = [](int) {};

   private:
    std::vector<std::string> entries_;
    int selected_ = 0;
    ftxui::Component menu_;
    std::chrono::steady_clock::time_point last_click_time_;
    int last_double_clicked_ = 0;
    float highlight_progress_ = 0.0f;
};
