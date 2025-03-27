// command_palette.cpp
#include "command_palette.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

CommandPalette::CommandPalette() {
    // Базовые команды
    commands_ = {
        {"help", "Show help", []{}, "F1"},
        {"quit", "Quit application", []{}, "Ctrl+Q"},
        {"refresh", "Refresh view", []{}, "F5"}
    };
}

void CommandPalette::AddCommand(Command cmd) {
    commands_.push_back(cmd);
}

void CommandPalette::Toggle() {
    visible_ = !visible_;
    if (visible_) {
        TakeFocus();
        selected_ = 0;
    }
}

bool CommandPalette::IsVisible() const {
    return visible_;
}

bool CommandPalette::OnEvent(Event event) {
    if (!visible_) return false;
    
    if (event == Event::Escape) {
        Toggle();
        return true;
    }
    else if (event == Event::ArrowUp || event == Event::Character('k')) {
        selected_ = (selected_ - 1 + commands_.size()) % commands_.size();
        return true;
    }
    else if (event == Event::ArrowDown || event == Event::Character('j')) {
        selected_ = (selected_ + 1) % commands_.size();
        return true;
    }
    else if (event == Event::Return) {
        commands_[selected_].action();
        Toggle();
        return true;
    }
    
    return false;
}

Element CommandPalette::OnRender() {
    if (!visible_) return text("");
    
    Elements list;
    for (size_t i = 0; i < commands_.size(); ++i) {
        const auto& cmd = commands_[i];
        auto style = (static_cast<int>(i) == selected_) ? inverted : nothing;
        
        list.push_back(
            hbox({
                text(" " + cmd.name) | style | flex_shrink,
                text(" - " + cmd.description) | dim,
                text(" (" + cmd.hotkey + ")") | align_right | dim
            })
        );
    }
    
    return vbox({
        text(" Command Palette ") | bold | center,
        separator(),
        vbox(list) | frame | size(HEIGHT, LESS_THAN, 10),
        separator(),
        text("↑/↓: Navigate • Enter: Execute • Esc: Close") | dim | center
    }) | border | clear_under | center;
}