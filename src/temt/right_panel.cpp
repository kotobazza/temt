// right_panel.cpp
#include "right_panel.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

RightPanel::RightPanel(int& size) : ResizablePanel("Right Panel", size) {}

Element RightPanel::OnRender() {
    return vbox({text(title_), paragraph("This is the resizable right panel."),
                 hbox({text("Size: "), text(std::to_string(size_) + "%")})}) |
           border | flex;
}