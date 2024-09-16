import csv
import time
import random

class FPSLogger:
    def __init__(self):
        self.fps_array = []
        self.current_frame = 0
        self.csv_file = 'tools/data/fps_data.csv'
        with open(self.csv_file, mode='w', newline='') as file:
            writer = csv.writer(file)
            writer.writerow(['Frame', 'fps'])
    def log_fps(self, fps):
        """Log FPS data at the Frame."""
        self.fps_array.append(fps)
        self.current_frame += 1
        # Append data to CSV file
        with open(self.csv_file, mode='a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([self.current_frame, fps])
    def simulate_data_collection(self, duration=1000):
        """Simulate data collection for a given duration(Frames)."""
        for _ in range(duration):
            fps = round(random.randint(0,1000))  # Random FPS value
            print(f"Frame {self.current_frame}: FPS = {fps}")
            self.log_fps(fps)