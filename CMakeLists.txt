# Playground for testing HPX/Kokkos/CUDA/ROCm interoperability
if(KOKKOS_ENABLE_CUDA)
  set(CMAKE_CXX_COMPILER ${CMAKE_CURRENT_SOURCE_DIR}/nvcc_wrapper CACHE STRING "C++ compiler" FORCE)
  set(KOKKOS_ENABLE_CUDA_LAMBDA ON)
  set(KOKKOS_ENABLE_CUDA_RDC ON)
endif()

cmake_minimum_required(VERSION 3.12)
project(hpx-kokkos-benchmarks CXX)

#--------------------------------------------------
# setup Output directories
#--------------------------------------------------
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all Executables."
)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single Directory for all Libraries"
)
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single Directory for all static libraries."
)

# ---------------------------------------------------------
# Module path setup
# ---------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#------------------------------------------------------------------------------
# Detect which machine we are running on to setup defaults for convenience
#------------------------------------------------------------------------------
SITE_NAME(hostname)
if(hostname MATCHES "unknown" OR hostname MATCHES "No")
  message("unknown hostname \"${hostname}\", using ENV$ $ENV{HOSTNAME} instead")
  set(hostname $ENV{HOSTNAME})
endif()

if(hostname MATCHES jb-s76)
  set(DEFAULT_HOST_ARCH   HSW)
  set(DEFAULT_DEVICE_ARCH Pascal60)
elseif(hostname MATCHES daint)
  set(DEFAULT_HOST_ARCH   BDW)
  set(DEFAULT_DEVICE_ARCH Pascal60)
  # please set other machine defaults
endif()

message("------------------------------------")
message("CMAKE_SYSTEM is ${CMAKE_SYSTEM} with hostname ${hostname} and processor ${CMAKE_SYSTEM_PROCESSOR}" )
message("HOST ARCH ${DEFAULT_HOST_ARCH}, DEVICE ARCH ${DEFAULT_DEVICE_ARCH}")
message("------------------------------------")

# ---------------------------------------------------------
# Find HPX
# ---------------------------------------------------------
find_package(HPX REQUIRED)

#--------------------------------------------------
# Check we are using the same build type as HPX
#--------------------------------------------------
if (NOT "${HPX_BUILD_TYPE}" STREQUAL "${CMAKE_BUILD_TYPE}")
  message(WARNING "CMAKE_BUILD_TYPE does not match ${HPX_BUILD_TYPE} :\n"
    "this project uses ${CMAKE_BUILD_TYPE}, hpx uses ${HPX_BUILD_TYPE}\n"
    "Please add -DCMAKE_BUILD_TYPE=${HPX_BUILD_TYPE} to your cmake command"
    "or use another HPX build dir"
  )
endif()

# ---------------------------------------------------------
# Add Kokkos (core/kernels) as subdirectories
# ---------------------------------------------------------
# Allow a local copy of kokkos src to be used instead of
# cloning it into the build dir
option(USE_LOCAL_KOKKOS_SOURCE_PATH OFF)
if(USE_LOCAL_KOKKOS_SOURCE_PATH)
  find_file(KOKKOS_LOCAL_SOURCE_PATH CMakeLists.txt /home/biddisco/src/kokkos/kokkos)
  set(FETCHCONTENT_SOURCE_DIR_KOKKOS ${KOKKOS_LOCAL_SOURCE_PATH})
endif()

# General options for Kokkos
set(Kokkos_CXX_STANDARD  ${HPX_CXX_STANDARD} CACHE STRING "C++ standard")
set(Kokkos_ENABLE_HWLOC  1)
set(Kokkos_ARCH          ${DEFAULT_HOST_ARCH})
set(Kokkos_ENABLE_AGGRESSIVE_VECTORIZATION 1)

# Backends we want to use
set(Kokkos_ENABLE_SERIAL     1)
set(benchmark_kokkos_devices SERIAL PTHREAD OpenMP HPX)

include(FetchContent)

FetchContent_Declare(
  kokkos
  GIT_REPOSITORY   git@github.com:biddisco/kokkos.git
  GIT_TAG          cmake-hpx-merge
)

FetchContent_Declare(
  kokkoskernels
  GIT_REPOSITORY http://github.com/kokkos/kokkos-kernels
  GIT_TAG        master
)

FetchContent_GetProperties(kokkos)
FetchContent_GetProperties(kokkoskernels)

# After the following call, the CMake targets defined by kokkos
# will be defined and available to the rest of the build
if (NOT kokkos_POPULATED)
  FetchContent_Populate(kokkos)
  foreach(device1_ ${benchmark_kokkos_devices})
    # turn off all devices
    foreach(device2_ ${benchmark_kokkos_devices})
      set(Kokkos_ENABLE_${device2_} 0)
    endforeach()
    # turn on just one
    set(Kokkos_ENABLE_${device1_} 1)
    set(KOKKOS_LIB_SUFFIX "-${device1_}")
    add_subdirectory(
        ${kokkos_SOURCE_DIR}
        ${kokkos_BINARY_DIR}-${device1_}
        EXCLUDE_FROM_ALL)
  endforeach()
endif()

## seems to need some trilinos cmake stuff. @TODO
#if (NOT kokkoskernels_POPULATED)
#  FetchContent_Populate(kokkoskernels)
#    add_subdirectory(
#      ${kokkoskernels_SOURCE_DIR}
#      ${kokkoskernels_BINARY_DIR}
#      EXCLUDE_FROM_ALL)
#endif()

# ---------------------------------------------------------------------------
# Setup lapack, use command line LAPACK_LIBS or intel MKL if available
# ---------------------------------------------------------------------------
if (NOT DEFINED LAPACK_LIBS)
  # Find intel MKL if possible, checks ${MKL_ROOT} and $ENV{MKL_ROOT}
  # enable dump of info during find
  ### Need to be cleaned up
  set(MKL_VERBOSE=1)
  find_package(MKL QUIET)

  if (MKL_FOUND)
    set(LAPACK_INCLUDE_DIRS ${MKL_INCLUDE_DIRS})
    set(LAPACK_LIBRARIES
      mkl_intel_lp64 mkl_core mkl_sequential
      pthread m dl
      CACHE PATH "LAPACK_LIBS"
    )
    link_directories(${MKL_LIB_DIR})
    message("${PROJECT_NAME} MKL libraries are ${MKL_LIBRARIES_MT}")
  elseif (NOT DEFINED LAPACK_LIBS)
    message("${PROJECT_NAME} MKL NOT_FOUND and no LAPACK_LIBS set")
  endif()
else()
  set_property(CACHE "LAPACK_LIBS" PROPERTY TYPE PATH)
  set(LAPACK_LIBRARIES ${LAPACK_LIBS})
  set(MKL_FOUND 1)
endif()

if (NOT DEFINED LAPACK_LIBRARIES)
  message(FATAL_ERROR "set MKL_ROOT or LAPACK_LIBS with the BLAS/LAPACK link line.")
endif()

# ---------------------------------------------------------------------------
# Default includes for Kokkos
# ---------------------------------------------------------------------------
set(kokkos_INCLUDES
  ${kokkos_SOURCE_DIR}/core/src
  ${kokkos_SOURCE_DIR}/algorithms/src
)

# ---------------------------------------------------------------------------
# Benchmarks
# ---------------------------------------------------------------------------
add_subdirectory(ParallelFor)
add_subdirectory(ParallelReduce)
add_subdirectory(ParallelScan)

# ---------------------------------------------------------------------------
# Scripts to run stuff
# ---------------------------------------------------------------------------
add_subdirectory(scripts)

