# Python program to create
# a file explorer in Tkinter
  
# import all components
# from the tkinter library
from tkinter import *
  
# import filedialog module
from tkinter import filedialog

import subprocess

filename = None
mode = None
approximation_method = 2

# Function for opening the
# file explorer window
def browseFiles():
    global filename
    global mode
    filename = filedialog.askopenfilename(initialdir = "/",
                                          title = "Select a File",
                                          filetypes = (("JPG files",
                                                        "*.jpg*"),
                                                        ("JSON files",
                                                        "*.json*")))
      
    # Change label contents
    label_file_explorer.configure(text="File Opened: " + filename)

    if filename.split('.')[-1] == 'jpg':
        mode = 0
        button_run.configure(text='Approximate image')
    else:
        mode = 1
        button_run.configure(text='Render DNA')
    
    button_run['state'] = 'normal'

def approximate_or_render():
    subprocess.run(['python3', '..\\image_processing\\read_and_print_pixels.py', str(mode), filename, str(approximation_method)])
    subprocess.run(['compile_and_run.bat'], shell=True)

def approximation_method_callback(selection):
    global approximation_method
    for i, method in enumerate(["Genetic Algorithm", "Best improvement HC", "First improvement HC", "Simulated Annealing"]):
        if method == selection:
            approximation_method = i
            break

# Create the root window
window = Tk()
  
# Set window title
window.title('Image approximator using polygons')
  
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
                     command = approximate_or_render)
button_run['state'] = 'disabled'

# approximation method
variable = StringVar(window)
variable.set("First improvement HC") # default value

w = OptionMenu(window, variable, "Genetic Algorithm", "Best improvement HC", "First improvement HC", "Simulated Annealing", command=approximation_method_callback)
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