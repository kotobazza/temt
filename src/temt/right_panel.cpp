// right_panel.cpp
#include "right_panel.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

RightPanel::RightPanel() : ResizablePanel("Right Panel") {
    size_ = 70; // Начальный размер правой панели
}

Component RightPanel::GetComponent() {
    return Renderer([this] { return Render(); });
}

Element RightPanel::Render() {
    return vbox({
        text(title_),
        paragraph("This is the resizable right panel."),
        hbox({text("Size: "), text(std::to_string(size_) + "%")})
    }) | border | flex;
}