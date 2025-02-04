cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS_DEBUG "-g -o0 DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG -s")

project(excercise3)

include_directories(include)

add_executable(excercise3
    src/main.c
    src/functions.c)

# target_link_libraries(excercise3 m)
