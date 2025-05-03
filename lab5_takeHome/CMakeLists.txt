cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS_DEBUG "-g -o0 DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG -s")

project(lab5)

include_directories(include)

add_executable(lab5
    src/main.c
    )

target_link_libraries(${CMAKE_PROJECT_NAME}
    m 
    ncurses)