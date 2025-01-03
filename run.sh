#!/bin/bash

# Delete all files in data_folder
rm -f data_folder/*
mkdir -p data_folder

make clean && make

MAX_CORES=8
NUMBER_OF_PROGRAMS=1
NUMBER_OF_RUNS_EACH_CORE=100

for ((i = 1; i <= $MAX_CORES; i++)); do
    rm -f -r "out/$i"
    mkdir -p "out/$i"

    for ((p = 0; p <= $NUMBER_OF_RUNS_EACH_CORE; p++)); do
        echo "Iteration: Core=$i, Run=$p"

        # Use a subshell and timeout to avoid hanging
        if ! timeout 10s bash -c "make run ARGS='$NUMBER_OF_PROGRAMS $i'" >> "out/$i/output.log" 2>> "out/$i/errors.log"; then
            echo "Error occurred or timeout reached in iteration $p (Core=$i). Skipping..."
            continue
        fi
    done

    # Process logs
    touch "out/$i/process.log"
    cp "out/process.log" "out/$i/process.log"

    # Clear the content
    > "out/process.log"
done
