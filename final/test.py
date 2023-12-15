import subprocess
import sys
import os

# Check if the script is called with the correct number of arguments


infile = sys.argv[1]
outfile1 = sys.argv[2]
outfile2 = sys.argv[3]
program1_outputs = []


# Loop to run the programs with different input and output files
for i in range(2, 9, 2):  # Incrementing by 2 from 2 to 8
    # Build the command lines with different input and output files
    for j in range(2, 5, 1):
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

newouts = []
for s in program1_outputs:
    newlist = s.strip().split("\n")
    newouts.append(float(value) for value in newlist)

command2 = ["./smb", "sounds/" + infile, "autotest/" + outfile1]
result2 = subprocess.run(command2, stdout=subprocess.PIPE, text=True)

print("sequential output", result2.stdout)
res2 = float(result2.stdout)
for i, output in enumerate(newouts, start=2):
    print(f"speed up for iteration {i}")
    print(output)
    # res1 = float(values_str)
    # res1 = [float(value) for value in values_str]
    speed = res2 / output
    # print(output)
    print(speed)
