#!/bin/bash

while IFS= read -r line; do
    echo "Trying: $line"

    # Run bomb with current line, capture output and errors
    echo "$line" | ./bomb responses.txt > temp_output.txt 2>&1

    # Check if it did NOT explode (you can tune this condition)
    if ! grep -q "BOOM" temp_output.txt; then
        echo "✅ Success with: $line"
        cat temp_output.txt
        break
    fi
done < numbers.txt