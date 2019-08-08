#!/usr/bin/env bash

source "../build_settings.sh"

pushd Serial
make -j20 KOKKOS_PATH="$KOKKOS_PATH" KOKKOS_ARCH="$KOKKOS_ARCH" KOKKOS_OPTIONS="$KOKKOS_OPTIONS"
popd

pushd Kokkos
make -j20 KOKKOS_DEVICES=OpenMP KOKKOS_ARCH="$KOKKOS_ARCH" KOKKOS_PATH="$KOKKOS_PATH" KOKKOS_OPTIONS="$KOKKOS_OPTIONS"
cp parallel_for.host parallel_for.host.openmp
make -j20 KOKKOS_DEVICES=HPX KOKKOS_ARCH="$KOKKOS_ARCH" KOKKOS_PATH="$KOKKOS_PATH" KOKKOS_OPTIONS="$KOKKOS_OPTIONS"
cp parallel_for.host parallel_for.host.hpx
popd

mkdir -p HPX/build/Release
pushd HPX/build/Release
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="$HPX_CXX_FLAGS" -DHPX_DIR="$HPX_DIR" ../..
make -j20 all
popd
