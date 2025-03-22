#include "LogInitializer.hpp"
#include "ArchiveManip.hpp"
#include "FileManip.hpp"
#include "CharProcessing.hpp"
#include "DiskInfo.hpp"

int main() {
    temt::Log::createFileLogger("logs/logs.txt");
    return 0;
}
