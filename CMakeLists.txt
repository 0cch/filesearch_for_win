cmake_minimum_required(VERSION 3.14)
project(filesearch LANGUAGES CXX)

add_subdirectory(pybind11)

link_directories(${CMAKE_SOURCE_DIR})
pybind11_add_module(filesearch main.cpp)