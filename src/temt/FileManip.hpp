#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace temt {
namespace FileManip {

enum class FileType { None, Block, Character, Directory, Socket, Pipe, Regular, Symlick, Other };

enum class ActionState {
    Done,
    IsDirectory,
    IsFile,
    NotEmpty,
    Empty,
    NoPermissions,
    NoWritePermissions,
    NotExists,
    Exists,
    Unavailable,
    Failed,
    ArchiveZipTypeNotSet,
    XmlFailed,
    JsonFailed,
    ParseFailed,
    StructFailed
};

struct FileInfo {
    std::string path;
    std::string parentDirectory;
    FileType type;
    std::string perms{""};
    std::string owner{""};
};

std::string clearNonRelativePath(std::string_view path);
ActionState createNewFile(std::string_view path);
ActionState createNewDirectory(std::string_view path);

ActionState deletePath(std::string_view path);

std::vector<FileInfo> readDirectoryFlatEntries(std::string_view path);
std::vector<FileInfo> readDirectoryRecursiveEntries(std::string_view path);

std::string getParentPath(std::string_view path);

bool isExistingPath(std::string_view path);
bool isDirectory(std::string_view path);
bool isFile(std::string_view path);
bool isEmpty(std::string_view path);

FileType identifyFileType(std::string_view path);

}  // namespace FileManip

}  // namespace temt
