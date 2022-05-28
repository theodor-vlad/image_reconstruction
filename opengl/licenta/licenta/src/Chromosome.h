#pragma once
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
			polygons.push_back(Polygon(POINT_MIN));
		}
	}

	void draw();

	void calculate_fitness();

	void mutate() {

		if (polygons.size() < POLY_MAX && rnd2 < PROB_ADD_POLY) {
			polygons.push_back(Polygon(POINT_MIN));
		}
		if (polygons.size() > POLY_MIN && rnd2 < PROB_REMOVE_POLY) {
			int idx = rnd2 * (polygons.size() - 1);
			polygons.erase(polygons.begin() + idx);
		}

		for (auto& poly : polygons) {

			// replace polygon altogether
			if (rnd2 < PROB_REPLACE_POLY) {
				poly = Polygon(POINT_MIN);
			}

			// add point
			if (poly.vertices.size() <= POINT_MAX && rnd2 < PROB_ADD_POINT) {
				int idx = rnd2 * (poly.vertices.size() - 2);
				Point prev = poly.vertices[idx];
				Point next = poly.vertices[idx + 1];
				Point p = Point(prev.x / 2 + next.x / 2, prev.y / 2 + prev.y / 2);
				poly.vertices.insert(poly.vertices.begin() + idx + 1, p);
			}

			// remove point
			if (poly.vertices.size() > POINT_MIN && rnd2 < PROB_REMOVE_POINT) {
				int idx = rnd2 * (poly.vertices.size() - 1);
				poly.vertices.erase(poly.vertices.begin() + idx);
			}

			// adjust points by small, random amount
			if (rnd2 < PROB_JIGGLE_POINTS) {
				for (auto& vertex : poly.vertices) {
					vertex.x += rnd3;
					vertex.y += rnd3;
					if (vertex.x < -1.0 || vertex.x > 1.0) vertex.x = rnd1;
					if (vertex.y < -1.0 || vertex.y > 1.0) vertex.y = rnd1;
				}
			}

			// adjust hue of polygon
			if (rnd2 < PROB_ADJUST_HUE) {
				poly.color.r += 10 * rnd3;
				poly.color.g += 10 * rnd3;
				poly.color.b += 10 * rnd3;
				poly.color.a += 10 * rnd3;
				if (poly.color.r < 0.0 || poly.color.r > 1.0) poly.color.r = rnd2;
				if (poly.color.g < 0.0 || poly.color.g > 1.0) poly.color.g = rnd2;
				if (poly.color.b < 0.0 || poly.color.b > 1.0) poly.color.b = rnd2;
				if (poly.color.a < 0.15 || poly.color.a > 0.5) poly.color.a = rnd2 * 0.35 + 0.15;
			}

			// translate polygon
			if (rnd2 < PROB_TRANSLATE_POLY) {
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

			// rotate polygon around centroid
			if (rnd2 < PROB_ROTATE_POLY) {
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

			// scale polygon
			if (rnd2 < PROB_SCALE_POLY) {
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