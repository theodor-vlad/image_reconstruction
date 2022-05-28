#pragma once
#include "Chromosome.h"
#include <set>
#include <chrono>
#include <iostream>

namespace GA {
    std::vector<Chromosome> population;

	void run() {
        // GA
        std::vector<Chromosome> newPopulation, elites;
        for (int i = 0; i < POP_MAX; i++) {
            population.push_back(Chromosome(GENE_MAX / 2));
        }

        const unsigned int K = 3;
        double total_prob, selection_prob;
        std::vector<double> individual_prob, accumulated_prob;
        std::vector<std::pair<unsigned int, double>> crossover_prob;

        unsigned int gen_no = 0, local_minimum_count = 0;
        std::set<unsigned int> elites_indices;

        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        while (!glfwWindowShouldClose(window))
        {
            // calculate fitness where it is due
            for (auto& chromosome : population) {
                if (chromosome.should_update_fitness) {
                    chromosome.calculate_fitness();
                }
            }

            // determine the K fittest individuals
            elites.clear();
            elites_indices.clear();
            unsigned int biggest = population[0].polygons.size();
            unsigned int smallest = population[0].polygons.size();
            while (elites.size() < K)
            {
                unsigned best_idx = 0;
                double best_f = population[0].fitness;
                for (unsigned i = 1; i < population.size(); ++i) {
                    biggest = population[i].polygons.size() > biggest ? population[i].polygons.size() : biggest;
                    smallest = population[i].polygons.size() < smallest ? population[i].polygons.size() : smallest;
                    if (elites_indices.find(i) == elites_indices.end() && population[i].fitness > best_f)
                    {
                        best_f = population[i].fitness;
                        best_idx = i;
                    }
                }

                elites_indices.insert(best_idx);
                elites.push_back(population[best_idx]);

                if (elites.size() == 1) {
                    if (gen_no % 100 == 0) {
                        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
                        std::cout << "generation " << gen_no << ": " << best_f;
                        std::cout << ", smallest: " << smallest << ", biggest: " << biggest;
                        std::cout << " (time elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s)" << '\n';
                    }
                }
            }

            // roulette wheel selection
            total_prob = 0;
            for (int i = 0; i < population.size(); ++i)
                total_prob += population[i].fitness;

            individual_prob.clear();
            for (unsigned i = 0; i < population.size(); ++i)
                individual_prob.push_back(population[i].fitness / total_prob);

            accumulated_prob.clear();
            accumulated_prob.push_back(0);
            for (unsigned i = 0; i < population.size(); ++i)
                accumulated_prob.push_back(accumulated_prob[i] + individual_prob[i]);
            accumulated_prob[accumulated_prob.size() - 1] = 1.0;

            for (unsigned i = 0; i < population.size(); ++i)
            {
                selection_prob = rnd2;
                for (unsigned j = 0; j < population.size(); ++j)
                    if (accumulated_prob[j] < selection_prob && selection_prob <= accumulated_prob[j + 1])
                    {
                        newPopulation.push_back(population[j]);
                        break;
                    }
            }

            population = newPopulation;
            newPopulation.clear();

            // mutate
            for (auto& chromosome : population) {
                chromosome.mutate();
            }

            // crossover: generate probabilites for each individual
            crossover_prob.clear();
            for (unsigned i = 0; i < population.size(); ++i)
                crossover_prob.push_back({ i, rnd2 });

            // sort the individuals in the ascending order of the generated probabilites
            std::sort(crossover_prob.begin(), crossover_prob.end(), [](std::pair<unsigned, double> p1, std::pair<unsigned, double> p2) {
                return p1.second < p2.second;
                });

            //// ORDER CROSSOVER - USE WITH FIXED SIZE INDIVIDUALS
            //for (unsigned i = 0; i < population.size() - 1 && crossover_prob[i].second < CX_RATE; i += 2)
            //{
            //    // parents chosen for crossover
            //    unsigned firstParentIndex = crossover_prob[i].first;
            //    unsigned secondParentIndex = crossover_prob[i + 1].first;
            //    auto firstParent = population[firstParentIndex];
            //    auto secondParent = population[secondParentIndex];

            //    // determine the cutpoint
            //    unsigned int cutpoint = rnd4;

            //    // cross the parents over using order, single-cutpoint crossover, then override the parents
            //    std::tie(population[firstParentIndex], population[secondParentIndex]) = firstParent.order_crossover(secondParent, cutpoint);
            //}

            // CUT AND SPLICE CROSSOVER - USE WITH MESSY INDIVIDUALS
            for (unsigned i = 0; i < population.size() - 1 && crossover_prob[i].second < CX_RATE; i += 2)
            {
                // parents chosen for crossover
                unsigned firstParentIndex = crossover_prob[i].first;
                unsigned secondParentIndex = crossover_prob[i + 1].first;
                auto firstParent = population[firstParentIndex];
                auto secondParent = population[secondParentIndex];

                // cross the parents over using order, single-cutpoint crossover, then override the parents
                std::tie(population[firstParentIndex], population[secondParentIndex]) = firstParent.cut_and_splice(secondParent);
            }

            /// overriding the first K individuals with the elites
            for (unsigned i = 0; i < elites.size(); ++i) {
                population[i] = elites[i];
            }

            gen_no++;

            // display the best individual
            population[0].draw();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
	}
}