import customtkinter as ct

class GraphicsSettings(ct.CTk):
    def __init__(self, app):
        super().__init__()  
        self.app = app
        self.camera = app.camera
        self.geometry("400x300")
        self.title("Graphics Settings")
        #Funktioniert Noch nicht richtig :(
        # #self.create_Window_Size_Menu()
        self.create_label_and_slider("FOV", 50, 30, 120, 0.2, 0.5)
        self.create_label_and_slider("NEAR", 0.1, 0.01, 1, 0.35, 0.5)
        self.create_label_and_slider("FAR", 100, 50, 200, 0.5, 0.5)
        self.create_label_and_slider("SPEED", 0.009, 0.001, 0.05, 0.65, 0.5)
        self.create_label_and_slider("SENSITIVITY", 0.04, 0.01, 1, 0.8, 0.5)
    
        self.apply_button = ct.CTkButton(master=self, text="Apply Settings", command=self.apply_settings)
        self.apply_button.place(relx=0.5, rely=0.95, anchor=ct.CENTER)

        self.mainloop()

    def create_label_and_slider(self, text, default, min_val, max_val, rel_y, rel_x):
        label = ct.CTkLabel(master=self, text=text)
        label.place(relx=rel_x - 0.2, rely=rel_y, anchor=ct.CENTER)

        slider = ct.CTkSlider(master=self, from_=min_val, to=max_val, number_of_steps=100)
        slider.set(default)
        slider.place(relx=rel_x + 0.2, rely=rel_y, anchor=ct.CENTER)
        
        setattr(self, f"{text.lower()}_slider", slider)
    def create_Window_Size_Menu(self):
        self.window_sizes = ["1600x900", "1280x720", "1920x1080", "1024x768"]
        
        # Dropdown menu for selecting window size
        self.size_option = ct.CTkComboBox(master=self, values=self.window_sizes, command=self.on_select_size)
        self.size_option.set(self.window_sizes[0])  # Default selection
        self.size_option.pack(pady=10)
    def apply_settings(self):
        self.camera.fov = self.fov_slider.get()
        self.camera.near = self.near_slider.get()
        self.camera.far = self.far_slider.get()
        self.camera.speed = self.speed_slider.get()
        self.camera.sensitivity = self.sensitivity_slider.get()
        self.destroy()
        #print(f"FOV: {self.camera.fov}, NEAR: {self.camera.near}, FAR: {self.camera.far}, SPEED: {self.camera.speed}, SENSITIVITY: {self.camera.sensitivity}")
    def apply_size(self):
        width, height = map(int, self.selected_size.split('x'))
        self.app.WIN_SIZE = (width, height)
        self.app.update_window_size()
        #print(f"Applied Size: {self.app.WIN_SIZE}")
    def on_select_size(self, choice):
        self.selected_size = choice
        #print(f"Selected Size: {self.selected_size}")


if __name__ == "__main__":
    app = None 
    graphics = GraphicsSettings(app)
