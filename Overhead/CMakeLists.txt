# ---------------------------------------------------------------------------
# tests
# ---------------------------------------------------------------------------
add_executable(overhead_serial task_overhead_serial.cpp)

foreach(dev_ ${benchmark_kokkos_devices})
  string(TOLOWER ${dev_} devlc_)
  set(targetname overhead_kokkos_${devlc_})
  add_executable(${targetname} task_overhead_kokkos.cpp)
  target_link_libraries(${targetname} PUBLIC kokkos-${dev_})
endforeach()
