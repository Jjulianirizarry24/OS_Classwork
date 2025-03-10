cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS_DEBUG "-g -o0 DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG -s")

project(processA)

include_directories(processA)

add_executable(processA
    src/main.c
    )

