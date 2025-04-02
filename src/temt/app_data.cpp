#include "app_data.hpp"

namespace temt {
void AppData::NavigateToPath(std::string_view path) {
    usingDirectoriesSelected_ = 0;
    usingDirectoryEntries_.clear();

    for (auto entry : temt::FileManip::readDirectoryFlatEntries(path)) {
        usingDirectoryEntries_.push_back(entry);
    }

    current_path_ = path;

    notify();
}

void AppData::notify() {
    for (auto& listener : listeners) {
        listener();
    }
}
}  // namespace temt