#pragma once
#include "Chromosome.h"
#include <chrono>
#include <iostream>

namespace SA {
    Chromosome curr(1);

	void run() {

        curr.calculate_fitness();
        int gen = 0;
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        while (!glfwWindowShouldClose(window)) {

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::cout << "gen " << gen << ": " << curr.fitness << " (" << curr.polygons.size() << " polygons)";
            std::cout << " (time elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s)";
            std::cout << " (temp: " << TEMPERATURE << ")\n";
        
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

            TEMPERATURE *= 0.3;
            gen++;
        }
	}
}