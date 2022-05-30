#pragma once
#include "Chromosome.h"
#include <chrono>
#include <iostream>

namespace SA {
    Chromosome curr(POLY_MAX / 2);
    std::chrono::steady_clock::time_point begin, end;

    long long get_running_duration() {
        return std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    }

	void run() {
        curr.calculate_fitness();
        double avg_temp = 0.0;
        for (int i = 0; i < 100; i++) {
            Chromosome randomChromo(POLY_MAX / 2);
            randomChromo.calculate_fitness();
            avg_temp += randomChromo.fitness;
        }
        TEMPERATURE = avg_temp /= 1000;
        int gen = 0;
        begin = std::chrono::steady_clock::now();
        while (!glfwWindowShouldClose(window)) {

            end = std::chrono::steady_clock::now();
            if (gen % 10 == 0) {
                std::cout << "gen " << gen << ": " << curr.fitness << " (" << curr.polygons.size() << " polygons)";
                std::cout << " (time elapsed: " << get_running_duration() << "s)";
                std::cout << " (temp: " << TEMPERATURE << ")\n";
            }
        
            for (int i = 0; i < 100; i++) {
                Chromosome next = curr;
                next.mutate();
                next.calculate_fitness();

                if (next.fitness > curr.fitness || rnd2 < exp(-abs(next.fitness - curr.fitness) / TEMPERATURE)) {
                    curr = next;
                }

                curr.draw();
                glfwSwapBuffers(window);
                glfwPollEvents();
            }

            TEMPERATURE *= 0.999;
            gen++;
        }
	}
}