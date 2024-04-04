#!/bin/bash

PROGRAM="./lop"
INSTANCES_DIR="instances/"

instance_names=$(awk '{print $1}' best_known/best_known.dat) # get same order as bestknown

# Loop over each instance name
for name in $instance_names; do
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "transpose" "random"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "transpose" "cw"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "exchange" "random"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "exchange" "cw"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "insertion" "random"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "best" "insertion" "cw"
    

    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "transpose" "random"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "transpose" "cw"
    ${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "exchange" "random"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "exchange" "cw"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "insertion" "random"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "first" "insertion" "cw"


    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "vnd" "tie" "random"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "vnd" "tie" "cw"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "vnd" "tei" "random"
    #${PROGRAM} -i "${INSTANCES_DIR}""${name}" "vnd" "tei" "cw"
done
