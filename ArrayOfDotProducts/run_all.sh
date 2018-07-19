#!/usr/bin/env bash

input_1_sizes=(10000 100000)
input_2_sizes=(10000)
num_threads_values=(1 2 4 9 18 36)

output_path=$(pwd)
output_file="$output_path"/$(date +%s)-$(git rev-parse HEAD).log

echo "hostname, timestamp, num_threads, benchmark, runtime, input_size_1, input_size_2, num_repeats, time, result, specific_metric, metric_name" > "$output_file"

for input_1_size in "${input_1_sizes[@]}"; do
    for input_2_size in "${input_2_sizes[@]}"; do
        echo "input_1_size = $input_1_size, input_2_size = $input_2_size"
        arguments="-num_vectors $input_1_size -l $input_2_size"
        echo $arguments

        pushd Serial
        ./array_of_dot_products.host $arguments >> "$output_file"
        popd

        for num_threads in "${num_threads_values[@]}"; do
            echo "num_threads = $num_threads"

            pushd OpenMP
            OMP_NUM_THREADS="$num_threads" OMP_PROC_BIND=spread OMP_PLACES=threads ./array_of_dot_products.host $arguments >> "$output_file"
            popd

            pushd Kokkos-Minimal
            ./array_of_dot_products.host.pthreads --kokkos-threads="$num_threads" $arguments >> "$output_file"
            OMP_NUM_THREADS="$num_threads" OMP_PROC_BIND=spread OMP_PLACES=threads ./array_of_dot_products.host.openmp $arguments >> "$output_file"
            popd

            pushd Kokkos-Flat
            ./array_of_dot_products.host.pthreads --kokkos-threads="$num_threads" $arguments >> "$output_file"
            OMP_NUM_THREADS="$num_threads" OMP_PROC_BIND=spread OMP_PLACES=threads ./array_of_dot_products.host.openmp $arguments >> "$output_file"
            popd

            pushd Kokkos-Hierarchy
            ./array_of_dot_products.host.pthreads --kokkos-threads="$num_threads" $arguments >> "$output_file"
            OMP_NUM_THREADS="$num_threads" OMP_PROC_BIND=spread OMP_PLACES=threads ./array_of_dot_products.host.openmp $arguments >> "$output_file"
            popd

            mkdir -p HPX/build/Release
            pushd HPX/build/Release
            ./array_of_dot_products --hpx:threads="$num_threads" $arguments >> "$output_file"
            popd
        done
    done
done
