#pragma once
#include "utils.h"
#include "../building_blocks/Chromosome.h"
#include <chrono>
#include <iostream>

class Serializer;

namespace HCBI {
    Chromosome curr, next, best_next;
    int gen = 0, neighbours_visited = 0;
    std::chrono::steady_clock::time_point begin, end;
    std::vector<double> fitness_over_time;

    void init() {
        curr = Chromosome(POLY_MAX / 2 + POLY_MIN / 2);
        best_next = curr;
        best_next.fitness = curr.fitness;
        begin = std::chrono::steady_clock::now();
    }

    long long get_running_duration() {
        return std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    }

    void display() {
        curr.draw();

        unsigned int elapsed_time = get_running_duration();
        sprintf(buf, "Generation: %d", gen);
        sprintf(buf2, "Fitness: %.4lf%%", (1.0 - pow(curr.fitness, -0.5)) * 100.0);
        sprintf(buf3, "Time: %ds", elapsed_time);
        sprintf(buf4, "Polygons: %d", curr.polygons.size());

        float start = 0.93f;
        float diff = 0.065f;
        float xoff = -0.9f;
        drawText(buf, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start);
        drawText(buf2, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - diff);
        drawText(buf3, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - 2 * diff);
        drawText(buf4, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - 3 * diff);
        glutSwapBuffers();

        if (fitness_over_time.size() < elapsed_time) {
            fitness_over_time.push_back(curr.fitness);
        }
    }

    void idle() {
        end = std::chrono::steady_clock::now();

        next = curr;
        next.mutate();
        next.calculate_fitness();

        if (next.fitness > best_next.fitness) {
            best_next = next;
        }

        neighbours_visited++;

        if (neighbours_visited % 25 == 0) {
            if (best_next.fitness > curr.fitness) {
                curr = best_next;
            }
            gen++;
            neighbours_visited = 0;
        }
        
        glutPostRedisplay();
    }
};
