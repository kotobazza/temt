// command_palette.hpp
#pragma once
#include "custom_component.hpp"
#include <vector>
#include <functional>

struct Command {
    std::string name;
    std::string description;
    std::function<void()> action;
    std::string hotkey;
};

class CommandPalette : public CustomComponent {
public:
    CommandPalette();
    
    void AddCommand(Command cmd);
    void Toggle();
    bool IsVisible() const;
    
    ftxui::Element OnRender() override;
    bool OnEvent(ftxui::Event event) override;
    
private:
    std::vector<Command> commands_;
    bool visible_ = false;
    int selected_ = 0;
};