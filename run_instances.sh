#!/bin/bash

PROGRAM="./lop"
INSTANCES_DIR="instances/"

#change parameters here Remember to do this again for some since we changed, then implement speed up 
ALGORITHM="first" 
NEIGHBORHOOD="insertion"  
CONSTRUCTION_HEURISTIC="random"

instance_names=$(awk '{print $1}' best_known/best_known.dat)

# Loop over each instance name
for name in $instance_names; do
    ${PROGRAM} -i "${INSTANCES_DIR}""${name}" "${ALGORITHM}" "${NEIGHBORHOOD}" "${CONSTRUCTION_HEURISTIC}"
done
