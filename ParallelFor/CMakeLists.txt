# ---------------------------------------------------------------------------
# tests
# ---------------------------------------------------------------------------
add_executable(parallel_for_serial Serial/parallel_for.cpp)

foreach(dev_ ${benchmark_kokkos_devices})
  string(TOLOWER ${dev_} devlc_)
  set(targetname parallel_for_kokkos_${devlc_})
  add_executable(${targetname} Kokkos/parallel_for.cpp)
  target_link_libraries(${targetname}      PUBLIC kokkos-${dev_})
endforeach()
