import tkinter as tk
from tkinter import messagebox

# Función para mostrar una ventana emergente
def mostrar_ventana_emergente():
    messagebox.showinfo("Ventana Emergente", "¡Hola! Esta es una ventana emergente.")

# Crear una ventana principal
ventana_principal = tk.Tk()
ventana_principal.title("Ventana Principal")

# Botón para mostrar la ventana emergente
boton = tk.Button(ventana_principal, text="Mostrar Ventana Emergente", command=mostrar_ventana_emergente)
boton.pack()

# Iniciar el bucle principal de la aplicación
ventana_principal.mainloop()
