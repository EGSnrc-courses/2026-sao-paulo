#!/usr/bin/env bash

# Input file
input_file="$1"

# Check if the input file exists
if [[ ! -f "$input_file" ]]; then
    echo "Input file not found!"
    exit 1
fi

# Define the output file with .pertb extension
output_file="${input_file%.*}.pertb"

# Initialize arrays to store values
s0_col1_values=()
s1_col1_values=()
s0_col2_values=()
s1_col2_values=()
s0_col3_values=()
s1_col3_values=()

# Read through the file and collect values
target=""
while IFS= read -r line; do
    if [[ "$line" =~ ^@target[[:space:]]+G0\.S0 ]]; then
        target="S0"
    elif [[ "$line" =~ ^@target[[:space:]]+G0\.S1 ]]; then
        target="S1"
    fi

    # Process data lines
    if [[ "$line" =~ ^-?[0-9.eE+-]+[[:space:]]+-?[0-9.eE+-]+[[:space:]]+-?[0-9.eE+-]+ ]]; then
        read -r col1 col2 col3 <<< "$line"

        if [[ "$target" == "S0" ]]; then
            s0_col1_values+=("$col1")
            s0_col2_values+=("$col2")
            s0_col3_values+=("$col3")
        elif [[ "$target" == "S1" ]]; then
            s1_col1_values+=("$col1")
            s1_col2_values+=("$col2")
            s1_col3_values+=("$col3")
        fi
    fi
done < "$input_file"

# Check if data lengths match
if [[ ${#s0_col1_values[@]} -ne ${#s1_col1_values[@]} ]]; then
    echo "Mismatch in data points between S0 and S1"
    exit 1
fi

# Output results in XMGrace format
{
    echo "@    title \"Perturbation function\""
    echo "@    xaxis  label \"Position (cm)\""
    echo "@    yaxis  label \"Perturbation value (a.u.)\""
    echo "@target G0.S0"
    echo "@type xydy"

    # Loop through values and calculate perturbations
    for i in "${!s0_col1_values[@]}"; do
        avg_col1=$(awk "BEGIN { print (${s0_col1_values[$i]} + ${s1_col1_values[$i]}) / 2 }")
        perturb_col2=$(awk "BEGIN { print (${s0_col2_values[$i]} / 0.555) - (${s1_col2_values[$i]} / 0.5) }")
        quad_sum_col3=$(awk "BEGIN { print sqrt((${s0_col3_values[$i]} ^ 2) + (${s1_col3_values[$i]} ^ 2)) }")

        # Output each line in scientific notation
        printf "%.5e %.5e %.5e\n" "$avg_col1" "$perturb_col2" "$quad_sum_col3"
    done

    echo "&"
} > "$output_file"

echo "Data processed and output to $output_file in XMGrace format."
