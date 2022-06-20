
# imports
from tkinter import *
from tkinter import filedialog
from PIL import ImageTk, Image
import subprocess
import json

# globals
filename = "../image_processing/sources/caragiale1.jpg"
mode = 0
approximation_method = 2
DESIRED_HEIGHT = 500
welcome_text = """If you wish to recreate an image, save it in a .JPG format, then select it from here 
    along with an approximation method. If you want to display an already generated solution, select 
    the .JSON file corresponding to it. When you are done with either selection, click on the \"Run\" button."""

# functions
def browseFiles():
    global filename, mode, label, button_run
    filename = filedialog.askopenfilename(
        initialdir = "/",
        title = "Select a File",
        filetypes = (
            ("Images (.jpg files)", "*.jpg*"),
            ("Solution DNAs (.json files)", "*.json*")
        )
    )

    # error checking
    if '' == filename:
        label.config(text=welcome_text + '\nNo file selected!')
    else:
        label.config(text=welcome_text + f'\nFile selected: {filename}')

    def changeLabelImage(path_to_image):
        img = Image.open(path_to_image)
        img = img.resize((img.width * DESIRED_HEIGHT // img.height, DESIRED_HEIGHT))
        tkImg = ImageTk.PhotoImage(img)
        imgLabel.config(image=tkImg)
        imgLabel.image = tkImg

    if filename.split('.')[-1] == 'jpg':
        mode = 0
        changeLabelImage(filename)
    elif filename.split('.')[-1] == 'json':
        mode = 1
        f = open(filename)
        dna = json.load(f)
        changeLabelImage(dna['path_to_image'])

def approximate_or_render():
    # error checking
    global label
    if '' == filename:
        return

    # other python script
    subprocess.run([
        'python3',  # version of python interpreter used
        '..\\image_processing\\read_and_print_pixels.py',  # path to script to run
        str(mode),  # approximation or rendering
        filename,  # path to image/json
        str(approximation_method)  # method of approximation, in case mode == 0
    ])

    # compiling and running the C++ project
    subprocess.run(['compile_and_run.bat'], shell=True)

def approximation_method_callback(selection):
    global approximation_method
    for i, method in enumerate(["Genetic Algorithm", "Best improvement HC", "First improvement HC", "Simulated Annealing"]):
        if method == selection:
            approximation_method = i
            break

# main
root = Tk()
root.title('Image approximator')
root.geometry("700x650")
root.config(background = "white")

# info label
label = Label(
    root,
    text = welcome_text,
    width = 100,
    pady=5
)
label.pack(side=TOP)

# image
img = Image.open(filename)
img = img.resize((img.width * DESIRED_HEIGHT // img.height, DESIRED_HEIGHT))
tkImg = ImageTk.PhotoImage(img)
imgLabel = Label(root, image=tkImg)
imgLabel.pack(side=TOP)

# browse and run buttons
frame1 = Frame(root)
button_explore = Button(frame1, text = "Browse Files", command = browseFiles).pack(side=LEFT)
button_run = Button(frame1, text = "Run", command = approximate_or_render).pack(side=LEFT)
frame1.pack(side=TOP)

# approximation method dropdown
stringVar = StringVar(root)
stringVar.set("First improvement HC") # default value
optionsMenu = OptionMenu(
    root, stringVar,
    "Genetic Algorithm", "Best improvement HC", "First improvement HC", "Simulated Annealing", 
    command=approximation_method_callback
)
optionsMenu.pack(side=TOP)

# main loop
root.mainloop()