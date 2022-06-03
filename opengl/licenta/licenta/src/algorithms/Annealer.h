#pragma once
#include "../building_blocks/Chromosome.h"
#include <chrono>
#include <iostream>

class Serializer;

namespace SA {
    Chromosome curr, next;
    std::chrono::steady_clock::time_point begin, end;
    double T;
    int gen = 0, neighbours_visited = 0;

    void init() {
        curr = Chromosome(POLY_MAX / 2);

        double avg_temp = 0.0;
        for (int i = 0; i < 100; i++) {
            Chromosome randomChromo(POLY_MAX / 2);
            avg_temp += randomChromo.fitness;
        }
        T = avg_temp / 5000.0;

        gen = 0;
        begin = std::chrono::steady_clock::now();
    }

    long long get_running_duration() {
        return std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    }

    void display() {
        curr.draw();
        sprintf(buf, "Generation: %d", gen);
        sprintf(buf2, "Fitness: %.4lf%%", (1.0 - pow(curr.fitness, -0.5)) * 100.0);
        sprintf(buf3, "Time: %ds", get_running_duration());
        sprintf(buf4, "Polygons: %d", curr.polygons.size());
        sprintf(buf5, "Temperature: %.4le", T);
        float start = 0.93f;
        float diff = 0.065f;
        float xoff = -0.9f;
        drawText(buf, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start);
        drawText(buf2, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - diff);
        drawText(buf3, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - 2 * diff);
        drawText(buf4, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - 3 * diff);
        drawText(buf5, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, 1.0 + xoff, start);
        glutSwapBuffers();
    }

    void idle() {
        end = std::chrono::steady_clock::now();

        next = curr;
        next.mutate();
        next.calculate_fitness();

        if (next.fitness > curr.fitness || rnd2(rgen) < exp(-abs(next.fitness - curr.fitness) / T)) {
            curr = next;
        }

        neighbours_visited++;

        if (neighbours_visited % 100 == 0) {
            gen++;
            T *= 0.995;
            neighbours_visited = 0;
        }

        glutPostRedisplay();
    }
};