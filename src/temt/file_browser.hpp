// file_browser.hpp
#pragma once
#include <filesystem>
#include <vector>
#include <functional>
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"

class FileBrowser : public ftxui::ComponentBase{
public:
    FileBrowser();
    
    ftxui::Element OnRender();
    void NavigateTo(const std::filesystem::path& path);
    std::filesystem::path CurrentPath() const;
    std::filesystem::path SelectedFile() const;
    
    // Сигналы
    using OnFileSelect = std::function<void(const std::filesystem::path&)>;
    void SetOnFileSelect(OnFileSelect callback);
    
private:
    void RefreshFiles();
    
    std::filesystem::path current_path_;
    std::vector<std::filesystem::directory_entry> files_;
    std::vector<std::string> file_names_;
    int selected_index_ = 0;
    ftxui::Component menu_;
    OnFileSelect on_file_select_;
};