# Benchmarks to compare Kokkos OpenMP, Kokkos HPX, and native HPX performance

Each directory contains a `build_all.sh` script which will build the different
benchmark variants. The script assumes that you either have `HPX_ROOT` and
`KOKKOS_PATH` set in environment variables or in the `build_settings.sh` script
(the build will give a hard error if these are not set).

The `run_all.sh` script sweeps through different input sizes and thread numbers
for all the variants. The script will by default write the results to a file in
your home directory.
