#pragma once
#include "utils.h"
#include "Chromosome.h"
#include <chrono>
#include <iostream>
#include "GLFW/glfw3.h"

namespace HC {
    Chromosome curr(POLY_MAX / 2);
    std::chrono::steady_clock::time_point begin, end;

    long long get_running_duration() {
        return std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    }

	void run_best_improvement() {
        Chromosome next, best_next;
        curr.calculate_fitness();
        int gen = 0;
        begin = std::chrono::steady_clock::now();
        while (!glfwWindowShouldClose(window)) {

            if (gen % 100 == 0) {
                end = std::chrono::steady_clock::now();
                std::cout << "gen " << gen << ": " << curr.fitness << " (" << curr.polygons.size() << " polygons)";
                std::cout << " (time elapsed: " << get_running_duration() << "s)" << '\n';
            }
            gen++;

            best_next = curr;
            best_next.fitness = curr.fitness;
            for (int i = 0; i < 25; i++) {
                next = curr;
                next.mutate();
                next.calculate_fitness();

                if (next.fitness > best_next.fitness) {
                    best_next = next;
                }
            }

            if (best_next.fitness > curr.fitness) {
                curr = best_next;
            }

            curr.draw();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
	}

    void run_first_improvement() {
        curr.calculate_fitness();
        int gen = 0;
        begin = std::chrono::steady_clock::now();
        while (!glfwWindowShouldClose(window)) {

            end = std::chrono::steady_clock::now();
            if (gen % 1000 == 0) {
                std::cout << "gen " << gen << ": " << curr.fitness << " (" << curr.polygons.size() << " polygons)";
                std::cout << " (time elapsed: " << get_running_duration() << "s)" << '\n';
            }
            gen++;

            Chromosome next = curr;
            next.mutate();
            next.calculate_fitness();

            if (next.fitness > curr.fitness) {
                curr = next;
            }

            curr.draw();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
}