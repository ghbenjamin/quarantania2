cmake_minimum_required(VERSION 3.14)
project(quarantania2)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB_RECURSE CPP_SOURCE_LIST "src/*.cpp")

include_directories(include)

add_executable(quarantania2 ${CPP_SOURCE_LIST})

add_compile_definitions(USE_GL)

if(MSVC)
    # Disable spurious warning from old version of RapidJson
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
else()
   # Nothing yet
endif()

# VCPKG Dependencies
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(quarantania2 PRIVATE SDL2::SDL2main SDL2::SDL2-static)

find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(quarantania2 PRIVATE SDL2::SDL2_image)

find_package(sdl2-ttf CONFIG REQUIRED)
target_link_libraries(quarantania2 PRIVATE SDL2::SDL2_ttf)

find_package(RapidJSON CONFIG REQUIRED)
target_include_directories(quarantania2 PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

find_package(fmt CONFIG REQUIRED)
target_link_libraries(quarantania2 PRIVATE fmt::fmt fmt::fmt-header-only)

find_package(ZLIB REQUIRED)
target_link_libraries(quarantania2 PRIVATE ZLIB::ZLIB)

find_package(glad CONFIG REQUIRED)
target_link_libraries(quarantania2 PRIVATE glad::glad)

find_path(STB_INCLUDE_DIRS "stb.h")
target_include_directories(quarantania2 PRIVATE ${STB_INCLUDE_DIRS})