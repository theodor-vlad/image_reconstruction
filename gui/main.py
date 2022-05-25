# Python program to create
# a file explorer in Tkinter
  
# import all components
# from the tkinter library
from tkinter import *
  
# import filedialog module
from tkinter import filedialog

import subprocess

filename = None

# Function for opening the
# file explorer window
def browseFiles():
    global filename
    filename = filedialog.askopenfilename(initialdir = "/",
                                          title = "Select a File",
                                          filetypes = (("JPG files",
                                                        "*.jpg*"),
                                                       ("All files",
                                                        "*.*")))
      
    # Change label contents
    label_file_explorer.configure(text="File Opened: "+filename)

def extract_pixels_and_run_GA():
    subprocess.run(['python3', '..\\image_processing\\read_and_print_pixels.py', filename])
    subprocess.run(['compile_and_run.bat'], shell=True)

# Create the root window
window = Tk()
  
# Set window title
window.title('File Explorer')
  
# Set window size
window.geometry("500x500")
  
#Set window background color
window.config(background = "white")
  
# Create a File Explorer label
label_file_explorer = Label(window,
                            text = "File Explorer using Tkinter",
                            width = 100, height = 4,
                            fg = "blue")
  
      
button_explore = Button(window,
                        text = "Browse Files",
                        command = browseFiles)
  
button_run = Button(window,
                     text = "Run",
                     command = extract_pixels_and_run_GA)

# approximation method
variable = StringVar(window)
variable.set("Hillclimber") # default value

w = OptionMenu(window, variable, "Genetic Algorithm", "Hillclimber", "Simulated Annealing")
w.pack()
w.place(x=240,y=250)

# Grid method is chosen for placing
# the widgets at respective positions
# in a table like structure by
# specifying rows and columns
label_file_explorer.grid(column = 1, row = 1)
  
button_explore.grid(column = 1, row = 2)
  
button_run.grid(column = 1,row = 3)
  
# Let the window wait for any events
window.mainloop()