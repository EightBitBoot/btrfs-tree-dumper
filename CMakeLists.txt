cmake_minimum_required(VERSION 3.0.0)

project(btrfs-tree-dump VERSION 0.0.0.1 LANGUAGES C)

file(GLOB_RECURSE BTD_SOURCES "src/*.c")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")

add_executable(btrfs-tree-dump ${BTD_SOURCES})

target_include_directories(btrfs-tree-dump PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)