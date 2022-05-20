
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

std::ifstream fin("F:\\licenta\\opengl\\licenta\\licenta\\pixels.txt");

//---------------------------------------- GLOBALS --------------------------------------------------
GLFWwindow* window;
unsigned int num_of_bgra_values, IMG_WIDTH, IMG_HEIGHT;
unsigned char* targetImgPixels;
unsigned char* currChromoPixels;

//--------------------------------------- FUNCTIONS -------------------------------------------------
void Chromosome::draw() {
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& poly : polygons) {
        glBegin(GL_POLYGON);
        glColor4f(poly.color.r, poly.color.g, poly.color.b, 0.2f); // RGBA
        for (auto& vertex : poly.vertices)
            glVertex2f(vertex.x, vertex.y);
        glEnd();
    }
}

//void calculate_fitness_parallel(int start) {
//    double fit = 0.0;
//    int end = start + thread_chunk_size * DIMENSIONS;
//    double diff;
//    for (int i = start; i < end; i++) {
//        fit += (double(currChromoPixels[0]) - double(targetImgPixels[0])) * (double(currChromoPixels[0]) - double(targetImgPixels[0]));
//    }
//    partial_fitnesses.push_back(pow(fit, -2));
//}

void Chromosome::calculate_fitness() {

    // draw the chromosome to the screen
    draw();

    // capture pixels
    glReadPixels(0, 0, IMG_WIDTH, IMG_HEIGHT, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)currChromoPixels);

    // calculate pixel-by-pixel 3d distance between capture and target image and update fitness
    double fit = 0.0;
    #pragma omp parallel for schedule(guided, 1024) reduction(+:fit)
    for (int i = 0; i < num_of_bgra_values; i += 4) {
        fit += (double(currChromoPixels[i]) - double(targetImgPixels[i])) * (double(currChromoPixels[i]) - double(targetImgPixels[i]));
        fit += (double(currChromoPixels[i + 1]) - double(targetImgPixels[i + 1])) * (double(currChromoPixels[i + 1]) - double(targetImgPixels[i + 1]));
        fit += (double(currChromoPixels[i + 2]) - double(targetImgPixels[i + 2])) * (double(currChromoPixels[i + 2]) - double(targetImgPixels[i + 2]));
    }
    fitness = pow(fit, -2);

    should_update_fitness = false;
}

