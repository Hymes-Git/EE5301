import matplotlib.pyplot as plt
import pandas as pd
import sys

if len(sys.argv) > 1:
    circuitName = sys.argv[1]
else:
    circuitName = ""

# WRITTEN EXCLUSIVELY USING MICROSOFT COPILOT

prespreadFileName = "./positions/positions_prespread_" + circuitName + ".csv" 
postspreadFileName = "./positions/positions_postspread_" + circuitName + ".csv"

# Read the data from the CSV file
try:
    df = pd.read_csv(prespreadFileName)
except FileNotFoundError:
    print(f"The file {prespreadFileName} was not found.")
    exit()

# Trim any extra spaces from the column names
df.columns = df.columns.str.strip()

# Check if the expected columns are present
if not all(col in df.columns for col in ['type', 'x coordinate', 'y coordinate']):
    print("The CSV file does not contain the required columns: 'type', 'x coordinate', 'y coordinate'.")
    print(f"Columns found: {df.columns.tolist()}")
    exit()

# Replace the old type names with the new ones
type_mapping = {'a': 'cell', 's': 'star', 'p': 'pin'}
df['type'] = df['type'].map(type_mapping)

# Define colors and markers for each type
colors = {'cell': 'red', 'star': 'blue', 'pin': 'green'}
markers = {'cell': 'o', 'star': 's', 'pin': '^'}

# Create the plot
plt.figure(figsize=(10, 10))

# Plot each type with different color and transparency
for cell_type in df['type'].unique():
    subset = df[df['type'] == cell_type]
    plt.scatter(subset['x coordinate'], subset['y coordinate'], 
                c=colors[cell_type], label=cell_type, alpha=0.5, marker=markers[cell_type])

# Add legend and labels
plt.legend()
plt.xlabel('X Coordinate')
plt.ylabel('Y Coordinate')
plt.title('Cell and Pin Locations')

# Save the plot as a PNG file

fileName = "./images/prespread_cell_and_pin_locations_" + circuitName + ".png"
plt.savefig(fileName)

# Show the plot (optional)
plt.show()

try:
    df = pd.read_csv(postspreadFileName)
except FileNotFoundError:
    print(f"The file {postspreadFileName} was not found.")
    exit()

# Trim any extra spaces from the column names
df.columns = df.columns.str.strip()

# Check if the expected columns are present
if not all(col in df.columns for col in ['type', 'x coordinate', 'y coordinate']):
    print("The CSV file does not contain the required columns: 'type', 'x coordinate', 'y coordinate'.")
    print(f"Columns found: {df.columns.tolist()}")
    exit()

# Replace the old type names with the new ones
type_mapping = {'a': 'cell', 's': 'star', 'p': 'pin'}
df['type'] = df['type'].map(type_mapping)

# Define colors and markers for each type
colors = {'cell': 'red', 'star': 'blue', 'pin': 'green'}
markers = {'cell': 'o', 'star': 's', 'pin': '^'}

# Create the plot
plt.figure(figsize=(10, 10))

# Plot each type with different color and transparency
for cell_type in df['type'].unique():
    subset = df[df['type'] == cell_type]
    plt.scatter(subset['x coordinate'], subset['y coordinate'], 
                c=colors[cell_type], label=cell_type, alpha=0.5, marker=markers[cell_type])

# Add legend and labels
plt.legend()
plt.xlabel('X Coordinate')
plt.ylabel('Y Coordinate')
plt.title('Cell and Pin Locations')

# Save the plot as a PNG file

fileName = "./images/postspread_cell_and_pin_locations_" + circuitName + ".png"
plt.savefig(fileName)

# Show the plot (optional)
plt.show()
