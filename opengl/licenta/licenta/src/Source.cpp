
//--------------------------------------- INCLUDES -------------------------------------------------
#include <GL/glut.h>
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
#include "HCFI.h"
#include "HCBI.h"
#include "Annealer.h"
#include "Genetic.h"
#include "Serializer.h"
#include "Deserializer.h"
#include <memory>
#include <functional>
#include <Windows.h>

std::ifstream fin("licenta\\input.txt");

//---------------------------------------- GLOBALS --------------------------------------------------
unsigned int mode, method;
std::string path_to_json, approximation_method, time_elapsed;
Serializer s;
Chromosome solution;

//--------------------------------------- FUNCTIONS -------------------------------------------------
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    if (key == GLFW_KEY_S && action == GLFW_RELEASE && mode != 1)
//    {
//        if (s.serialize()) {
//            std::cout << "serialized successfully!\n";
//        }
//        else {
//            std::cout << "serialization not successful...\n";
//        }
//    }
//}

void key_pressed(unsigned char key, int x, int y) {
    if (key == 83 || key == 115) {
        if (s.serialize()) {
            std::cout << "serialized successfully!\n";
        }
        else {
            std::cout << "serialization not successful...\n";
        }
    }
}

void initialize(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

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

    glutInitWindowSize(IMG_WIDTH, IMG_HEIGHT + 100);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Image approximation");
    glutKeyboardFunc(key_pressed);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    s.set_algortihm(Serializer::algorithm_t(method));
}

void display() {
    solution.draw();
    sprintf(buf, "Polygons: %d", solution.polygons.size());
    sprintf(buf2, "Fitness: %le", solution.fitness);
    sprintf(buf3, "Time elapsed: %ds", time_elapsed);
    float start = 0.93f;
    float diff = 0.065f;
    float xoff = -0.9f;
    drawText(buf, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start);
    drawText(buf2, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - diff);
    drawText(buf3, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - 2 * diff);
    glutSwapBuffers();
}

void idle() {
    glutPostRedisplay();
}

//------------------------------------------ MAIN ----------------------------------------------------
int main(int argc, char** argv)
{
    //FreeConsole();
    initialize(argc, argv);

    if (mode == 0) {
        switch (Serializer::algorithm_t(method))
        {
        case Serializer::GENETIC:
            glutSetWindowTitle("Genetic Algorithm");
            GA::init();
            glutDisplayFunc(GA::display);
            glutIdleFunc(GA::idle);
            break;
        case Serializer::HILLCLIMBER_BEST:
            glutSetWindowTitle("Hillclimbing - Best Improvement");
            HCBI::init();
            glutDisplayFunc(HCBI::display);
            glutIdleFunc(HCBI::idle);
            break;
        case Serializer::HILLCLIMBER_FIRST:
            glutSetWindowTitle("Hillclimbing - First Improvement");
            HCFI::init();
            glutDisplayFunc(HCFI::display);
            glutIdleFunc(HCFI::idle);
            break;
        case Serializer::ANNEALER:
            glutSetWindowTitle("Simulated Annealing");
            SA::init();
            glutDisplayFunc(SA::display);
            glutIdleFunc(SA::idle);
            break;
        default:
            std::cout << "Unknown method chosen. Exiting...";
            exit(-1);
        }
    }
    else if (mode == 1) {
        solution = Deserializer::reconstruct_solution_from_file(path_to_json);
        approximation_method = Deserializer::get_approximation_method(path_to_json);
        time_elapsed = Deserializer::get_running_time(path_to_json);
        glutSetWindowTitle(approximation_method.c_str());
        glutDisplayFunc(display);
        glutIdleFunc(idle);
    }

    glutMainLoop();
    delete[] currChromoPixels;
    return 0;
}