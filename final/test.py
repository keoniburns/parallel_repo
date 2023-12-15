import subprocess
import sys
import os

# Check if the script is called with the correct number of arguments


infile = sys.argv[1]
outfile1 = sys.argv[2]
outfile2 = sys.argv[3]
program1_outputs = []


# Loop to run the programs with different input and output files
print("parallel results")
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
        result1 = subprocess.run(command1, stdout=subprocess.PIPE, text=True)
        print("threads: " + str(i), "workers: " + str(j), result1)

        # Run the programs using subprocess

        # program1_outputs.append(result1.stdout)

command2 = ["./smb", "sounds/" + infile, "autotest/" + outfile1]
result2 = subprocess.run(command2, stdout=subprocess.PIPE, text=True)

print("sequential output", result2.stdout)
res2 = float(result2.stdout)
for i, output in enumerate(program1_outputs, start=2):
    print(f"speed up for iteration {i}")
    print(output)
    # print(output)
