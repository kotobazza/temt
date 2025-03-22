cmake_minimum_required(VERSION 3.20)

set(MINOR_VERSION "0" CACHE INTERNAL "Minor version")
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.${MINOR_VERSION}.${PATCH_VERSION})

project(TeMT LANGUAGES C CXX VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_DESCRIPTION "A Terminal Multitool")
set(PROJECT_COPYRIGHT "Copyright (c) 2025 kotobazza")
set(PROJECT_URL "https://github.com/kotobazza/temt")



include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
set(FETCHCONTENT_QUIET FALSE)

# --- FTXUI
FetchContent_Declare(ftxui
  GIT_REPOSITORY "https://github.com/ArthurSonzogni/ftxui"
  GIT_TAG main
  GIT_PROGRESS TRUE
  GIT_SHALLOW
)


FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()



set(BUILD_TESTING OFF CACHE BOOL "Disable testing for libarchive")

# --- libarchive

FetchContent_Declare(
    libarchive
    GIT_REPOSITORY https://github.com/libarchive/libarchive.git
    GIT_TAG master
    GIT_SHALLOW
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(libarchive)


# --- cereal

FetchContent_Declare(
    cereal
    GIT_REPOSITORY https://github.com/USCiLab/cereal.git
    GIT_TAG        master  
    GIT_SHALLOW
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(cereal)



# --- GTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG v1.14.0
    GIT_SHALLOW
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(googletest)


# --- spdlog

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG        v1.15.1 
    GIT_SHALLOW
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable(spdlog)

