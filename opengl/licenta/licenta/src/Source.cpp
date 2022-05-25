
//--------------------------------------- INCLUDES -------------------------------------------------
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <set>
#include <iomanip>
#include "Chromosome.h"
#include "omp.h"
#include <thread>
#include "Hillclimber.h"
#include "Annealer.h"
#include "Genetic.h"
#include "Serializer.h"
#include "Deserializer.h"

std::ifstream fin("F:\\licenta\\opengl\\licenta\\licenta\\pixels.txt");

//---------------------------------------- GLOBALS --------------------------------------------------
unsigned int num_of_bgra_values, IMG_WIDTH, IMG_HEIGHT;
unsigned char* targetImgPixels;
unsigned char* currChromoPixels;
Serializer s;

//--------------------------------------- FUNCTIONS -------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_S && action == GLFW_RELEASE)
    {
        if (s.serialize()) {
            std::cout << "serialized successfully!\n";
        }
        else {
            std::cout << "serialization not successful...\n";
        }
    }
}

void initialize() {
    int counter_target_img_pixels = 0;
    int r, g, b;
    fin >> IMG_WIDTH >> IMG_HEIGHT;
    num_of_bgra_values = IMG_WIDTH * IMG_HEIGHT * DIMENSIONS;
    targetImgPixels = new unsigned char[num_of_bgra_values];
    currChromoPixels = new unsigned char[num_of_bgra_values];

    for (int i = 0; i < IMG_WIDTH * IMG_HEIGHT; i++) {
        fin >> r >> g >> b;
        targetImgPixels[counter_target_img_pixels++] = b;
        targetImgPixels[counter_target_img_pixels++] = g;
        targetImgPixels[counter_target_img_pixels++] = r;
        targetImgPixels[counter_target_img_pixels++] = 0; // alpha, redundant
    }

    /* Initialize the library */
    if (!glfwInit())
        exit(-1);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(IMG_WIDTH, IMG_HEIGHT, "Image reconstruction", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(-1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetKeyCallback(window, key_callback);

    s.set_algortihm(Serializer::HILLCLIMBER);
}

void Chromosome::draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& poly : polygons) {
        glBegin(GL_POLYGON);
        glColor4f(poly.color.r, poly.color.g, poly.color.b, poly.color.a); // RGBA
        for (auto& vertex : poly.vertices)
            glVertex2f(vertex.x, vertex.y);
        glEnd();
    }
}

void Chromosome::calculate_fitness() {

    // draw the chromosome to the screen
    draw();

    // capture pixels
    glReadPixels(0, 0, IMG_WIDTH, IMG_HEIGHT, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)currChromoPixels);

    // calculate pixel-by-pixel 3d distance between capture and target image and update fitness
    double fit = 0.0;
    #pragma omp parallel for schedule(guided, 1024) reduction(+:fit)
    for (int i = 0; i < num_of_bgra_values; i += 4) {
        fit += (double(currChromoPixels[i]) - double(targetImgPixels[i])) * (double(currChromoPixels[i]) - double(targetImgPixels[i]));
        fit += (double(currChromoPixels[i + 1]) - double(targetImgPixels[i + 1])) * (double(currChromoPixels[i + 1]) - double(targetImgPixels[i + 1]));
        fit += (double(currChromoPixels[i + 2]) - double(targetImgPixels[i + 2])) * (double(currChromoPixels[i + 2]) - double(targetImgPixels[i + 2]));
    }
    fitness = pow(fit, -2);

    should_update_fitness = false;
}
//------------------------------------------ MAIN ----------------------------------------------------
int main()
{
    initialize();
    
    //GA::run();
    //HC::run();
    //SA::run();

    Chromosome solution = Deserializer::deserialize("F:\\licenta\\image_processing\\dna\\hc\\Wed_May_25_18_45_16_2022.json");

    while (!glfwWindowShouldClose(window)) {
        solution.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    delete[] currChromoPixels;
    return 0;
}