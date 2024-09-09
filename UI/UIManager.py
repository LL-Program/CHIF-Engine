#TODO:  Verschiedene Funktionen für Fenster (1 : Grafikeinstellungen, 2 : Objekte)
import customtkinter

customtkinter.set_appearance_mode("System")  # Modes: system (default), light, dark
customtkinter.set_default_color_theme("blue")  # Themes: blue (default), dark-blue, green

class UIManager:
    def __init__(self,Engine):
        self.engine = Engine
    def GraphicSettings(self):
        """
        z.B
        self.app = customtkinter.CTk()
        self.app.geometry("400x240")
        button = customtkinter.CTkButton(master=app, text="CTkButton", command=button_function)
        button.place(relx=0.5, rely=0.5, anchor=customtkinter.CENTER)
        app.mainloop()
        """
        pass