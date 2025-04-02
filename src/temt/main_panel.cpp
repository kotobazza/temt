#include "main_panel.hpp"

using namespace ftxui;

class Impl : public ComponentBase {
   public:
    Impl() {
        mainContainer_ = Container::Vertical({
            Renderer([](){return text("MainPanel");})
        });
        Add(mainContainer_);
    }

   private:
    ftxui::Component mainContainer_;
};

ftxui::Component MainPanel() {
    return Make<Impl>() | flex_grow| border;
};