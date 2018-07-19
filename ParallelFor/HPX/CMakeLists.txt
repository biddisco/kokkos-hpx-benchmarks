cmake_minimum_required(VERSION 3.3.2)
project(arrays_of_dot_products CXX)

find_package(HPX REQUIRED)

add_hpx_executable(parallel_for
  SOURCES parallel_for.cpp
  COMPONENT_DEPENDENCIES iostreams)
