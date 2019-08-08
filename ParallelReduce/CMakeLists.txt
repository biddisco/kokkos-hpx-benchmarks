# ---------------------------------------------------------------------------
# tests
# ---------------------------------------------------------------------------
message("KOKKOS_ARCH=${DEFAULT_HOST_ARCH}")

add_executable(parallel_reduce_serial Serial/parallel_reduce.cpp)

add_executable(parallel_reduce_openmp Kokkos/parallel_reduce.cpp)
target_compile_definitions(parallel_reduce_openmp PUBLIC KOKKOS_ARCH=${DEFAULT_HOST_ARCH})
target_include_directories(parallel_reduce_openmp PUBLIC ${kokkos_INCLUDES})
target_link_libraries(parallel_reduce_openmp PUBLIC kokkos-OpenMP)

add_executable(parallel_reduce_hpx Kokkos/parallel_reduce.cpp)
target_compile_definitions(parallel_reduce_hpx PUBLIC KOKKOS_ARCH=${DEFAULT_HOST_ARCH})
target_include_directories(parallel_reduce_hpx PUBLIC ${kokkos_INCLUDES})
target_link_libraries(parallel_reduce_hpx PUBLIC kokkos-HPX)
