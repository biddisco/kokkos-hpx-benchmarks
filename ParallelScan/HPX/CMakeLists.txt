cmake_minimum_required(VERSION 3.3.2)
project(parallel_scan CXX)

find_package(HPX REQUIRED)

add_hpx_executable(parallel_scan
  SOURCES parallel_scan.cpp
  COMPONENT_DEPENDENCIES iostreams)
