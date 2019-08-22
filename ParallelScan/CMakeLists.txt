# ---------------------------------------------------------------------------
# tests
# ---------------------------------------------------------------------------
add_executable(parallel_scan_serial Serial/parallel_scan.cpp)

foreach(dev_ ${benchmark_kokkos_devices})
  string(TOLOWER ${dev_} devlc_)
  set(targetname parallel_scan_kokkos_${devlc_})
  add_executable(${targetname} Kokkos/parallel_scan.cpp)
  target_link_libraries(${targetname}      PUBLIC kokkos-${dev_})
endforeach()
