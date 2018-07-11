This directory contains different versions of an arrays of dot products
benchmark.

To build the non-HPX benchmarks run (for example) `make -j KOKKOS_ARCH=HSW
KOKKOS_DEVICES=Cuda,OpenMP KOKKOS_PATH=/path/to/kokkos`. For the HPX benchmark
build it using CMake.
