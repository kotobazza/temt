// resizable_panel.hpp
#pragma once

#include <string>
#include <memory>
#include "ftxui/component/component.hpp"

class ResizablePanel {
public:
    ResizablePanel(const std::string& title, int& size);
    
    virtual ftxui::Component GetComponent() = 0;
    virtual ftxui::Element Render() = 0;
    
    void SetSize(int& size);
    int GetSize() const;
    int& GetSizeRef();
    std::string GetTitle() const;

protected:
    std::string title_;
    int& size_; // размер в процентах
};