import subprocess
import sys
import os

# Check if the script is called with the correct number of arguments


infile = sys.argv[1]
outfile1 = sys.argv[2]
outfile2 = sys.argv[3]
program1_outputs = []


# Loop to run the programs with different input and output files

command2 = ["./smb", "sounds/" + infile, "autotest/" + outfile1]
result2 = subprocess.run(command2, stdout=subprocess.PIPE, text=True)
res2 = float(result2.stdout)

for j in range(2, 5, 1):
    for i in range(2, 9, 2):  # Incrementing by 2 from 2 to 8
        # Build the command lines with different input and output files
        # command1 = [
        #     "mpirun -n " + str(j) + " -ppn 8 ./mpishift",
        #     "sounds/" + infile,
        #     "sounds/" + str(i) + str(j) + outfile1,
        #     str(i),
        # ]
        command1 = [
            "mpirun",
            "-n",
            str(j),
            "-ppn",
            "8",
            "./mpishift",
            "sounds/" + infile,
            "auotest/" + str(i) + str(j) + outfile1,
            str(i),
        ]

        # Run the programs using subprocess
        result1 = subprocess.run(command1, stdout=subprocess.PIPE, text=True)
        program1_outputs.append(result1.stdout)

    for i, output in enumerate(program1_outputs, start=2):
        res1 = float(output)
        speedup = res2 / res1
        # Print CSV line
        print(f"{i},{j},{res1},{res2},{speedup}")

    # Reset the program1_outputs list for the next iteration of j
    program1_outputs = []
