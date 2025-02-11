cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS_DEBUG "-g -o0 DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG -s")

project(word_explorer)

include_directories(include)

add_executable(word_explorer
    src/main.c
    src/functions.c)

# target_link_libraries(excercise2 m)
