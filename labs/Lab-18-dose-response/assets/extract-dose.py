#!/usr/bin/env python3
"""
This script extracts dose data from an .egslog file and writes the data to an
output file in xmgrace format.

Usage:
    ./extract_dose.py egslog_file

Arguments:
    egslog_file: The path to the input .egslog file containing dose data.

The script processes the input file, extracts dose data for different materials,
and generates an output file that can be used for plotting with xmgrace.
"""

import sys
import re
import os
import getopt

# Function to read and parse the input file
def read_input_file(file_path):
    try:
        with open(file_path, 'r') as file:
            return file.readlines()
    except FileNotFoundError:
        sys.exit("Input file not found!")

# Function to extract dose data using regex
def extract_dose(lines, material_pattern):
    data = []
    in_section = False
    for line in lines:
        if re.search(r'Geometry\s+Cavity dose', line):
            in_section = True
            continue
        if in_section and line.strip() == '':
            break
        if in_section and re.search(material_pattern, line):
            parts = line.split()
            if len(parts) >= 5:
                dose = float(parts[1])
                uncertainty = dose * (float(parts[3]) / 100)
                data.append((dose, uncertainty))
    return data

# Function to write the extracted data to an output file
def write_output_file(output_file, water_data, chamber_data):
    with open(output_file, 'w') as file:
        # Write metadata for XMGrace
        file.write('''@title "Dose response functions"\n''')
        file.write('''@xaxis label "Position  (cm)"\n''')
        file.write('''@yaxis label "Absorbed dose  (Gy / history)"\n''')
        file.write('''@s0 legend "Water"\n''')
        file.write('''@s1 legend "Chamber"\n''')

        # Write data for water and chamber
        data_sets = [
            ("G0.S0", water_data),
            ("G0.S1", chamber_data)
        ]

        for target, data in data_sets:
            file.write(f"@target {target}\n")
            file.write("@type xydy\n")
            for i, (dose, uncertainty) in enumerate(data):
                position = -2.0 + i * 0.1
                file.write(f"{position:6.1f} {dose:14.6e} {uncertainty:14.6e}\n")
            file.write("&\n")

# Function to parse command-line arguments
def parse_arguments():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h", ["help"])
    except getopt.GetoptError as err:
        print(err)
        print("Usage: ./extract_dose.py egslog_file")
        sys.exit(2)

    for opt, argument in opts:
        if opt in ("-h", "--help"):
            print("Usage: ./extract_dose.py egslog_file")
            sys.exit(0)
    if not args:
        print("Error: Missing egslog file name")
        print("Usage: ./extract_dose.py egslog_file")
        sys.exit(2)

    return args[0]

# Main function
def main():
    input_file = parse_arguments()
    output_file = os.path.splitext(input_file)[0] + '-dose.dat'

    lines = read_input_file(input_file)
    water_data = extract_dose(lines, "water_in_water")
    chamber_data = extract_dose(lines, "chamber_in_water")

    write_output_file(output_file, water_data, chamber_data)
    print(f"Processing complete. Output file created: {output_file}")

if __name__ == "__main__":
    main()
