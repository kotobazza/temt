#include <filesystem>
#include <fstream>
#include <iostream>

#include <archive.h>
#include <archive_entry.h>

#include "ArchiveManip.hpp"

namespace temt {
namespace ArchiveManip {
/*Creates zip archive given by entry names (only files, not dirs) and path for save (shouldn't exist before)*/
FileManip::ActionState createNewZipArchive(std::vector<FileManip::FileInfo> entries, std::string_view path) {
    if (FileManip::isExistingPath(path))
        return FileManip::ActionState::Exists;

    struct archive* a = archive_write_new();

    if (archive_write_set_format_zip(a) != ARCHIVE_OK) {
        // needs logger
        std::cerr << "Failed to set archive format to ZIP" << std::endl;
        return FileManip::ActionState::Failed;
    }

    if (archive_write_open_filename(a, path.data()) != ARCHIVE_OK) {
        std::cerr << "Failed to open archive for writing" << std::endl;
        return FileManip::ActionState::Unavailable;
    }

    for (const auto& entry : entries) {
        std::filesystem::path p(entry.path);
        if (!std::filesystem::exists(p)) {
            std::cerr << "Entry '" << entry.path << "' does not exist" << std::endl;
            continue;
        }

        struct archive_entry* entry_info = archive_entry_new();
        archive_entry_copy_pathname(entry_info, entry.path.c_str());
        archive_entry_set_size(entry_info, std::filesystem::file_size(p));
        archive_entry_set_filetype(entry_info, S_IFREG);
        archive_entry_set_perm(entry_info, 0644);

        if (archive_write_header(a, entry_info) != ARCHIVE_OK) {
            std::cerr << "Failed to write header for entry '" << entry.path << "'" << std::endl;
            archive_entry_free(entry_info);
            continue;
        }

        std::ifstream file(entry.path, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file '" << entry.path << "'" << std::endl;
            archive_entry_free(entry_info);
            continue;
        }

        char buf[8192];
        size_t bytes_read;
        while ((bytes_read = file.read(buf, sizeof(buf)).gcount()) > 0) {
            if (archive_write_data(a, buf, bytes_read) != static_cast<ssize_t>(bytes_read)) {
                std::cerr << "Failed to write data for entry '" << entry.path << "'" << std::endl;
                archive_entry_free(entry_info);
                return FileManip::ActionState::Failed;
            }
        }

        archive_entry_free(entry_info);
    }

    if (archive_write_close(a) != ARCHIVE_OK) {
        std::cerr << "Failed to close archive" << std::endl;
        return FileManip::ActionState::Failed;
    }

    archive_write_free(a);
    return FileManip::ActionState::Done;
}

std::string transformPathFromArchiveToFolder(std::string_view path) {
    std::filesystem::path zipPath{path.data()};
    return zipPath.stem().string();
}

bool isArchive(std::string_view path) {
    std::ifstream file(path.data(), std::ios::binary);
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

ArchiveReadingResults readArchiveEntries(std::string_view zipFilePath) {
    std::vector<std::string> fileList;
    struct archive* a = archive_read_new();
    archive_read_support_format_zip(a);

    if (archive_read_open_filename(a, zipFilePath.data(), 10240) != ARCHIVE_OK) {
        archive_read_free(a);
        return ArchiveReadingResults{FileManip::ActionState::Failed, {}};
    }

    struct archive_entry* entry;
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        fileList.push_back(archive_entry_pathname(entry));
        archive_read_data_skip(a);
    }

    archive_read_free(a);
    return ArchiveReadingResults{FileManip::ActionState::Done, fileList};
}

FileManip::ActionState unzipArchive(std::string_view zipFilePath, std::string pathToExtract) {
    struct archive* a = archive_read_new();
    archive_read_support_format_zip(a);

    // Get the name of the ZIP file without the extension
    std::filesystem::path zipPath{std::string(zipFilePath)};
    std::string directoryName = zipPath.stem().string();

    // Create the directory for the extracted files
    std::filesystem::path extractionDir = std::filesystem::path{std::string(pathToExtract)} / directoryName;
    std::filesystem::create_directory(extractionDir);

    if (archive_read_open_filename(a, std::string(zipFilePath).c_str(), 10240) != ARCHIVE_OK) {
        archive_read_free(a);
        return FileManip::ActionState::Failed;
    }

    struct archive_entry* entry;
    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        std::filesystem::path entryPath = extractionDir / archive_entry_pathname(entry);

        if (archive_entry_filetype(entry) == AE_IFDIR) {
            std::filesystem::create_directories(entryPath);
        } else {
            std::filesystem::create_directories(entryPath.parent_path());

            std::ofstream outFile(entryPath, std::ios::binary);
            if (outFile.is_open()) {
                std::vector<char> buffer(10240);
                ssize_t bytes_read;
                while ((bytes_read = archive_read_data(a, buffer.data(), buffer.size())) > 0) {
                    outFile.write(buffer.data(), bytes_read);
                }
                outFile.close();
            }
        }

        archive_read_data_skip(a);
    }

    archive_read_free(a);

    return FileManip::ActionState::Done;
}

}  // namespace ArchiveManip
}  // namespace temt
