import matplotlib.pyplot as plt
import pandas as pd

# WRITTEN EXCLUSIVELY USING MICROSOFT COPILOT

# Read the data from the CSV file
try:
    df = pd.read_csv('positions.csv')
except FileNotFoundError:
    print("The file 'positions.csv' was not found.")
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
plt.savefig('cell_and_pin_locations.png')

# Show the plot (optional)
plt.show()
