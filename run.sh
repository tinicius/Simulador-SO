#!/bin/bash

# Delete all files in data_folder
rm -f data_folder/*

for((k = 0; k < 20; k++)) do

    # Clear the content of data.txt
    > data.txt

    for ((i = 1; i <= 10; i++)) do
    echo "Running with arguments: $ARGS"
    
    # Clear the screen, clean, build, and run with the current arguments
    clear && make clean && make && make run ARGS="3 $i" &
    
    # # Get the PID of the background process
    PID=$!
    wait $PID
    
    done

    touch "data_folder/data$k.txt"

    cp data.txt "data_folder/data$k.txt"

done


