// file_preview.cpp
#include "file_preview.hpp"
#include "ftxui/dom/elements.hpp"
#include <fstream>

using namespace ftxui;

FilePreview::FilePreview() {
    content_ = "Select a file to preview";
}

void FilePreview::SetFile(const std::filesystem::path& file) {
    current_file_ = file;
    
    if (!std::filesystem::exists(file) || std::filesystem::is_directory(file)) {
        content_ = "Not a file or doesn't exist";
        return;
    }
    
    std::ifstream in(file);
    if (!in) {
        content_ = "Cannot open file";
        return;
    }
    
    content_.assign(std::istreambuf_iterator<char>(in), 
                   std::istreambuf_iterator<char>());
    
    if (content_.size() > 1000) {
        content_ = content_.substr(0, 1000) + "\n... (truncated)";
    }
}

Element FilePreview::OnRender() {
    return vbox({
        text("Preview: " + current_file_.filename().string()) | bold,
        separator(),
        paragraph(content_) | flex | frame | border | focus
    });
}