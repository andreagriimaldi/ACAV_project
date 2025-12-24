cmake_minimum_required(VERSION 4.0)
project(ACAV)

set(CMAKE_CXX_STANDARD 23)

add_executable(ACAV main.cpp
        Point.cpp
        Point.h
        Point_type.h
        Map.cpp
        Map.h)
