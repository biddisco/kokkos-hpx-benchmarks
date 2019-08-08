# ---------------------------------------------------------------------------
# tests
# ---------------------------------------------------------------------------
message("KOKKOS_ARCH=${DEFAULT_HOST_ARCH}")

add_executable(parallel_scan_serial Serial/parallel_scan.cpp)

add_executable(parallel_scan_openmp Kokkos/parallel_scan.cpp)
target_compile_definitions(parallel_scan_openmp PUBLIC KOKKOS_ARCH=${DEFAULT_HOST_ARCH})
target_include_directories(parallel_scan_openmp PUBLIC ${kokkos_INCLUDES})
target_link_libraries(parallel_scan_openmp PUBLIC kokkos-OpenMP)

add_executable(parallel_scan_hpx Kokkos/parallel_scan.cpp)
target_compile_definitions(parallel_scan_hpx PUBLIC KOKKOS_ARCH=${DEFAULT_HOST_ARCH})
target_include_directories(parallel_scan_hpx PUBLIC ${kokkos_INCLUDES})
target_link_libraries(parallel_scan_hpx PUBLIC kokkos-HPX)
