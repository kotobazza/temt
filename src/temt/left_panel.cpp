// left_panel.cpp
#include "left_panel.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

LeftPanel::LeftPanel(int& size) : ResizablePanel("Left Panel", size) {}

Component LeftPanel::GetComponent() {
    return Renderer([this] { return Render(); });
}

Element LeftPanel::Render() {
    return vbox({text(title_), paragraph("This is the resizable left panel."),
                 hbox({text("Size: "), text(std::to_string(size_) + "%")})}) |
           border | flex;
}