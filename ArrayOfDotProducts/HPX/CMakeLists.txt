cmake_minimum_required(VERSION 3.3.2)
project(array_of_dot_products CXX)

find_package(HPX REQUIRED)

add_hpx_executable(array_of_dot_products
  SOURCES array_of_dot_products.cpp
  COMPONENT_DEPENDENCIES iostreams)
