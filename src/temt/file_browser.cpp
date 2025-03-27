// file_browser.cpp
#include "file_browser.hpp"
#include "ftxui/component/event.hpp"
#include "ftxui/dom/elements.hpp"

using namespace ftxui;

FileBrowser::FileBrowser() {
    menu_ = Menu(&file_names_, &selected_index_, MenuOption::Vertical());
    NavigateTo(std::filesystem::current_path());
    Add(menu_);
}

ftxui::Element FileBrowser::OnRender() {
    return vbox({menu_->Render()});
}

void FileBrowser::NavigateTo(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path))
        return;

    current_path_ = std::filesystem::canonical(path);
    RefreshFiles();
}

void FileBrowser::RefreshFiles() {
    files_.clear();
    file_names_.clear();
    selected_index_ = 0;

    // Добавляем переход в родительскую директорию
    if (current_path_.has_parent_path()) {
        files_.emplace_back(current_path_.parent_path());
        file_names_.push_back(".. (parent directory)");
    }

    // Собираем файлы и директории
    for (const auto& entry : std::filesystem::directory_iterator(
             current_path_, std::filesystem::directory_options::skip_permission_denied)) {
        files_.push_back(entry);
        std::string prefix = entry.is_directory() ? "📁 " : "📄 ";
        file_names_.push_back(prefix + entry.path().filename().string());
    }

    // Если есть подписчик - уведомляем о новом выборе
    if (!files_.empty() && on_file_select_) {
        on_file_select_(files_[selected_index_].path());
    }
}

std::filesystem::path FileBrowser::CurrentPath() const {
    return current_path_;
}

std::filesystem::path FileBrowser::SelectedFile() const {
    if (files_.empty())
        return {};
    return files_[selected_index_].path();
}

void FileBrowser::SetOnFileSelect(OnFileSelect callback) {
    on_file_select_ = callback;

    // Настраиваем обработчик выбора в меню
    menu_ |= CatchEvent([this](Event event) {
        if (event == Event::Return) {
            if (!files_.empty()) {
                const auto& entry = files_[selected_index_];
                if (entry.is_directory()) {
                    NavigateTo(entry.path());
                } else if (on_file_select_) {
                    on_file_select_(entry.path());
                }
            }
            return true;
        }
        return false;
    });
}