# Image Reconstruction

## What is it?

**Image Reconstruction** is a bachelor's degree in Computer Science project, which aims to reconstruct an input image using transparent overlapping polygons. This was inspired by Roger Alsing's "Evolution of Mona Lisa" project and is written in Python (GUI and image preprocessing) and C++ (computation and evolution visualization under OpenGL 1.0).

## How does it work?

**Image Reconstruction** tries to iteratively evolve a candidate/a population of candidates - with candidate here meaning a collection of polygons - such that the overlapping polygons in a candidate best approximate the input image. The fitness function is implemented as the channel-wise pixel distance between the input image and the candidate solution (i.e. the image obtained by drawing the candidate's polygons on an empty canvas). The user can choose from four approximation methods for the fitness function: a simulated annealer, two hillclimbers (steepst & nearest ascent), and a genetic algorithm. The GIF below exhibits the sped-up evolution of Mona Lisa in the span of approximately 4 hours, where noticeable results can be observed within the first 5 minutes of runtime:

![ezgif-5-b3a4374e8e](https://user-images.githubusercontent.com/22328874/228052162-8a1b511f-c3c8-4348-bd0a-6e053755712b.gif)


The results of this project were accepted and presented during the **24th International Symposium on Symbolic and Numeric Algorithms for Scientific Computing** (SYNASC 2022 - Austria), the paper being available on the IEEE Xplore portal: [link to publication](https://ieeexplore.ieee.org/document/10131018).

## How to run (Windows only):

1. Install python3.9 for Windows
1. Make sure you have MSBuild installed and added to your PATH system variable
1. Configure OpenGL + GLUT for Visual Studio: [tutorial](https://www.geeksforgeeks.org/how-to-setup-opengl-with-visual-studio-2019-on-windows-10/)
1. Open command prompt: `pip install pillow`
1. Open command prompt from `gui` folder: `python3 main.py`
1. Choose the image you want reconstructed from polygons, then click on `Run`
1. Wait cca. 30-45 mins for good results
