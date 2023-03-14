#!/bin/bash

# Define the ranges of n and n_t to test
echo "Code is running"
n_values=( 124 2048 4096 10240)
n_t_values=(1 2 4 8)

# Run the program for each combination of n and n_t, and record the time taken
declare -A results
for n in ${n_values[@]}; do
    for n_t in ${n_t_values[@]}; do
        output=$(./p $n $n_t 2>&1)
        # echo "output is $output"
        if [ $? -ne 0 ]; then
            echo "Error executing command ['./p', $n, $n_t]: $output" >&2
        else
            time_taken=$(echo "$output" | grep "Time taken" | awk '{print $4}')
            results["$n,$n_t"]=$time_taken
        fi
    done
done

# Plot the results on a graph
gnuplot -p -e "
set xlabel 'n';
set ylabel 'Time taken (s)';
set logscale x;
set key left top;
plot $(for n_t in ${n_t_values[@]}; do echo "'-' with lines title '$n_t threads', "; done);
$(for n_t in ${n_t_values[@]}; do echo "${n_values[@]}" | tr ' ' '\n' | awk '{print $1","'$n_t'"}"}' | gnuplot -p -e "plot '-' using 1:2 with lines title '$n_t threads'; $(for n in ${n_values[@]}; do echo "$n ${results["$n,$n_t"]]}"; done); e"; done)"
