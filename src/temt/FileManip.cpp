#include "FileManip.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>


namespace temt{

bool FileManipulation::createNewFile(std::string_view path)
{
    std::filesystem::path relativePath{FileManipulation::clearNonRelativePath(path)};

    //относительные пути?
    if(isExistingPath(relativePath.string()))
        return false;
    std::filesystem::create_directories(std::filesystem::current_path() / relativePath.parent_path());  
    std::ofstream fout {std::filesystem::current_path() / relativePath};
    fout.close();

    return true;
} 


bool FileManipulation::createNewDirectory(std::string_view path)
{
    std::filesystem::path relativePath{FileManipulation::clearNonRelativePath(path)};
    std::filesystem::create_directories(std::filesystem::current_path() / path);

    return true;
}

std::string FileManipulation::clearNonRelativePath(std::string_view path)
{
    std::string path_temp{path};
    size_t pos = 0;
    while((pos = path_temp.find("../", pos)) != std::string::npos)
    {
        path_temp.erase(pos, 3);
    }
    return path_temp;
}



bool FileManipulation::isExistingPath(std::string_view path)
{
    return std::filesystem::exists(clearNonRelativePath(path));
}


bool FileManipulation::isFile(std::string_view path)
{
    return !std::filesystem::is_directory(clearNonRelativePath(path));
}


bool FileManipulation::isDirectory(std::string_view path)
{
    return std::filesystem::is_directory(clearNonRelativePath(path));
}


bool FileManipulation::deletePath(std::string_view path)
{
    return std::filesystem::remove_all(clearNonRelativePath(path));
}

bool FileManipulation::isEmpty(std::string_view path)
{
    return std::filesystem::is_empty(clearNonRelativePath(path));
}

std::vector<std::string> FileManipulation::getDirectoryEntries(std::string_view path)
{
    std::vector<std::string> contents;

    for (const auto& entry : std::filesystem::directory_iterator(std::string(path))) {
        contents.push_back(entry.path().filename().string());
    }

    return contents;
}

std::string FileManipulation::getParentPath(std::string path)
{
    return std::filesystem::path(path).parent_path().string();
}


bool FileManipulation::isArchive(std::string_view path)
{
    std::ifstream file(std::string(path), std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    // Проверяем сигнатуру zip-архива
    char header[4];
    file.read(header, 4);
    if (header[0] == 'P' && header[1] == 'K' && header[2] == '\x03' && header[3] == '\x04') {
        return true;
    }

    return false;
}

std::string FileManipulation::transformPathFromArchiveToFolder(std::string_view path)
{
    std::filesystem::path zipPath{std::string(path)};
    return zipPath.stem().string();
}

}



