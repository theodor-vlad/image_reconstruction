import os
import sys
from PIL import Image

im = Image.open(sys.argv[1])
im = im.resize((400, 300))

pixels = list(im.getdata())
with open("F:\\licenta\\opengl\\licenta\\licenta\\pixels.txt", "w") as f:
    for i in range(im.height):
        for j in range(im.width // 2):
            pixels[i * 400 + j], pixels[i * 400 + 400 - j - 1] = pixels[i * 400 + 400 - j - 1], pixels[i * 400 + j]
    for r, g, b in reversed(pixels):
        f.write(f'{r} {g} {b}\n')