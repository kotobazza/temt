// file_preview.hpp
#pragma once
#include "custom_component.hpp"
#include <filesystem>

class FilePreview : public CustomComponent {
public:
    FilePreview();
    
    void SetFile(const std::filesystem::path& file);
    ftxui::Element OnRender() override;
    
private:
    std::filesystem::path current_file_;
    std::string content_;
};