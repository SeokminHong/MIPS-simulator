cmake_minimum_required(VERSION 3.10)

ADD_COMPILE_OPTIONS(-std=c++14 -g -Wall)

SET(CMAKE_C_COMPILER "g++")

SET(SRC_FILES src/main.cpp)

ADD_EXECUTABLE(mips-sim ${SRC_FILES})