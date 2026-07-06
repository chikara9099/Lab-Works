#!/bin/bash
set -e
STUDENT_ID="2205062"
MAIN_FILE="${STUDENT_ID}_main.cpp"
OUTPUT_EXE="${STUDENT_ID}"
if [ -f "$MAIN_FILE" ]; then
    echo "Compiling $MAIN_FILE with Leak and Address Sanitizers..."
    g++ -std=c++17 -Wall -Wextra -g -fsanitize=address,undefined "$MAIN_FILE" -o "$OUTPUT_EXE"
    echo "Compilation successful! Executable created: ./$OUTPUT_EXE"
    echo "Run using ./$OUTPUT_EXE <input_file> <output_file>"
else
    echo "❌ Error: Source file $MAIN_FILE not found in the current directory."
    exit 1
fi