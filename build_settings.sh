set -u

#HPX_ROOT=path/to/hpx
#KOKKOS_PATH=path/to/kokkos
HPX_DIR="$HPX_ROOT/lib64/cmake/HPX"
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:"$HPX_ROOT/lib64/pkg-config"
HPX_CXX_FLAGS="-march=core-avx2 -mtune=core-avx2 -mrtm"
KOKKOS_ARCH="BDW"
KOKKOS_OPTIONS="aggressive_vectorization,disable_profiling,disable_deprecated_code"
