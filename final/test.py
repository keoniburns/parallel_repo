import subprocess
import sys
import os

# Check if the script is called with the correct number of arguments
if len(sys.argv) != 6:
    print(
        "Usage: python script.py <program1> <infile1> <outfile1> <program2> <infile2> <outfile2>"
    )
    sys.exit(1)

program1 = sys.argv[1]
infile1 = sys.argv[2]
outfile1 = sys.argv[3]

program2 = sys.argv[4]
infile2 = sys.argv[5]
outfile2 = sys.argv[6]

# Number of iterations
num_iterations = 5  # You can adjust this as needed

# Loop to run the programs with different input and output files
for i in range(2, 9, 2):  # Incrementing by 2 from 2 to 8
    # Build the command lines with different input and output files
    for j in range(2, 4, 1):
        command1 = [
            "mpirun -n " + str(j) + " -ppn 8 ./" + program1,
            "sounds/" + infile1,
            "sounds/" + outfile1 + str(i) + str(j),
            str(i),
        ]

    # Run the programs using subprocess
    result1 = subprocess.run(command1, stdout=subprocess.PIPE, text=True)
    program1_outputs.append(result1.stdout)

command2 = ["./" + program1, "sounds/" + infile1, "sounds/" + outfile1]
result2 = subprocess.run(command2, stdout=subprocess.PIPE, text=True)

for i, output in enumerate(program1_outputs, start=2):
    print(result2 / output)
