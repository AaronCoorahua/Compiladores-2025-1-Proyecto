cmake_minimum_required(VERSION 3.29)
project(Proyecto)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto main.cpp
        environment.h
        exp.cpp
        token.cpp
        scanner.cpp
        parser.cpp
        visitor.cpp)
