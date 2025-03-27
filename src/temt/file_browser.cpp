// file_browser.cpp
#include "file_browser.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

FileBrowser::FileBrowser() {
    NavigateTo(std::filesystem::current_path());
}

void FileBrowser::NavigateTo(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) return;
    
    current_path_ = std::filesystem::canonical(path);
    RefreshFiles();
}

void FileBrowser::RefreshFiles() {
    files_.clear();
    selected_index_ = 0;
    
    if (current_path_.has_parent_path()) {
        files_.emplace_back(current_path_.parent_path());
    }
    
    for (const auto& entry : std::filesystem::directory_iterator(
        current_path_, 
        std::filesystem::directory_options::skip_permission_denied)) {
        files_.push_back(entry);
    }
}

bool FileBrowser::OnEvent(Event event) {
    if (!Focused()) return false;
    
    if (event == Event::ArrowUp || event == Event::Character('k')) {
        MoveSelection(-1);
        return true;
    }
    else if (event == Event::ArrowDown || event == Event::Character('j')) {
        MoveSelection(1);
        return true;
    }
    else if (event == Event::Return) {
        if (!files_.empty()) {
            const auto& entry = files_[selected_index_];
            if (entry.is_directory()) {
                NavigateTo(entry.path());
            }
            else if (on_file_select_) {
                on_file_select_(entry.path());
            }
        }
        return true;
    }
    
    return CustomComponent::OnEvent(event);
}

void FileBrowser::MoveSelection(int offset) {
    if (files_.empty()) return;
    
    selected_index_ += offset;
    if (selected_index_ < 0) selected_index_ = 0;
    if (selected_index_ >= static_cast<int>(files_.size())) 
        selected_index_ = files_.size() - 1;
    
    if (on_file_select_ && !files_[selected_index_].is_directory()) {
        on_file_select_(files_[selected_index_].path());
    }
}

Element FileBrowser::OnRender() {
    Elements list;
    
    for (size_t i = 0; i < files_.size(); ++i) {
        const auto& entry = files_[i];
        auto style = (static_cast<int>(i) == selected_index_) 
            ? (Focused() ? inverted : bold) 
            : nothing;
        
        auto prefix = entry.is_directory() ? "📁 " : "📄 ";
        list.push_back(text(prefix + entry.path().filename().string()) | style);
    }
    
    return vbox({
        text("Path: " + current_path_.string()) | bold,
        separator(),
        vbox(list) | frame | flex | border | focus
    });
}

void FileBrowser::SetOnFileSelect(OnFileSelect callback) {
    on_file_select_ = callback;
}