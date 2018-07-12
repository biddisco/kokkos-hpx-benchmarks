#!/usr/bin/env bash

KOKKOS_PATH=""
HPX_DIR=""
KOKKOS_ARCH=""

input_1_sizes=()
input_2_sizes=()
num_threads_values=()

output_path=$(pwd)
output_file="$output_path"/$(date +%s)-$(git rev-parse HEAD)

echo "hostname, timestamp, num_threads, benchmark, runtime, input_size_1, input_size_2, num_repeats, time, result" > "$output_file"

for input_1_size in "${input_1_sizes[@]}"; do
    for input_2_size in "${input_2_sizes[@]}"; do
        echo "input_1_size = $input_1_size, input_2_size = $input_2_size"
        arguments="-num_vectors $input_1_size -l $input_2_size"
        echo $arguments

        pushd Serial
        make KOKKOS_PATH="$KOKKOS_PATH"
        ./array_of_dot_products.host $arguments >> "$output_file"
        popd

        for num_threads in "${num_threads_values[@]}"; do
            echo "num_threads = $num_threads"
            pushd OpenMP
            make KOKKOS_PATH="$KOKKOS_PATH"
            OMP_NUM_THREADS="$num_threads" OMP_PROC_BIND=spread OMP_PLACES=threads ./array_of_dot_products.host $arguments >> "$output_file"
            popd

            pushd Kokkos-Flat
            make KOKKOS_DEVICES=Pthreads KOKKOS_ARCH="$KOKKOS_ARCH" KOKKOS_PATH="$KOKKOS_PATH"
            ./array_of_dot_products.host --kokkos-threads="$num_threads" $arguments >> "$output_file"
            popd

            mkdir -p HPX/build/Release
            pushd HPX/build/Release
            cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-mavx -march=native -mtune=native -ffast-math" -DHPX_DIR="$HPX_DIR" ../..
            make all
            ./array_of_dot_products --hpx:threads="$num_threads" $arguments >> "$output_file"
            popd
        done
    done
done