bool almost_equals(double a, double b) {
    return fabs(a - b) <= EPS;
}
//------------------------------------------ MAIN ----------------------------------------------------
int main()
{
    srand(time(NULL));

    int counter_target_img_pixels = 0;
    int r, g, b;
    fin >> IMG_WIDTH >> IMG_HEIGHT;
    num_of_bgra_values = IMG_WIDTH * IMG_HEIGHT * DIMENSIONS;
    targetImgPixels  = new unsigned char[num_of_bgra_values];
    currChromoPixels = new unsigned char[num_of_bgra_values];

    for (int i = 0; i < IMG_WIDTH * IMG_HEIGHT; i++) {
        fin >> r >> g >> b;
        targetImgPixels[counter_target_img_pixels++] = b;
        targetImgPixels[counter_target_img_pixels++] = g;
        targetImgPixels[counter_target_img_pixels++] = r;
        targetImgPixels[counter_target_img_pixels++] = 0; // alpha, redundant
    }

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(IMG_WIDTH, IMG_HEIGHT, "Image reconstruction", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    //glfwSwapInterval(1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //// GA
    //std::vector<Chromosome> population, newPopulation, elites;
    //for (int i = 0; i < POP_MAX; i++) {
    //    population.push_back(Chromosome(POLY_MAX / 2));
    //}

    //const unsigned int K = 5;
    //double total_prob, selection_prob;
    //std::vector<double> individual_prob, accumulated_prob;
    //std::vector<std::pair<unsigned int, double>> crossover_prob;

    //unsigned int gen_no = 0, local_minimum_count = 0;
    //std::set<unsigned int> elites_indices;

    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //while (!glfwWindowShouldClose(window))
    //{
    //    // calculate fitness where it is due
    //    for (auto& chromosome : population) {
    //        if (chromosome.should_update_fitness) {
    //            chromosome.calculate_fitness();
    //        }
    //    }

    //    // determine the K fittest individuals
    //    elites.clear();
    //    elites_indices.clear();
    //    unsigned int biggest = population[0].polygons.size();
    //    unsigned int smallest = population[0].polygons.size();
    //    while (elites.size() < K)
    //    {
    //        unsigned best_idx = 0;
    //        double best_f = population[0].fitness;
    //        for (unsigned i = 1; i < population.size(); ++i) {
    //            biggest = population[i].polygons.size() > biggest ? population[i].polygons.size() : biggest;
    //            smallest = population[i].polygons.size() < smallest ? population[i].polygons.size() : smallest;
    //            if (elites_indices.find(i) == elites_indices.end() && population[i].fitness > best_f)
    //            {
    //                best_f = population[i].fitness;
    //                best_idx = i;
    //            }
    //        }

    //        elites_indices.insert(best_idx);
    //        elites.push_back(population[best_idx]);

    //        if (elites.size() == 1) {
    //            if (gen_no % 100 == 0) {
    //                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //                std::cout << "generation " << gen_no << ": " << best_f << ", mutation rate: " << M_RATE;
    //                std::cout << ", smallest: " << smallest << ", biggest: " << biggest;
    //                std::cout << " (time elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s)" << '\n';
    //            }
    //        }
    //    }

    //    // roulette wheel selection
    //    total_prob = 0;
    //    for (int i = 0; i < population.size(); ++i)
    //        total_prob += population[i].fitness;

    //    individual_prob.clear();
    //    for (unsigned i = 0; i < population.size(); ++i)
    //        individual_prob.push_back(population[i].fitness / total_prob);

    //    accumulated_prob.clear();
    //    accumulated_prob.push_back(0);
    //    for (unsigned i = 0; i < population.size(); ++i)
    //        accumulated_prob.push_back(accumulated_prob[i] + individual_prob[i]);
    //    accumulated_prob[accumulated_prob.size() - 1] = 1.0;

    //    for (unsigned i = 0; i < population.size(); ++i)
    //    {
    //        selection_prob = rnd2;
    //        for (unsigned j = 0; j < population.size(); ++j)
    //            if (accumulated_prob[j] < selection_prob && selection_prob <= accumulated_prob[j + 1])
    //            {
    //                newPopulation.push_back(population[j]);
    //                break;
    //            }
    //    }

    //    population = newPopulation;
    //    newPopulation.clear();

    //    // mutate
    //    for (auto& chromosome : population) {
    //        chromosome.mutate();
    //    }

    //    // crossover: generate probabilites for each individual
    //    crossover_prob.clear();
    //    for (unsigned i = 0; i < population.size(); ++i)
    //        crossover_prob.push_back({ i, rnd2 });

    //    // sort the individuals in the ascending order of the generated probabilites
    //    std::sort(crossover_prob.begin(), crossover_prob.end(), [](std::pair<unsigned, double> p1, std::pair<unsigned, double> p2) {
    //        return p1.second < p2.second;
    //        });

    //    //// ORDER CROSSOVER - USE WITH FIXED SIZE INDIVIDUALS
    //    //for (unsigned i = 0; i < population.size() - 1 && crossover_prob[i].second < CX_RATE; i += 2)
    //    //{
    //    //    // parents chosen for crossover
    //    //    unsigned firstParentIndex = crossover_prob[i].first;
    //    //    unsigned secondParentIndex = crossover_prob[i + 1].first;
    //    //    auto firstParent = population[firstParentIndex];
    //    //    auto secondParent = population[secondParentIndex];

    //    //    // determine the cutpoint
    //    //    unsigned int cutpoint = rnd4;

    //    //    // cross the parents over using order, single-cutpoint crossover, then override the parents
    //    //    std::tie(population[firstParentIndex], population[secondParentIndex]) = firstParent.order_crossover(secondParent, cutpoint);
    //    //}

    //    // CUT AND SPLICE CROSSOVER - USE WITH MESSY INDIVIDUALS
    //    for (unsigned i = 0; i < population.size() - 1 && crossover_prob[i].second < CX_RATE; i += 2)
    //    {
    //        // parents chosen for crossover
    //        unsigned firstParentIndex = crossover_prob[i].first;
    //        unsigned secondParentIndex = crossover_prob[i + 1].first;
    //        auto firstParent = population[firstParentIndex];
    //        auto secondParent = population[secondParentIndex];

    //        // cross the parents over using order, single-cutpoint crossover, then override the parents
    //        std::tie(population[firstParentIndex], population[secondParentIndex]) = firstParent.cut_and_splice(secondParent);
    //    }

    //    /// overriding the first K individuals with the elites
    //    for (unsigned i = 0; i < elites.size(); ++i) {
    //        population[i] = elites[i];
    //    }

    //    gen_no++;

    //    // display the best individual
    //    population[0].draw();
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //}

    //// HC
    //Chromosome curr(POLY_MAX / 2);
    //curr.calculate_fitness();
    //int gen = 0;
    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //while (!glfwWindowShouldClose(window)) {

    //    if (gen % 1000 == 0) {
    //        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //        std::cout << "gen " << gen << ": " << curr.fitness << " (" << curr.polygons.size() << " polygons)";
    //        std::cout << " (time elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s)" << '\n';
    //    }
    //    gen++;

    //    Chromosome next = curr;
    //    next.mutate();
    //    next.calculate_fitness();

    //    if (next.fitness > curr.fitness) {
    //        curr = next;
    //    }

    //    curr.draw();
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();
    //}

    //// SA
    //Chromosome curr(POLY_MAX / 2);
    //curr.calculate_fitness();
    //int gen = 0;
    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    //while (!glfwWindowShouldClose(window)) {

    //    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //    std::cout << "gen " << gen << ": " << curr.fitness << " (" << curr.polygons.size() << " polygons)";
    //    std::cout << " (time elapsed: " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "s)";
    //    std::cout << " (temp: " << TEMPERATURE << ")\n";
    //    

    //    for (int i = 0; i < 1000; i++) {
    //        Chromosome next = curr;
    //        next.mutate();
    //        next.calculate_fitness();

    //        if (next.fitness > curr.fitness || rnd2 < exp(-abs(next.fitness - curr.fitness) / TEMPERATURE)) {
    //            curr = next;
    //        }

    //        curr.draw();
    //        glfwSwapBuffers(window);
    //        glfwPollEvents();
    //    }

    //    TEMPERATURE *= 0.5;
    //    gen++;
    //}

    glfwTerminate();
    delete[] currChromoPixels;
    return 0;
}