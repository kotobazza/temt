// right_panel.cpp
#include "right_panel.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

RightPanel::RightPanel(int& size) : ResizablePanel("Right Panel", size) {}

Component RightPanel::GetComponent() {
    return Renderer([this] { return Render(); });
}

Element RightPanel::Render() {
    return vbox({
        text(title_),
        paragraph("This is the resizable right panel."),
        hbox({text("Size: "), text(std::to_string(100-size_) + "%")})
    }) | border | flex;
}