#include "Polygon.h"
#include <vector>
#include <functional>
#include <cmath>

struct Chromosome {

	std::vector<Polygon> polygons;
	double fitness = 0.0;
	bool should_update_fitness = true;

	Chromosome() {};

	Chromosome(unsigned int n) {
		for (unsigned int i = 0; i < n; i++) {
			polygons.push_back(Polygon(rnd5));
		}
	}

private:
	void draw();

public:
	void calculate_fitness();

	void mutate() {
		for (auto& poly : polygons) {
			if (rnd2 < M_RATE) { // we will mutate this polygon
				// which mutation should we choose?
				auto which = rnd2;
				if (which < 0.04) {
					// totally replace the polygon with a new one
					poly = Polygon(rnd5);
				}
				else if (which < 0.28) {
					// slightly adjust its vertices...
					for (auto& vertex : poly.vertices) {
						vertex.x += rnd3;
						vertex.y += rnd3;
						if (vertex.x < -1.0 || vertex.x > 1.0) vertex.x = rnd1;
						if (vertex.y < -1.0 || vertex.y > 1.0) vertex.y = rnd1;
					}

					// ...and its color
					poly.color.r += 10 * rnd3;
					poly.color.g += 10 * rnd3;
					poly.color.b += 10 * rnd3;
					if (poly.color.r < 0.0 || poly.color.r > 1.0) poly.color.r = rnd2;
					if (poly.color.g < 0.0 || poly.color.g > 1.0) poly.color.g = rnd2;
					if (poly.color.b < 0.0 || poly.color.b > 1.0) poly.color.b = rnd2;
				}
				else if (which < 0.52) {
					// angle, sin and cos values to rotate by
					float angle = rnd3 * 10.0;
					float s = sin(angle);
					float c = cos(angle);

					auto centroid = poly.centroid;

					// rotate the vertex about its centroid
					for (auto& vertex : poly.vertices) {
						// translate point back to origin
						vertex.x -= centroid.x;
						vertex.y -= centroid.y;

						// rotate point & translate point back
						vertex.x = vertex.x * c - vertex.y * s + centroid.x;
						vertex.y = vertex.x * s + vertex.y * c + centroid.y;

						// correct point if it is out of bounds
						if (vertex.x < -1.0 || vertex.x > 1.0) vertex.x = rnd1;
						if (vertex.y < -1.0 || vertex.y > 1.0) vertex.y = rnd1;
					}
				}
				else if (which < 0.76) {
					// translate
					float tx = rnd3;
					float ty = rnd3;

					for (auto& vertex : poly.vertices) {
						vertex.x += tx;
						vertex.y += ty;

						// correct point if it is out of bounds
						if (vertex.x < -1.0 || vertex.x > 1.0) vertex.x = rnd1;
						if (vertex.y < -1.0 || vertex.y > 1.0) vertex.y = rnd1;
					}
				}
				else {
					// scale
					float factor = rnd2 + 0.5;

					Point centroid_before = poly.centroid;
					for (auto& vertex : poly.vertices) {
						vertex.x *= factor;
						vertex.y *= factor;
					}
					Point centroid_after = poly.centroid;

					float offset_x = centroid_before.x - centroid_after.x;
					float offset_y = centroid_before.y - centroid_after.y;

					for (auto& vertex : poly.vertices) {
						vertex.x += offset_x;
						vertex.y += offset_y;

						// correct point if it is out of bounds
						if (vertex.x < -1.0 || vertex.x > 1.0) vertex.x = rnd1;
						if (vertex.y < -1.0 || vertex.y > 1.0) vertex.y = rnd1;
					}
				}

				// no matter how it changed, this chromosome should be re-evaluated
				should_update_fitness = true;
			}
		}
	}

	std::pair<Chromosome, Chromosome> cut_and_splice(const Chromosome& other) {
		Chromosome child1, child2;
		unsigned int cutpoint1 = rnd2 * this->polygons.size();
		unsigned int cutpoint2 = rnd2 * other.polygons.size();

		for (unsigned int i = 0; i < this->polygons.size(); i++) {
			if (i < cutpoint1) {
				if (child1.polygons.size() < GENE_MAX)
					child1.polygons.push_back(this->polygons[i]);
			}
			else {
				if (child2.polygons.size() < GENE_MAX)
					child2.polygons.push_back(this->polygons[i]);
			}
		}

		for (unsigned int i = 0; i < other.polygons.size(); i++) {
			if (i < cutpoint2) {
				if (child2.polygons.size() < GENE_MAX)
					child2.polygons.push_back(other.polygons[i]);
			}
			else {
				if (child1.polygons.size() < GENE_MAX)
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
			if (rnd2 < 0.5) {
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

	~Chromosome() = default;
};