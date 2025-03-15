cmake_minimum_required(VERSION 3.14)

set(MINOR_VERSION "1" CACHE INTERNAL "Minor version")
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.${MINOR_VERSION}.${PATCH_VERSION})

project(TeMT LANGUAGES C CXX VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(PROJECT_DESCRIPTION "A Terminal Multitool")
set(PROJECT_COPYRIGHT "Copyright (c) 2025 kotobazza")
set(PROJECT_URL "https://github.com/kotobazza/temt")

set(BUILD_TESTING OFF CACHE BOOL "Disable testing for libarchive")

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


get_property(all_targets GLOBAL PROPERTY TARGETS)
foreach(target ${all_targets})
    if (target MATCHES "test_.*")  # Предполагаем, что тестовые цели начинаются с "test_"
        set_target_properties(${target} PROPERTIES EXCLUDE_FROM_ALL TRUE)
    endif()
endforeach()



add_executable(main src/main.cpp)
add_executable(tests tests/tests.cpp)

target_link_libraries(
    main 
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component 
    PRIVATE archive
)

target_include_directories(main PRIVATE ${cereal_SOURCE_DIR}/include)


target_link_libraries(
    tests
    PRIVATE GTest::gtest_main
    PRIVATE archive
)

target_include_directories(tests PRIVATE ${cereal_SOURCE_DIR}/include)


set_target_properties(main PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)


file(GLOB headers "src/temt/*.hpp")
foreach(file ${headers})
  target_sources(main PUBLIC ${file})
endforeach()

file(GLOB sources "src/temt/*.cpp")
foreach(file ${sources})
  target_sources(main PRIVATE ${file})
endforeach()

target_include_directories(main
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
    target_compile_options(main PRIVATE /W4)
else ()
    target_compile_options(main PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

# Установка целевых файлов
install(TARGETS main RUNTIME DESTINATION bin COMPONENT cli)

# Настройки CPack
set(CPACK_PACKAGE_NAME "temt")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "lumenos.kotobazza@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION "A Terminal MultiTool")
set(CPACK_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Tools")
set(CPACK_GENERATOR "RPM;DEB") 

include(CPack)


enable_testing()
add_test(tests tests)


include(GoogleTest)
gtest_discover_tests(tests)

add_custom_target(run_tests
  COMMAND ctest --test-dir ${CMAKE_BINARY_DIR} -R tests
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Running tests with ctest -R tests"
)

