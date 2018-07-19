#!/usr/bin/env bash

KOKKOS_PATH=""
HPX_DIR=""
HPX_CXX_FLAGS=""
KOKKOS_ARCH=""
KOKKOS_OPTIONS=""

pushd Serial
make KOKKOS_PATH="$KOKKOS_PATH" KOKKOS_ARCH="$KOKKOS_ARCH" KOKKOS_OPTIONS="$KOKKOS_OPTIONS"
popd

pushd Kokkos
make KOKKOS_DEVICES=Pthreads KOKKOS_ARCH="$KOKKOS_ARCH" KOKKOS_PATH="$KOKKOS_PATH" KOKKOS_USE_TPLS="hwloc" KOKKOS_OPTIONS="$KOKKOS_OPTIONS"
cp parallel_for.host parallel_for.host.pthreads
make KOKKOS_DEVICES=OpenMP KOKKOS_ARCH="$KOKKOS_ARCH" KOKKOS_PATH="$KOKKOS_PATH" KOKKOS_OPTIONS="$KOKKOS_OPTIONS"
cp parallel_for.host parallel_for.host.openmp
popd

mkdir -p HPX/build/Release
pushd HPX/build/Release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$HPX_CXX_FLAGS" -DHPX_DIR="$HPX_DIR" ../..
make all
popd
