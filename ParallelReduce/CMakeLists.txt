# ---------------------------------------------------------------------------
# tests
# ---------------------------------------------------------------------------
add_executable(parallel_reduce_serial Serial/parallel_reduce.cpp)

foreach(dev_ ${benchmark_kokkos_devices})
  string(TOLOWER ${dev_} devlc_)
  set(targetname parallel_reduce_kokkos_${devlc_})
  add_executable(${targetname} Kokkos/parallel_reduce.cpp)
  target_link_libraries(${targetname}      PUBLIC kokkos-${dev_})
endforeach()
