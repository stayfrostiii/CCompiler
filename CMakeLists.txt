cmake_minimum_required(VERSION 3.5.0)
project(ccompiler VERSION 0.1.0 LANGUAGES C CXX)

add_executable(ccompiler 
    src/main.cpp
    src/class/stmt.cpp
)

