import csv
import matplotlib.pyplot as plt

# Step 1: Read data from the CSV file
csv_file = ".csv"  # Replace with the path to your CSV file

x = []  # Data for X-axis
y = []  # Data for Y-axis

with open(csv_file, 'r') as file:
    reader = csv.reader(file)
    next(reader)  # Skip the header row if it exists

    for row in reader:
        x.append(row[0])  # Assuming the first column contains X-axis data
        y.append(float(row[1]))  # Assuming the second column contains Y-axis data

# Step 2: Create a bar chart
plt.figure(figsize=(10, 6))  # Set the figure size

plt.bar(x, y)
plt.xlabel("Categories")
plt.ylabel("Values")
plt.title("Bar Chart from CSV Data")

# Step 3: Save the chart as an image (e.g., PNG)
plt.savefig("chart.png")

# Step 4: Display the chart (optional)
plt.show()
