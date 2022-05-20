import os
import sys
from PIL import Image

DESIRED_HEIGHT = 500

im = Image.open(sys.argv[1])
im = im.resize((im.width * DESIRED_HEIGHT // im.height, DESIRED_HEIGHT))

pixels = list(im.getdata())
with open("F:\\licenta\\opengl\\licenta\\licenta\\pixels.txt", "w") as f:
    for i in range(im.height):
        for j in range(im.width // 2):
            pixels[i * im.width + j], pixels[i * im.width + im.width - j - 1] = pixels[i * im.width + im.width - j - 1], pixels[i * im.width + j]
    
    f.write(f'{im.width} {im.height}\n')
    for r, g, b in reversed(pixels):
        f.write(f'{r} {g} {b}\n')