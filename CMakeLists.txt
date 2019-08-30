cmake_minimum_required(VERSION 3.9)
project(SO2zad1)

set(CMAKE_CXX_STANDARD 14)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(Threads REQUIRED)
include_directories(${THREADS_INCLUDE_DIR})

add_executable(SO2zad1 main.cpp Ship.cpp Ship.h Port.cpp Port.h Worker.cpp Worker.h)
target_link_libraries(SO2zad1 ${CURSES_LIBRARIES})
target_link_libraries(SO2zad1 ${CMAKE_THREAD_LIBS_INIT})