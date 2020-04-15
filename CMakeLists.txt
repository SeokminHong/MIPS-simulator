cmake_minimum_required(VERSION 3.10)

project(MIPS_Simulator)

add_compile_options(-std=c++14 -O2 -g -Wall)

set(CMAKE_C_COMPILER "g++")

set(SRC_FILES 
    src/main.cpp
    
    src/machine/machine.cpp
    src/machine/instruction.cpp
    )

add_executable(MIPS_Simulator ${SRC_FILES})