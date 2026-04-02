#!/usr/bin/env python3
"""
This script extracts perturbation values from an input file and writes the data
to an output file in xmgrace format.

Usage:
    ./extract-perturbation.py data_file

Arguments:
    data_file: The path to the input file containing perturbation data.

The script processes the input file, calculates perturbation values, and
generates an output file that can be used for plotting with xmgrace.
"""

import sys
import re
import os
import getopt
import math

# Data point class
class DataPoint:
    def __init__(self, x, y, dy):
        self.x = x
        self.y = y
        self.dy = dy
    def get(self):
        return (self.x, self.y, self.dy)

# Function to read and parse the input file
def read_input_file(file_path):
    try:
        with open(file_path, 'r') as file:
            return file.readlines()
    except FileNotFoundError:
        sys.exit("Input file not found!")

# Function to extract dose data for S0 and S1
def extract_values(lines):
    s0_points = []
    s1_points = []
    target = None

    # Loop over lines
    for line in lines:

        # Identify the target data set
        if '@target G0.S0' in line:
            target = 'S0'
        elif '@target G0.S1' in line:
            target = 'S1'
        elif target and len(line.split()) == 3:
            try:
                x, y, dy = map(float, line.split())
                point = DataPoint(x, y, dy)
                if target == 'S0':
                    s0_points.append(point)
                elif target == 'S1':
                    s1_points.append(point)
            except ValueError:
                continue

    return s0_points, s1_points

# Function to write the extracted data to an output file
def write_output_file(output_file, s0_points, s1_points):

    # Check if data lengths match
    if len(s0_points) != len(s1_points):
        sys.exit("There is a different number of points in both data sets")

    with open(output_file, 'w') as file:
        # Write metadata for XMGrace
        file.write('@title "Perturbation function"\n')
        file.write('@xaxis label "Position  (cm)"\n')
        file.write('@yaxis label "Normalized perturbation value"\n')
        file.write('@target G0.S0\n')
        file.write('@type xydy\n')
        file.write('@s0 legend "Water"\n')
        file.write('@s1 legend "Chamber"\n')

        # Loop through points and calculate perturbations
        for i in range(len(s0_points)):
            (x0, y0, dy0) = s0_points[i].get()
            (x1, y1, dy1) = s1_points[i].get()
            position = (x0 + x1) / 2
            perturbation = (y0 / 0.555) - (y1 / 0.5)
            uncertainty = math.sqrt(dy0**2 + dy1**2)

            # Output each line in scientific notation
            file.write(f"{position:6.1f} {perturbation:14.6e} {uncertainty:14.6e}\n")

        # output xmgrace end of set marker
        file.write('&\n')

# Function to parse command-line arguments
def parse_arguments():
    try:
        opts, args = getopt.getopt(sys.argv[1:], "h", ["help"])
    except getopt.GetoptError as err:
        print(err)
        print("Usage: ./extract-perturbation.py input_file")
        sys.exit(2)

    for opt, argument in opts:
        if opt in ("-h", "--help"):
            print("Usage: ./extract-perturbation.py input_file")
            sys.exit(0)

    if not args:
        print("Usage: ./extract-perturbation.py input_file")
        sys.exit(2)

    return args[0]

# Main function
def main():
    input_file = parse_arguments()
    output_file = os.path.splitext(input_file)[0] + '-perturbation.dat'

    lines = read_input_file(input_file)
    s0_points, s1_points = extract_values(lines)

    write_output_file(output_file, s0_points, s1_points)
    print(f"Processing complete. Output file created: {output_file}")

if __name__ == "__main__":
    main()
