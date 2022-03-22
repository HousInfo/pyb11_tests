cmake_minimum_required(VERSION 2.8.12)
project(pyb11demo)
file(
  GLOB_RECURSE TEST_SOURCES
  *.h *.cpp *.py)

find_package(PythonInterp REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

add_subdirectory(pybind11)
#add_subdirectory(pydemo)
pybind11_add_module(pyb11demo demo.cpp)