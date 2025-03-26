// center_panel.cpp
#include "center_panel.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

CenterPanel::CenterPanel(int& size) : ResizablePanel("Center Panel", size) {}

Component CenterPanel::GetComponent() {
    return Renderer([this] { return Render(); });
}

Element CenterPanel::Render() {
    return vbox({
        text(title_),
        paragraph("This is the central panel. It's always visible."),
        text("Other panels can be toggled with buttons above.")
    }) | border | flex;
}