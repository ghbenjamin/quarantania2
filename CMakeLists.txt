cmake_minimum_required(VERSION 3.14)
project(quarantania2)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE CPP_SOURCE_LIST "src/*.cpp")

include_directories(include)

add_executable(quarantania2 ${CPP_SOURCE_LIST})

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(quarantania2 PRIVATE SDL2::SDL2main SDL2::SDL2-static)

find_package(sdl2-image CONFIG REQUIRED)
target_link_libraries(quarantania2 PRIVATE SDL2::SDL2_image)

find_package(sdl2-ttf CONFIG REQUIRED)
target_link_libraries(quarantania2 PRIVATE SDL2::SDL2_ttf)

find_package(RapidJSON CONFIG REQUIRED)
target_include_directories(quarantania2 PRIVATE ${RAPIDJSON_INCLUDE_DIRS})

#find_package(tinyxml2 CONFIG REQUIRED)
#target_link_libraries(quarantania2 PRIVATE tinyxml2::tinyxml2)