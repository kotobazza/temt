cmake_minimum_required(VERSION 3.20)



# -------- Project definition
SET(MAJOR_VERSION "1" CACHE INTERNAL "Major version")
set(MINOR_VERSION "0" CACHE INTERNAL "Minor version")
set(PATCH_VERSION "0" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

set(PROJECT_DESCRIPTION "A Terminal Multitool")
set(PROJECT_COPYRIGHT "Copyright (c) 2025 kotobazza")
set(PROJECT_URL "https://github.com/kotobazza/temt")

project(TeMT LANGUAGES CXX VERSION ${PROJECT_VERSION})



# -------- Libraries fetching
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


# --- libarchive
set(ENABLE_TEST OFF CACHE BOOL "Disable tests from external projects" FORCE)

FetchContent_Declare(
    libarchive
    GIT_REPOSITORY https://github.com/libarchive/libarchive.git
    GIT_TAG master
    GIT_SHALLOW
    CMAKE_ARGS -DENABLE_TESTING=OFF
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



# -------- Executables
add_executable(temt src/main.cpp)
add_executable(tests tests/tests.cpp)

target_compile_features(temt PRIVATE cxx_std_20)
set_target_properties(temt PROPERTIES
    CXX_EXTENSIONS OFF
)

target_compile_features(tests PRIVATE cxx_std_20)
set_target_properties(tests PROPERTIES
    CXX_EXTENSIONS OFF
)


target_link_libraries(
    temt 
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component 
    PRIVATE archive
    PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
)

target_include_directories(temt PRIVATE ${cereal_SOURCE_DIR}/include)

target_link_libraries(
    tests
    PRIVATE GTest::gtest_main
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component 
    PRIVATE archive
    PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
)

target_include_directories(tests PRIVATE ${cereal_SOURCE_DIR}/include)

file(GLOB headers "src/temt/*.hpp")
foreach(file ${headers})
  target_sources(temt PUBLIC ${file})
endforeach()

file(GLOB sources "src/temt/*.cpp")
foreach(file ${sources})
  target_sources(temt PRIVATE ${file})
endforeach()

target_include_directories(temt
PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/temt
)


file(GLOB headers "src/temt/*.hpp")
foreach(file ${headers})
  target_sources(tests PUBLIC ${file})
endforeach()

file(GLOB sources "src/temt/*.cpp")
foreach(file ${sources})
  target_sources(tests PRIVATE ${file})
endforeach()

target_include_directories(tests
PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/temt
)

if (MSVC)
    target_compile_options(temt PRIVATE /O2 /W4)
    target_compile_options(tests PRIVATE /O2 /W4)
else ()
    target_compile_options(temt PRIVATE -O3 -Wall -Wextra -pedantic -Werror)
    target_compile_options(tests PRIVATE -O3 -Wall -Wextra -pedantic -Werror)
endif()



# -------- Installation properties
install(TARGETS temt RUNTIME DESTINATION bin)



# -------- CPack 
set(CPACK_PACKAGE_NAME "temt")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "lumenos.kotobazza@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION "A Terminal MultiTool")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
set(CPACK_GENERATOR "RPM;DEB") 

# -------- Final Packaging Setup
if(WIN32)
    set(CPACK_GENERATOR "NSIS")
elseif(UNIX AND EXISTS "/etc/fedora-release")
    set(CPACK_GENERATOR "RPM")
elseif(UNIX AND EXISTS "/etc/lsb-release")
    set(CPACK_GENERATOR "DEB")
endif()

set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")

include(CPack)



# -------- Tests
enable_testing()
add_test(tests tests)

include(GoogleTest)
gtest_discover_tests(tests)
