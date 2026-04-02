#!/usr/bin/env bash

# Input file and output file for XMGrace
input_file="$1"

# Check if the input file exists
if [[ ! -f "$input_file" ]]; then
    echo "Input file not found!"
    exit 1
fi

# Create output file name by removing the extension from input_file and adding .plotdat
output_file="${input_file%.*}.plotdat"

# Write in output file
echo "@    title \"Dose response functions\"" > "$output_file"

# Add in the output file
echo "@    xaxis  label \"Position (cm)\"" >> "$output_file"
echo "@    yaxis  label \"Absorbed dose (Gy/hist)\"" >> "$output_file"
echo "@    s0 legend  \"Water\"" >> "$output_file"
echo "@    s1 legend  \"Chamber\"" >> "$output_file"
echo "@target G0.S0" >> "$output_file"
echo "@type xydy" >> "$output_file"

# Extract and calculate the modified dose values with an incrementing first column
awk '/Geometry[[:space:]]+Cavity dose/,/^$/' "$input_file" | \
    grep -E 'water_in_water' | \
    awk -v counter=-2 -v step=0.1 '{uncert = $2 * ($4 / 100); print counter, $2, uncert; counter += step}' >> "$output_file"

echo "&" >> "$output_file"
echo "@target G0.S1" >> "$output_file"
echo "@type xydy" >> "$output_file"

# Extract and calculate the modified dose values with an incrementing first column for 'chamber_in_water'
awk '/Geometry[[:space:]]+Cavity dose/,/^$/' "$input_file" | \
    grep -E 'chamber_in_water' | \
    awk -v counter=-2 -v step=0.1 '{uncert = $2 * ($4 / 100); print counter, $2, uncert; counter += step}' >> "$output_file"

echo "&" >> "$output_file"

echo "Data extraction and modification complete. Output saved to $output_file"
