cmake_minimum_required(VERSION 3.23 FATAL_ERROR)

project(Julia
    LANGUAGES CXX
)

add_executable(julia
    src/main.cpp
)