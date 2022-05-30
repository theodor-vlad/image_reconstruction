import os
import sys
from PIL import Image

DESIRED_HEIGHT = 500

with open("../opengl/licenta/licenta/input.txt", "w") as f:

    f.write(f'{sys.argv[1]}\n')

    if sys.argv[1] == '0': # approximate
        f.write(f'{sys.argv[3]}\n')
        f.write(f'{sys.argv[2]}\n')
    elif sys.argv[1] == '1': # render
        windows_path = sys.argv[2].replace('/', '\\\\')
        f.write(f'{windows_path}\n')

    im = Image.open(sys.argv[2])
    im = im.resize((im.width * DESIRED_HEIGHT // im.height, DESIRED_HEIGHT))
    f.write(f'{im.width} {im.height}\n')

    if sys.argv[1] == '0': # approximate
        pixels = list(im.getdata())
        for i in range(im.height):
            for j in range(im.width // 2):
                aux = pixels[i * im.width + j]
                pixels[i * im.width + j] = pixels[i * im.width + im.width - j - 1]
                pixels[i * im.width + im.width - j - 1] = aux
        
        for r, g, b in reversed(pixels):
            f.write(f'{b} {g} {r}\n')
        
        
