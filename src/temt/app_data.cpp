#include "app_data.hpp"

namespace temt {
void AppData::NavigateToPath(std::string_view path) {
    try {
        usingDirectorySelected_ = 0;
        usingDirectoryEntries_.clear();

        auto entries = temt::FileManip::readDirectoryFlatEntries(path);
        std::copy(entries.begin(), entries.end(), std::back_inserter(usingDirectoryEntries_));

        current_path_ = path;

        notify();
    } catch (const std::exception& e) {
        file_logger_->error("Navigation failed: {}", e.what());
        // TODO: restoration?????
    }
}

void AppData::notify() {
    for (const auto& listener : listeners) {
        listener();
    }
}
}  // namespace temt