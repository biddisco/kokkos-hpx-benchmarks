#!/usr/bin/env bash

input_1_sizes=(100000 1000000 10000000 100000000 1000000000)
num_threads_values=(1 2 4 8 12 16 18 20 24 28 32 36)
total_size=1000000000 # Make sure nrepeat * input_1_size = total_size

output_path=$HOME
output_file="$output_path"/$(date +%s)-$(git rev-parse HEAD).log

echo "hostname,timestamp,num_threads,benchmark,runtime,input_size_1,input_size_2,num_repeats,time,result,specific_metric,metric_name" > "$output_file"

for input_1_size in "${input_1_sizes[@]}"; do
    echo "input_1_size = $input_1_size"
    arguments="-l $input_1_size -nrepeat $((total_size / input_1_size))"
    echo $arguments

    pushd Serial
    ./parallel_for.host $arguments >> "$output_file"
    popd

    for num_threads in "${num_threads_values[@]}"; do
        echo "num_threads = $num_threads"

        pushd Kokkos
        OMP_NUM_THREADS="$num_threads" OMP_PROC_BIND=close OMP_PLACES=threads ./parallel_for.host.openmp $arguments >> "$output_file"
        ./parallel_for.host.hpx --hpx:threads="$num_threads" $arguments >> "$output_file"
        popd

        pushd HPX/build/Release
        ./parallel_for --hpx:threads="$num_threads" $arguments >> "$output_file"
        popd
    done
done
