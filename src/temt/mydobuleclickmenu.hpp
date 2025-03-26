#include <chrono>

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"


//TODO: ftxui::Component::Menu is not needed now as OnRender renders whole menu structure without any need of menu class itself
class DoubleClickMenu : public ftxui::ComponentBase {
   public:
    DoubleClickMenu(std::vector<std::string>& vals, std::function<void(int)>callback) {
        on_double_click_ = callback;
        entries_ = vals;
        menu_ = ftxui::Menu(&entries_, &selected_);
        Add(menu_);
    };
    ftxui::Element OnRender() override;
    bool OnEvent(ftxui::Event event) override;
    void update_timerwait();

    std::function<void(int)> on_double_click_ = [](int) {};



   private:
    std::vector<std::string> entries_;
    int selected_ = 0;
    ftxui::Component menu_;
    std::chrono::steady_clock::time_point last_click_time_;
    int last_double_clicked_ = 0;
    float highlight_progress_ = 0.0f;
    std::chrono::steady_clock::time_point waiting_end_;
};
