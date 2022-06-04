#pragma once
#include "../building_blocks/Chromosome.h"
#include <set>
#include <chrono>
#include <iostream>

class Serializer;

namespace GA {
    std::vector<Chromosome> population, newPopulation, elites, afterCrossover;
    double total_prob, selection_prob, fitness_last_gen;
    std::vector<double> individual_prob, accumulated_prob, fitness_over_time;
    std::vector<std::pair<unsigned int, double>> crossover_prob;
    unsigned int gen = 0, local_minimum_count = 0, biggest, smallest;
    unsigned int max_stagnated = 0, stagnated_fitness_generations = 0, firstParentIndex, secondParentIndex;
    std::set<unsigned int> elites_indices;
    std::chrono::steady_clock::time_point begin, end;
    Chromosome firstParent, secondParent, firstChild, secondChild;

    void init() {
        for (int i = 0; i < POP_MAX; i++) {
            population.push_back(Chromosome(POLY_MAX / 2));
        }

        begin = std::chrono::steady_clock::now();
    }

    long long get_running_duration() {
        return std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
    }

    void trigger_hypermutation() {
        M_RATE = TRIGGERED_HYPERM_RATE;
        std::cout << "triggered hyperm rate!\n";
        stagnated_fitness_generations = 0;
    }

    void display() {
        population[0].draw();

        unsigned int elapsed_time = get_running_duration();
        sprintf(buf, "Generation: %d", gen);
        sprintf(buf2, "Fitness: %.4lf%%", (1.0 - pow(population[0].fitness, -0.5)) * 100.0);
        sprintf(buf3, "Time: %ds", elapsed_time);
        sprintf(buf4, "Polygons: %d", population[0].polygons.size());
        sprintf(buf5, "Population count: %d", population.size());
        sprintf(buf6, "Crossover rate: %.4lf%%", CX_RATE * 100.0);

        float start = 0.93f;
        float diff = 0.065f;
        float xoff = -0.9f;
        drawText(buf, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start);
        drawText(buf2, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - diff);
        drawText(buf3, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - 2 * diff);
        drawText(buf4, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, xoff, start - 3 * diff);
        drawText(buf5, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, 1.0 + xoff, start);
        drawText(buf6, GLUT_BITMAP_HELVETICA_12, 0.9, 0.9, 0.9, 1.0 + xoff, start - diff);
        glutSwapBuffers();

        if (fitness_over_time.size() < elapsed_time) {
            fitness_over_time.push_back(population[0].fitness);
        }
    }

	void idle() {
        end = std::chrono::steady_clock::now();

        // calculate fitness where it is due
        for (auto& chromosome : population) {
            if (chromosome.should_update_fitness) {
                chromosome.calculate_fitness();
            }
        }

        // determine the K fittest individuals
        elites.clear();
        elites_indices.clear();
        biggest = population[0].polygons.size();
        smallest = population[0].polygons.size();
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
        }

        if (stagnated_fitness_generations == 0 || elites[0].fitness == fitness_last_gen) {
            stagnated_fitness_generations++;
        }
        else {
            stagnated_fitness_generations = 0;
        }
        fitness_last_gen = elites[0].fitness;

        if (stagnated_fitness_generations > max_stagnated) {
            max_stagnated = stagnated_fitness_generations;
            std::cout << "new record for stagnated fitness generations: " << max_stagnated << '\n';
        }

        if (stagnated_fitness_generations == 50) {
            trigger_hypermutation();
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
            selection_prob = rnd2(rgen);
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
            crossover_prob.push_back({ i, rnd2(rgen) });

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
        //    unsigned int cutpoint = rnd4(rgen);

        //    // cross the parents over using order, single-cutpoint crossover, then override the parents
        //    std::tie(population[firstParentIndex], population[secondParentIndex]) = firstParent.order_crossover(secondParent, cutpoint);
        //}

        // CUT AND SPLICE CROSSOVER - USE WITH MESSY INDIVIDUALS
        for (unsigned i = 0; i < population.size() - 1 && crossover_prob[i].second < CX_RATE; i += 2)
        {
            // parents chosen for crossover
            firstParentIndex = crossover_prob[i].first;
            secondParentIndex = crossover_prob[i + 1].first;
            firstParent = population[firstParentIndex];
            secondParent = population[secondParentIndex];

            // cross the parents over using order, single-cutpoint crossover
            std::tie(firstChild, secondChild) = firstParent.cut_and_splice(secondParent);
            firstChild.calculate_fitness();
            secondChild.calculate_fitness();

            // sort the parent and the children descendingly after fitness
            afterCrossover.clear();
            afterCrossover.push_back(firstParent);
            afterCrossover.push_back(secondParent);
            afterCrossover.push_back(firstChild);
            afterCrossover.push_back(secondChild);
            std::sort(afterCrossover.begin(), afterCrossover.end(), [](const Chromosome& a, const Chromosome& b) {
                return a.fitness > b.fitness;
            });

            // override the parents with the two best chromosomes
            std::tie(population[firstParentIndex], population[secondParentIndex]) = std::make_tuple(afterCrossover[0], afterCrossover[1]);
        }

        /// overriding the first K individuals with the elites
        for (unsigned i = 0; i < elites.size(); ++i) {
            population[i] = elites[i];
        }

        CX_RATE *= CX_TEMP;
        if (CX_RATE > FINAL_CX_RATE)
            CX_RATE = FINAL_CX_RATE;
        if (M_RATE > 1.0)
            M_RATE = 1.0;

        gen++;

        glutPostRedisplay();
	}
}