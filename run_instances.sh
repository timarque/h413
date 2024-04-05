#!/bin/bash

PROGRAM="./lop"
INSTANCES_DIR="instances/"

instance_names=$(awk '{print $1}' best_known/best_known.dat) # get same order as bestknown

# Function to limit the number of parallel processes
function limit_parallel_processes {
    while [[ $(jobs -p | wc -l) -ge $1 ]]; do
        sleep 0.1
    done
}

# Loop over each instance name
for name in $instance_names; do
    # Parallelize the execution of commands
    limit_parallel_processes 2 # Limit to 2 parallel processes
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "transpose" "random" &
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "transpose" "cw" &
    ${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "exchange" "random" &
    ${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "exchange" "cw" &
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "insertion" "random" &
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "insertion" "cw" &

    limit_parallel_processes 2
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "transpose" "random" &
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "transpose" "cw" &
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "exchange" "random" &
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "exchange" "cw" &
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "insertion" "random" &
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "insertion" "cw" &

    limit_parallel_processes 2
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "vnd" "tie" "cw" &
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "vnd" "tei" "cw" &
done

# Wait for all background processes to finish
wait
