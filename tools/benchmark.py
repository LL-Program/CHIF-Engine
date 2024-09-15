import matplotlib
import matplotlib.pyplot as plt
import csv
import numpy as np
# Switch to the Agg backend
matplotlib.use('Agg')

def read_and_plot_fps(csv_file, skip=2):
    seconds = []
    fps_values = []

    with open(csv_file, mode='r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            seconds.append(int(row['Frame']))
            fps_values.append(float(row['fps']))

    # Reduce the number of points by skipping N points
    skip_value = round(len(seconds)/1500)
    reduced_seconds = seconds[::skip_value]  # Plot every Nth point
    reduced_fps_values = fps_values[::skip_value]
    mean = np.mean(fps_values)
    # Plot the reduced data
    plt.plot(reduced_seconds, reduced_fps_values, linestyle='-')
    plt.title(f'FPS over Time (Middle : {mean})')
    plt.xlabel(f'Frames (Skip: {skip_value})')
    plt.ylabel(f'FPS (Skip: {skip_value})')
    plt.grid(True)
    plt.savefig('stats/fps_plot.png')
    plt.show()

csv_file = 'data/fps_data.csv'
read_and_plot_fps(csv_file, skip=1)
