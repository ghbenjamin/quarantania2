# Config

cmake_minimum_required(VERSION 3.14)
project(quarantania2)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)


# Static library shared between test & main code

include_directories(include)

file(GLOB_RECURSE CPP_SOURCE_LIST "src/*.cpp")
add_library( quarantania2_lib STATIC ${CPP_SOURCE_LIST} )

# VCPKG Dependencies
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(quarantania2_lib PRIVATE SDL2::SDL2main SDL2::SDL2-static)

find_package(freetype CONFIG REQUIRED)
target_link_libraries(quarantania2_lib PRIVATE freetype)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(quarantania2_lib PRIVATE nlohmann_json nlohmann_json::nlohmann_json)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(quarantania2_lib PRIVATE fmt::fmt fmt::fmt-header-only)

find_package(ZLIB REQUIRED)
target_link_libraries(quarantania2_lib PRIVATE ZLIB::ZLIB)

find_package(glad CONFIG REQUIRED)
target_link_libraries(quarantania2_lib PRIVATE glad::glad)

find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(quarantania2_lib PRIVATE ${STB_INCLUDE_DIRS})

find_package(glm CONFIG REQUIRED)
target_link_libraries(quarantania2_lib PRIVATE glm::glm)


# Main executable

add_executable(quarantania2 "src/main.cpp" )
target_link_libraries( quarantania2 PRIVATE quarantania2_lib )


# Test executable

file(GLOB_RECURSE TEST_SOURCE_LIST "test/*.cpp")
add_executable(quarantania2_tests ${TEST_SOURCE_LIST})

find_package(Catch2 REQUIRED)
target_link_libraries(quarantania2_tests PRIVATE Catch2::Catch2)
target_link_libraries(quarantania2_tests PRIVATE quarantania2_lib)

set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
include(CTest)
include(Catch)
catch_discover_tests(quarantania2_tests)
