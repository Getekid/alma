#!/bin/bash

if [ "$1" == "" ] || [ "$2" == "" ];
then
    echo "Please provide the script to run and the folder with the input/output scripts"
    exit 2
fi

for i in $(seq 1 20);
do
    input="$2/input$i.txt"
    expected=$(cat $2/output$i.txt)
    echo "Running script with input $input, expected output is $expected"
    output=$(/usr/bin/time -f "%e %M" ./$1 < $input)
    if [ "$output" == "$expected" ];
    then
        echo "SUCCESS: Input $i returns the expected output!"
    else
        echo "ERROR: Input $i does NOT return the expected output! Output: $output"
    fi
done
