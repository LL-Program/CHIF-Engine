import tkinter as tk
import customtkinter as ctk
import pygame as pg
import moderngl
import numpy as np
from PIL import Image, ImageTk
#TODO Dominik Pascher
class App(ctk.CTk):
    def __init__(self):
        super().__init__()
        self.geometry("1920x1080")
        self.title("CHIF Editor")
        self.create_toolbar()
        self.create_main_frame()
        self.create_status_bar()
        self.init_pygame_and_modern_gl()

    def create_toolbar(self):
        toolbar = ctk.CTkFrame(self)
        toolbar.pack(side="top", fill="x")
        add_button = ctk.CTkButton(toolbar, text="Add Item", command=self.add_item)
        add_button.pack(side="left", padx=2, pady=2)

        remove_button = ctk.CTkButton(toolbar, text="Remove Item", command=self.remove_item)
        remove_button.pack(side="left", padx=2, pady=2)

        self.status_var = tk.StringVar()
        self.status_label = ctk.CTkLabel(toolbar, textvariable=self.status_var)
        self.status_label.pack(side="right", padx=10)

    def create_main_frame(self):
        self.main_frame = ctk.CTkFrame(self)
        self.main_frame.pack(expand=True, fill="both")
        self.create_list_widget()
        self.create_properties_editor()
        self.create_process_binding_widget()
        self.create_rendering_area()

    def create_list_widget(self):
        self.list_frame = ctk.CTkFrame(self.main_frame)
        self.list_frame.pack(side="left", fill="y", padx=10, pady=10)
        self.listbox = tk.Listbox(self.list_frame, selectmode=tk.SINGLE)
        self.listbox.pack(expand=True, fill="both")
        self.listbox.insert(tk.END, "Parent 1")
        self.listbox.insert(tk.END, "Parent 2")
        self.listbox.bind("<<ListboxSelect>>", self.update_properties)

    def create_properties_editor(self):
        self.properties_frame = ctk.CTkFrame(self.main_frame)
        self.properties_frame.pack(side="right", fill="y", padx=10, pady=10)
        self.position_label = ctk.CTkLabel(self.properties_frame, text="Position:")
        self.position_label.pack(pady=5)
        self.position_entry = ctk.CTkEntry(self.properties_frame)
        self.position_entry.pack(pady=5)
        self.size_label = ctk.CTkLabel(self.properties_frame, text="Size:")
        self.size_label.pack(pady=5)
        self.size_entry = ctk.CTkEntry(self.properties_frame)
        self.size_entry.pack(pady=5)
        self.rotation_label = ctk.CTkLabel(self.properties_frame, text="Rotation:")
        self.rotation_label.pack(pady=5)
        self.rotation_entry = ctk.CTkEntry(self.properties_frame)
        self.rotation_entry.pack(pady=5)
    def create_process_binding_widget(self):
        self.process_frame = ctk.CTkFrame(self.main_frame)
        self.process_frame.pack(expand=True, fill="both", padx=10, pady=10)
        self.process_label = ctk.CTkLabel(self.process_frame, text="Bind Process:")
        self.process_label.pack(pady=5)
        self.process_entry = ctk.CTkEntry(self.process_frame)
        self.process_entry.pack(pady=5)
        self.bind_button = ctk.CTkButton(self.process_frame, text="Bind", command=self.bind_process)
        self.bind_button.pack(pady=5)
    def create_rendering_area(self):
        self.render_frame = ctk.CTkFrame(self.main_frame)
        self.render_frame.pack(expand=True, fill="both", padx=10, pady=10)
        self.canvas = ctk.CTkCanvas(self.render_frame, bg="black")
        self.canvas.pack(expand=True, fill="both")
        self.ctx = None
        self.fbo = None
        self.image = None
    def create_status_bar(self):
        status_frame = ctk.CTkFrame(self)
        status_frame.pack(side="bottom", fill="x")
        self.status_message = ctk.CTkLabel(status_frame, text="Welcome to the CustomTkinter App!")
        self.status_message.pack(side="left", padx=10)
    def update_properties(self, event):
        selected = self.listbox.curselection()
        if selected:
            index = selected[0]
            item_text = self.listbox.get(index)
            self.status_var.set(f"Selected: {item_text}")

            self.position_entry.delete(0, tk.END)
            self.position_entry.insert(0, "100, 200")
            self.size_entry.delete(0, tk.END)
            self.size_entry.insert(0, "50, 100")
            self.rotation_entry.delete(0, tk.END)
            self.rotation_entry.insert(0, "0")

    def add_item(self):
        new_item = "New Item"
        self.listbox.insert(tk.END, new_item)
        self.status_var.set(f"Added: {new_item}")

    def remove_item(self):
        selected = self.listbox.curselection()
        if selected:
            item_text = self.listbox.get(selected[0])
            self.listbox.delete(selected[0])
            self.status_var.set(f"Removed: {item_text}")

    def bind_process(self):
        process_name = self.process_entry.get()
        print(f"Process '{process_name}' bound!")
        self.status_var.set(f"Process '{process_name}' bound!")

    def init_pygame_and_modern_gl(self):
        pg.init()
        pg.display.set_mode((640, 480), pg.OPENGL | pg.DOUBLEBUF)
        self.ctx = moderngl.create_context()
        self.fbo = self.ctx.framebuffer(color_attachments=[self.ctx.texture((640, 480), 4)])
        self.render_loop()

    def render_loop(self):
        self.ctx.clear()
        self.ctx.enable(moderngl.DEPTH_TEST)
        vertices = np.array([
            -0.6, -0.4,
            0.6, -0.4,
            0.0, 0.6
        ], dtype='f4')

        vbo = self.ctx.buffer(vertices)
        shader_program = self.ctx.program(
            vertex_shader="""
            #version 330
            in vec2 in_vert;
            void main() {
                gl_Position = vec4(in_vert, 0.0, 1.0);
            }
            """,
            fragment_shader="""
            #version 330
            out vec4 fragColor;
            void main() {
                fragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Red color
            }
            """,
        )

        vao = self.ctx.simple_vertex_array(shader_program, vbo, 'in_vert')
        vao.render(moderngl.TRIANGLES)
        data = self.fbo.read(components=3)
        image = Image.frombytes('RGB', (640, 480), data)
        self.image = ImageTk.PhotoImage(image)
        self.canvas.delete("all")
        self.canvas.create_image(0, 0, anchor='nw', image=self.image)

        pg.display.flip()

        self.after(16, self.render_loop)

if __name__ == "__main__":
    app = App()
    app.mainloop()



