// resizable_panel.cpp
#include "resizable_panel.hpp"

ResizablePanel::ResizablePanel(const std::string& title, int& size) : title_(title), size_(size) {}

int& ResizablePanel::GetSizeRef() {
    return size_;
}