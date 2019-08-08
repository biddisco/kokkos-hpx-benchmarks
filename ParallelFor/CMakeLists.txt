# ---------------------------------------------------------------------------
# tests
# ---------------------------------------------------------------------------
add_executable(parallel_for_serial Serial/parallel_for.cpp)

add_executable(parallel_for_openmp Kokkos/parallel_for.cpp)
target_compile_definitions(parallel_for_openmp PUBLIC KOKKOS_ARCH=${DEFAULT_HOST_ARCH})
target_include_directories(parallel_for_openmp PUBLIC ${kokkos_INCLUDES})
target_link_libraries(parallel_for_openmp PUBLIC kokkos-OpenMP)

add_executable(parallel_for_hpx Kokkos/parallel_for.cpp)
target_compile_definitions(parallel_for_hpx PUBLIC KOKKOS_ARCH=${DEFAULT_HOST_ARCH})
target_include_directories(parallel_for_hpx PUBLIC ${kokkos_INCLUDES})
target_link_libraries(parallel_for_hpx PUBLIC kokkos-HPX)
