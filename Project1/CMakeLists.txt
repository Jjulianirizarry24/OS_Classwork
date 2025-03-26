cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_FLAGS_DEBUG "-g -o0 DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "-o3 -DNDEBUG -s")

project(project1)

include_directories(include)

add_executable(project1 
    src/functions.c
    src/main.c
    )

target_link_libraries(${CMAKE_PROJECT_NAME}
    ncurses)