import random

n = 50  # Number of rows
m = 50  # Number of columns
nn = 50
mm = 50
file_name = "50x50.dat"


def generate_random_numbers(n, m):
    data = [[random.randint(1, 9) for _ in range(m)] for _ in range(n)]
    return data


def save_to_dat_file(data, filename):
    with open(filename, "a") as file:  # Use "a" mode to append to the file
        for row in data:
            line = " ".join(map(str, row))
            file.write(line + "\n")
        # file.write("\n")  # Add an empty line to separate the matrices


if __name__ == "__main__":
    with open(file_name, "w") as file:  # Create or overwrite the file
        pass  # Clear the file if it already exists

    random_data1 = generate_random_numbers(n, m)
    random_data2 = generate_random_numbers(nn, mm)
    save_to_dat_file(random_data1, file_name)
    save_to_dat_file(random_data2, file_name)
