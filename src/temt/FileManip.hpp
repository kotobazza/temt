#pragma once 
#include <string>
#include <string_view>
#include <vector>




namespace temt{

enum FileType
{
    File,
    Directory,
    Socket,
    Pipe,
    Device
};

struct FileInfo
{
    std::string path;
    std::string perms;
    std::string owner;
    std::string includingDirectory;
    FileType type;
};

enum ActionState{
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


class FileManipulation
{
public:
    static std::string clearNonRelativePath(std::string_view path);
    static bool createNewFile(std::string_view path);
    static bool createNewDirectory(std::string_view path);

    static bool deletePath(std::string_view path);

    static std::vector<std::string> getDirectoryEntries(std::string_view path);
    static std::string getParentPath(std::string path);

    static bool isExistingPath(std::string_view path);
    static bool isDirectory(std::string_view path);
    static bool isFile(std::string_view path);
    static bool isEmpty(std::string_view path);
    static bool isArchive(std::string_view path);

    static std::string transformPathFromArchiveToFolder(std::string_view path);
    
};


}
