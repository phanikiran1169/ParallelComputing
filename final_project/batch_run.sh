#!/bin/bash

# Runs all combinations of MPI processes and OpenMP threads to test 
# the performance of the Floyd-Warshall algorithm

echo "=========================================="
PROCESSES=(1 2 4 8 12)
THREADS=(1)

# Create logs directory
mkdir -p logs

# Run all combinations
for p in "${PROCESSES[@]}"; do
    for t in "${THREADS[@]}"; do
        echo "Running test: ${p} processes, ${t} threads"
        
        OUTPUT_FILE="logs/output_p${p}_t${t}.txt"
        LOG_FILE="logs/screen_log_p${p}_t${t}.txt"
        
        echo "  Command: make run NUM_PROCESSES=${p} NUM_THREADS=${t} OUTPUT_FILE=${OUTPUT_FILE}"
        echo "  Output: ${OUTPUT_FILE}"
        echo "  Log: ${LOG_FILE}"
        
        make run NUM_PROCESSES=${p} NUM_THREADS=${t} OUTPUT_FILE=${OUTPUT_FILE} > "${LOG_FILE}" 2>&1
        exit_code=$?
        
        if [ $exit_code -eq 0 ]; then
            echo "Completed successfully"
        else
            echo "Failed with exit code ${exit_code}"
        fi
        echo ""
    done
done

echo "=========================================="