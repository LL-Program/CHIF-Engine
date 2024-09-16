import customtkinter as ct
import time

class GraphicsSettings(ct.CTk):
    def __init__(self, app):
        super().__init__()
        self.app = app
        self.camera = app.camera
        self.geometry("400x400")  # Increased height for FPS/Position labels
        self.title("Graphics Settings")

        self.create_label_and_slider("FOV","fov", self.camera.fov, 50, 100, 0.2, 0.5)
        self.create_label_and_slider("NEAR","near",self.camera.near, 0.01, 1, 0.35, 0.5)
        self.create_label_and_slider("Render Distance","far", self.camera.far, 200, 10000, 0.5, 0.5)
        self.create_label_and_slider("SPEED","speed", self.camera.speed, 0.001, 0.05, 0.65, 0.5)
        self.create_label_and_slider("SENSITIVITY","sensivity",self.camera.sensitivity, 0.01, 1, 0.8, 0.5)
        self.physics_checkbox = ct.CTkCheckBox(master=self, text="Physics", onvalue=True, offvalue=False)
        self.physics_checkbox.pack(padx=20, pady=10)
        self.camera_checkbox = ct.CTkCheckBox(master=self, text="Camera Collision", onvalue=True, offvalue=False)
        self.camera_checkbox.pack(padx=20, pady=10)
        # FPS and Player Position labels
        self.fps_label = ct.CTkLabel(master=self, text="FPS: 0")
        self.fps_label.place(relx=0.5, rely=0.9, anchor=ct.CENTER)
        self.position_label = ct.CTkLabel(master=self, text="Position: (0, 0, 0)")
        self.position_label.place(relx=0.5, rely=0.85, anchor=ct.CENTER)
        self.apply_button = ct.CTkButton(master=self, text="Apply Settings", command=self.apply_settings)
        self.apply_button.place(relx=0.5, rely=0.95, anchor=ct.CENTER)
        self.update_labels()
        self.mainloop()
    def create_label_and_slider(self, text,id, default, min_val, max_val, rel_y, rel_x):
        label = ct.CTkLabel(master=self, text=text)
        label.place(relx=rel_x - 0.2, rely=rel_y, anchor=ct.CENTER)
        slider = ct.CTkSlider(master=self, from_=min_val, to=max_val, number_of_steps=100)
        slider.set(default)
        slider.place(relx=rel_x + 0.2, rely=rel_y, anchor=ct.CENTER)
        setattr(self, f"{id}_slider", slider)
    def apply_settings(self):
        self.camera.fov = self.fov_slider.get()
        self.camera.near = self.near_slider.get()
        self.camera.far = self.far_slider.get()
        self.camera.speed = self.speed_slider.get()
        self.camera.sensitivity = self.sensivity_slider.get()
        self.app.physics = self.physics_checkbox.get()
        self.app.camera.collision_on = self.camera_checkbox.get()
        self.destroy()
    def update_labels(self):
        # Calculate FPS (using app's delta_time, assuming it's set in app)
        fps = 1.0 / self.app.delta_time if self.app.delta_time > 0 else 0
        # Update the FPS label
        self.fps_label.configure(text=f"FPS Configured")
        # Update the position label (from camera position)
        pos = self.camera.position
        self.position_label.configure(text=f"Position: ({pos.x:.2f}, {pos.y:.2f}, {pos.z:.2f})")
        # Schedule the next update (refresh every 100ms or 0.1s)
        self.after(100, self.update_labels)

