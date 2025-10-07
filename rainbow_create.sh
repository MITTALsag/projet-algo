#!/bin/bash

# rainbow_create.sh - Script to create rainbow tables with flexible parameters
# Usage: ./rainbow_create.sh <num_tables> [pass_file] [output_dir]

# Check if at least one argument is provided
if [ $# -lt 1 ]; then
    echo "Usage: $0 <num_tables> [pass_file] [output_dir]"
    echo "  num_tables: Number of rainbow tables to create (e.g., 10, 15, 20)"
    echo "  pass_file:  Optional file containing initial passwords (one per line)"
    echo "  output_dir: Optional directory to store output files (default: ./files)"
    echo ""
    echo "Examples:"
    echo "  $0 10                    # Create 10 tables with random passwords in ./files"
    echo "  $0 15 passwords.txt      # Create 15 tables using passwords from file"
    echo "  $0 20 passwords.txt data # Create 20 tables in ./data directory"
    exit 1
fi

# Parse arguments
NUM_TABLES=$1
PASS_FILE=""
OUTPUT_DIR="files"  # Default directory

# Check if password file is provided
if [ $# -ge 2 ]; then
    PASS_FILE=$2
    if [ ! -f "$PASS_FILE" ]; then
        echo "Warning: Password file '$PASS_FILE' not found. Will use random passwords."
        PASS_FILE=""
    fi
fi

# Check if output directory is provided
if [ $# -ge 3 ]; then
    OUTPUT_DIR=$3
fi

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Check if R in hash.h matches the requested number
R_VALUE=$(grep "^#define R " hash.h | awk '{print $3}')
if [ "$R_VALUE" != "$NUM_TABLES" ]; then
    echo "Warning: hash.h defines R=$R_VALUE but you requested $NUM_TABLES tables"
    echo "You may need to update hash.h with: #define R $NUM_TABLES"
    echo "Continue anyway? (y/n)"
    read -r response
    if [ "$response" != "y" ]; then
        exit 1
    fi
fi

# Build the file list for rainbow_create
FILE_LIST=""
for i in $(seq 1 $NUM_TABLES); do
    FILE_LIST="$FILE_LIST ${OUTPUT_DIR}/table${i}.txt"
done

# Add password file if provided
if [ -n "$PASS_FILE" ]; then
    FILE_LIST="$FILE_LIST $PASS_FILE"
fi

# Display what will be executed
echo "Creating $NUM_TABLES rainbow tables in directory: $OUTPUT_DIR"
if [ -n "$PASS_FILE" ]; then
    echo "Using passwords from: $PASS_FILE"
else
    echo "Using randomly generated passwords"
fi
echo ""
echo "Executing: ./rainbow_create $FILE_LIST"
echo ""

# Execute rainbow_create with timing
time ./rainbow_create $FILE_LIST

# Check if creation was successful
if [ $? -eq 0 ]; then
    echo ""
    echo "Rainbow tables created successfully!"
    echo "Files created in: $OUTPUT_DIR/"
    
    # Show statistics
    echo ""
    echo "Table statistics:"
    for i in $(seq 1 $NUM_TABLES); do
        FILE="${OUTPUT_DIR}/table${i}.txt"
        if [ -f "$FILE" ]; then
            LINES=$(wc -l < "$FILE")
            UNIQUE=$(awk '{print $2}' "$FILE" | sort | uniq | wc -l)
            echo "  table${i}.txt: $LINES chains, $UNIQUE unique endpoints"
        fi
    done
else
    echo "Error: Rainbow table creation failed!"
    exit 1
fi