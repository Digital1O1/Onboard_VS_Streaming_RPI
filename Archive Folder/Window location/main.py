import tkinter as tk

# Create a Tkinter window
root = tk.Tk()
root.geometry("640x480")  # Set window size
print("Click within the grey window to determine location")

# Function to print window location
def print_window_location():
    print("Window Location:", root.winfo_x(), root.winfo_y())

# Bind the print_window_location function to a mouse click event
root.bind("<Button-1>", lambda event: print_window_location())

# Run the Tkinter event loop
root.mainloop()

# Location to be placed 349 246