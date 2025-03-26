// resizable_panel.cpp
#include "resizable_panel.hpp"

ResizablePanel::ResizablePanel(const std::string& title, int& size) : title_(title), size_(size) {}

void ResizablePanel::SetSize(int& size) {
    size_ = size;
}

int ResizablePanel::GetSize() const {
    return size_;
}

std::string ResizablePanel::GetTitle() const {
    return title_;
}

int& ResizablePanel::GetSizeRef() {
    return size_;
}