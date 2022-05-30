#pragma once
#include "Chromosome.h"
#include <chrono>
#include <iostream>

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
        T = avg_temp / 2500.0;

        gen = 0;
        begin = std::chrono::steady_clock::now();
    }

    long long get_running_duration() {
        return std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    }

    void display() {
        curr.draw();
        sprintf(buf, "Generation: %d", gen);
        sprintf(buf2, "Fitness: %le", curr.fitness);
        sprintf(buf3, "Time: %ds", get_running_duration());
        sprintf(buf4, "Polygons: %d", curr.polygons.size());
        sprintf(buf5, "Temperature: %le", T);
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
            T *= 0.999;
            neighbours_visited = 0;
        }

        glutPostRedisplay();
    }
};