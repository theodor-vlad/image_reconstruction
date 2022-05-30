
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

std::ifstream fin("licenta\\input.txt");

//---------------------------------------- GLOBALS --------------------------------------------------
unsigned int mode, method;
std::string path_to_json, approximation_method;
Serializer s;

//--------------------------------------- FUNCTIONS -------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_S && action == GLFW_RELEASE && mode != 1)
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
    srand(time(NULL));

    int counter_target_img_pixels = 0;
    int r, g, b;
    fin >> mode;
    if (mode == 0) {
        // approximate
        fin >> method;
        fin >> path_to_image;
        fin >> IMG_WIDTH >> IMG_HEIGHT;
        num_of_bgra_values = IMG_WIDTH * IMG_HEIGHT * DIMENSIONS;
        targetImgPixels = new unsigned char[num_of_bgra_values];
        currChromoPixels = new unsigned char[num_of_bgra_values];

        for (int i = 0; i < IMG_WIDTH * IMG_HEIGHT; i++) {
            fin >> b >> g >> r;
            targetImgPixels[counter_target_img_pixels++] = b;
            targetImgPixels[counter_target_img_pixels++] = g;
            targetImgPixels[counter_target_img_pixels++] = r;
            targetImgPixels[counter_target_img_pixels++] = 0; // alpha, redundant
        }
    }
    else if (mode == 1) {
        // show
        fin >> path_to_json;
        if (false == Deserializer::get_and_set_screen_dimensions(path_to_json)) {
            std::cout << "Could not get and set screen dimensions from solution json. Exiting...\n";
            exit(-1);
        }
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

    s.set_algortihm(Serializer::algorithm_t(method));
}

//------------------------------------------ MAIN ----------------------------------------------------
int main(int argc, char** argv)
{
    initialize();

    if (mode == 0) {
        switch (Serializer::algorithm_t(method))
        {
        case Serializer::GENETIC:
            glfwSetWindowTitle(window, "Genetic Algorithm");
            GA::run();
            break;
        case Serializer::HILLCLIMBER_BEST:
            glfwSetWindowTitle(window, "Hillclimbing - Best Improvement");
            HC::run_best_improvement();
            break;
        case Serializer::HILLCLIMBER_FIRST:
            glfwSetWindowTitle(window, "Hillclimbing - First Improvement");
            HC::run_first_improvement();
            break;
        case Serializer::ANNEALER:
            glfwSetWindowTitle(window, "Simulated Annealing");
            SA::run();
            break;
        default:
            std::cout << "Unknown method chosen. Exiting...";
            exit(-1);
        }
    }
    else if (mode == 1) {
        Chromosome solution = Deserializer::reconstruct_solution_from_file(path_to_json);
        approximation_method = Deserializer::get_approximation_method(path_to_json);
        glfwSetWindowTitle(window, approximation_method.c_str());

        while (!glfwWindowShouldClose(window)) {
            solution.draw();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    delete[] currChromoPixels;
    return 0;
}