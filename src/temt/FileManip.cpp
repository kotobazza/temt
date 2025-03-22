#include "FileManip.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>

namespace temt {

namespace FileManip {

ActionState createNewFile(std::string_view path) {
    std::filesystem::path relativePath{clearNonRelativePath(path)};

    if (isExistingPath(relativePath.string()))
        return ActionState::NotExists;
    std::filesystem::create_directories(std::filesystem::current_path() / relativePath.parent_path());
    std::ofstream fout{std::filesystem::current_path() / relativePath};
    fout.close();

    return ActionState::Done;
}

ActionState createNewDirectory(std::string_view path) {
    std::filesystem::path relativePath{clearNonRelativePath(path)};
    std::filesystem::create_directories(std::filesystem::current_path() / path);

    return ActionState::Done;
}

std::string clearNonRelativePath(std::string_view path) {
    std::string path_temp{path};
    size_t pos = 0;
    while ((pos = path_temp.find("../", pos)) != std::string::npos) {
        path_temp.erase(pos, 3);
    }
    return path_temp;
}

bool isExistingPath(std::string_view path) {
    return std::filesystem::exists(clearNonRelativePath(path));
}

// TODO: regular_file
bool isFile(std::string_view path) {
    return !std::filesystem::is_directory(clearNonRelativePath(path));
}

bool isDirectory(std::string_view path) {
    return std::filesystem::is_directory(clearNonRelativePath(path));
}

//ActionState deletePath(std::string_view path) { return ActionState::Unavailable;}

bool isEmpty(std::string_view path) {
    return std::filesystem::is_empty(clearNonRelativePath(path));
}

std::vector<FileInfo> readDirectoryFlatEntries(std::string_view path) {
    std::vector<FileInfo> contents;

    for (const auto& entry : std::filesystem::directory_iterator(std::string(path))) {
        contents.push_back(FileInfo{entry.path().filename().string(), getParentPath(entry.path().string()),
                            identifyFileType(entry.path().string())});
    }

    return contents;
}

std::vector<FileInfo> readDirectoryRecursiveEntries(std::string_view path) {
    std::vector<FileInfo> entries;
    std::stack<std::filesystem::path> dirStack;
    dirStack.push(path);

    while (!dirStack.empty()) {
        std::filesystem::path currentDir = dirStack.top();
        dirStack.pop();

        for (const auto& entry : std::filesystem::directory_iterator(currentDir)) {
            if (std::filesystem::is_directory(entry)) {
                dirStack.push(entry);
            } else {
                entries.push_back(FileInfo{entry.path().filename().string(), getParentPath(entry.path().string()),
                                   identifyFileType(entry.path().string())});
            }
        }
    }

    return entries;
}

std::string getParentPath(std::string_view path) {
    return std::filesystem::path(path).parent_path().string();
}

FileType identifyFileType(std::string_view path) {
    std::filesystem::directory_entry entry(path);

    // directory_entries can't be used in switches

    FileType type;

    if (entry.exists()) {
        if (entry.is_regular_file()) {
            type = FileType::Regular;
        } else if (entry.is_directory()) {
            type = FileType::Directory;
        } else if (entry.is_symlink()) {
            type = FileType::Symlick;
        } else if (entry.is_block_file()) {
            type = FileType::Block;
        } else if (entry.is_character_file()) {
            type = FileType::Character;
        } else if (entry.is_fifo()) {
            type = FileType::Pipe;
        } else if (entry.is_socket()) {
            type = FileType::Socket;
        } else {
            type = FileType::Other;
        }
    } else {
        type = FileType::None;
    }

    return type;
}

}  // namespace FileManip

}  // namespace temt