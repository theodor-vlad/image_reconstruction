#pragma once
#include "Polygon.h"
#include <vector>
#include <functional>
#include <cmath>

struct Chromosome {

	std::vector<Poly> polygons;
	double fitness = 0.0;
	bool should_update_fitness = true;

	Chromosome() {};

	Chromosome(unsigned int n) {
		for (unsigned int i = 0; i < n; i++) {
			polygons.push_back(Poly(POINT_MIN));
		}
		calculate_fitness();
	}

	void draw() {
		glClear(GL_COLOR_BUFFER_BIT);

		for (auto& poly : polygons) {
			glBegin(GL_POLYGON);
			glColor4f(poly.color.r, poly.color.g, poly.color.b, poly.color.a); // RGBA
			for (auto& vertex : poly.vertices)
				glVertex2f(vertex.x, vertex.y);
			glEnd();
		}
	}

	void calculate_fitness() {

		// draw the chromosome to the screen
		draw();

		// capture pixels
		glReadPixels(0, 0, IMG_WIDTH, IMG_HEIGHT, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (GLvoid*)currChromoPixels);

		// calculate pixel-by-pixel 3d distance between capture and target image and update fitness
		double fit = 0.0;

		// heavily inspired by Dr. Karoly Zsolnai-Feher's implementation
		// source: https://users.cg.tuwien.ac.at/zsolnai/gfx/mona_lisa_parallel_genetic_algorithm/
		#pragma omp parallel for schedule(guided, 1024) reduction(+:fit)
		for (int i = 0; i < num_of_bgra_values; i += 4) {
			fit += (double(currChromoPixels[i]) - double(targetImgPixels[i])) * (double(currChromoPixels[i]) - double(targetImgPixels[i]));
			fit += (double(currChromoPixels[i + 1]) - double(targetImgPixels[i + 1])) * (double(currChromoPixels[i + 1]) - double(targetImgPixels[i + 1]));
			fit += (double(currChromoPixels[i + 2]) - double(targetImgPixels[i + 2])) * (double(currChromoPixels[i + 2]) - double(targetImgPixels[i + 2]));
		}
		fitness = pow(fit / double(255.0 * 255.0 * 3 * IMG_WIDTH * IMG_HEIGHT), -2);

		should_update_fitness = false;
	}

	void mutate() {

		if (polygons.size() < POLY_MAX && rnd2(rgen) < PROB_ADD_POLY * M_RATE) {
			polygons.push_back(Poly(POINT_MIN));
		}
		if (polygons.size() > POLY_MIN && rnd2(rgen) < PROB_REMOVE_POLY * M_RATE) {
			int idx = rnd2(rgen) * (polygons.size() - 1);
			polygons.erase(polygons.begin() + idx);
		}

		if (rnd2(rgen) < PROB_SWAP_TWO_POLYS * M_RATE) {
			int idx1 = rnd2(rgen) * (polygons.size() - 1);
			int idx2 = rnd2(rgen) * (polygons.size() - 1);
			std::swap(polygons[idx1], polygons[idx2]);
		}

		for (auto& poly : polygons) {
			if (rnd2(rgen) < PROB_REPLACE_POLY * M_RATE) {
				poly = Poly(POINT_MIN);
			}
			poly.mutate();
		}
		should_update_fitness = true;
	}

	std::pair<Chromosome, Chromosome> cut_and_splice(const Chromosome& other) {
		Chromosome child1, child2;
		unsigned int cutpoint1 = rnd2(rgen) * this->polygons.size();
		unsigned int cutpoint2 = rnd2(rgen) * other.polygons.size();

		for (unsigned int i = 0; i < this->polygons.size(); i++) {
			if (i < cutpoint1) {
				if (child1.polygons.size() < POLY_MAX)
					child1.polygons.push_back(this->polygons[i]);
			}
			else {
				if (child2.polygons.size() < POLY_MAX)
					child2.polygons.push_back(this->polygons[i]);
			}
		}

		for (unsigned int i = 0; i < other.polygons.size(); i++) {
			if (i < cutpoint2) {
				if (child2.polygons.size() < POLY_MAX)
					child2.polygons.push_back(other.polygons[i]);
			}
			else {
				if (child1.polygons.size() < POLY_MAX)
					child1.polygons.push_back(other.polygons[i]);
			}
		}

		return { child1, child2 };
	}

	std::pair<Chromosome, Chromosome> order_crossover(const Chromosome& other, unsigned int cutpoint) {
		Chromosome child1, child2;
		for (unsigned int i = 0; i < polygons.size(); i++) {
			if (i < cutpoint) {
				child1.polygons.push_back(polygons[i]);
				child2.polygons.push_back(other.polygons[i]);
			}
			else {
				child1.polygons.push_back(other.polygons[i]);
				child2.polygons.push_back(polygons[i]);
			}
		}

		return { child1, child2 };
	}

	std::pair<Chromosome, Chromosome> random_crossover(const Chromosome& other) {
		Chromosome child1, child2;
		for (unsigned int i = 0; i < polygons.size(); i++) {
			if (rnd2(rgen) < 0.5) {
				child1.polygons.push_back(polygons[i]);
				child2.polygons.push_back(other.polygons[i]);
			}
			else {
				child1.polygons.push_back(other.polygons[i]);
				child2.polygons.push_back(polygons[i]);
			}
		}

		return { child1, child2 };
	}

	friend std::ofstream& operator<<(std::ofstream& out, const Chromosome& c) {
		out << "[\n";
		for (int i = 0; i < c.polygons.size(); i++) {
			out << c.polygons[i];
			if (i != c.polygons.size() - 1) {
				out << ",\n";
			}
		}
		out << "\n]";
		return out;
	}

	friend std::ostream& operator<<(std::ostream& out, const Chromosome& c) {
		out << "[\n";
		for (int i = 0; i < c.polygons.size(); i++) {
			out << c.polygons[i];
			if (i != c.polygons.size() - 1) {
				out << ",\n";
			}
		}
		out << "\n]";
		return out;
	}

	~Chromosome() = default;
};