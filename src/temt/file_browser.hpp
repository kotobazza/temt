// file_browser.hpp
#pragma once
#include "custom_component.hpp"
#include <filesystem>
#include <vector>

class FileBrowser : public CustomComponent {
public:
    FileBrowser();
    
    void NavigateTo(const std::filesystem::path& path);
    std::filesystem::path CurrentPath() const;
    std::filesystem::path SelectedFile() const;
    
    ftxui::Element OnRender() override;
    bool OnEvent(ftxui::Event event) override;
    
    // Сигналы
    using OnFileSelect = std::function<void(const std::filesystem::path&)>;
    void SetOnFileSelect(OnFileSelect callback);
    
private:
    void RefreshFiles();
    void MoveSelection(int offset);
    
    std::filesystem::path current_path_;
    std::vector<std::filesystem::directory_entry> files_;
    int selected_index_ = 0;
    OnFileSelect on_file_select_;
};