#include "options_menu.hpp"


using namespace ftxui;

class OptionsMenuImpl : public ComponentBase {
    OptionsMenuImpl(temt::AppData& appData, std::function<void()> optionSelectedClosure) : appData_(appData), optionSelectedClosure_(optionSelectedClosure) {
        
    }

    temt::AppData& appData_;
    std::function<void()> optionSelectedClosure_;  //optionSelectedClosure should return using component in mainPanel_. Now i need to implement an using component to create full ui
};



ftxui::Component OptionsMenu(temt::AppData& appData, std::function<void()> optionSelectedClosure){
    return Make<OptionsMenuImpl>(appData, optionSelectedClosure);
}