#include "options_menu.hpp"
#include "text_writer.hpp"
#include <string_view>
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

class OptionsMenuImpl : public ComponentBase {
   public:
    OptionsMenuImpl(temt::AppData& appData, std::function<void(ftxui::Component)> optionSelectedClosure)
        : appData_(appData), optionSelectedClosure_(optionSelectedClosure) {
            Add(Container::Vertical({
                Container::Horizontal({
                    Button("Open\nText Writer", [this](){
                        appData_.toggleOptionsMenu();
                        optionSelectedClosure_(TextWriter("", appData_.current_path_, [](){}));
                    }),
                    Renderer([](){
                        return paragraph("Opens a Text Writer applet that lets read and write text into character files")|vcenter;
                    })
                }),
                Container::Horizontal({
                    Button("Exit", [this](){appData_.toggleOptionsMenu();})
                })
            })|border);
        }

   private:
    temt::AppData& appData_;
    std::function<void(Component)> optionSelectedClosure_;
};

ftxui::Component OptionsMenu(temt::AppData& appData, std::function<void(ftxui::Component)> optionSelectedClosure) {
    return Make<OptionsMenuImpl>(appData, optionSelectedClosure);
}