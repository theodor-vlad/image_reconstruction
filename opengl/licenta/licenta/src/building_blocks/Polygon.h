#pragma once
#include "Point.h"
#include "Color.h"
#include <vector>
#include <algorithm>

struct Poly {
	std::vector<Point> vertices;
	Point centroid;
	Color color;

	Poly() {};
	Poly(unsigned int n) {
		for (unsigned int i = 0; i < n; i++)
			vertices.push_back(Point());
		calculate_centroid();
	}

private:
	void calculate_centroid() {
		Point centroid(0, 0);
		for (auto& vertex : vertices) {
			centroid.x += vertex.x;
			centroid.y += vertex.y;
		}
		centroid.x /= vertices.size();
		centroid.y /= vertices.size();
		this->centroid = centroid;
	}

public:
	void mutate() {
		
		// add point
		if (vertices.size() <= POINT_MAX && rnd2(rgen) < PROB_ADD_POINT * M_RATE) {
			int idx = rnd2(rgen) * (vertices.size() - 2);
			Point prev = vertices[idx];
			Point next = vertices[idx + 1];
			Point p = Point(prev.x / 2 + next.x / 2, prev.y / 2 + prev.y / 2);
			vertices.insert(vertices.begin() + idx + 1, p);
			calculate_centroid();
		}

		// remove point
		if (vertices.size() > POINT_MIN && rnd2(rgen) < PROB_REMOVE_POINT * M_RATE) {
			int idx = rnd2(rgen) * (vertices.size() - 1);
			vertices.erase(vertices.begin() + idx);
			calculate_centroid();
		}

		// translate polygon
		if (rnd2(rgen) < PROB_TRANSLATE_POLY * M_RATE) {
			float tx = rnd3(rgen);
			float ty = rnd3(rgen);

			for (auto& vertex : vertices) {
				vertex.x += tx;
				vertex.y += ty;

				// correct point if it is out of bounds
				if (vertex.x < -1.0 || vertex.x > 1.0) vertex.x = rnd1(rgen);
				if (vertex.y < -1.0 || vertex.y > max_y) vertex.y = rnd0(rgen);
			}
		}

		// rotate polygon around centroid
		// source: https://stackoverflow.com/questions/2259476/rotating-a-point-about-another-point-2d
		if (rnd2(rgen) < PROB_ROTATE_POLY * M_RATE) {
			float angle = rnd3(rgen) * 50.0;
			float s = sin(angle);
			float c = cos(angle);

			// rotate the vertex about its centroid
			for (auto& vertex : vertices) {
				// translate point back to origin
				vertex.x -= centroid.x;
				vertex.y -= centroid.y;

				// rotate point & translate point back
				vertex.x = vertex.x * c - vertex.y * s + centroid.x;
				vertex.y = vertex.x * s + vertex.y * c + centroid.y;

				// correct point if it is out of bounds
				if (vertex.x < -1.0 || vertex.x > 1.0) vertex.x = rnd1(rgen);
				if (vertex.y < -1.0 || vertex.y > max_y) vertex.y = rnd0(rgen);
			}
		}

		// scale polygon
		if (rnd2(rgen) < PROB_SCALE_POLY * M_RATE) {
			float factor = rnd2(rgen) + 0.5;

			// translate polygon so that the centroid sits in the origin
			Point centroid_before = centroid;
			for (auto& vertex : vertices) {
				vertex.x -= centroid_before.x;
				vertex.y -= centroid_before.y;
			}

			// scale polygon and translate back
			for (auto& vertex : vertices) {
				vertex.x = vertex.x * factor + centroid_before.x;
				vertex.y = vertex.y * factor + centroid_before.y;

				// correct point if it is out of bounds
				if (vertex.x < -1.0 || vertex.x > 1.0) vertex.x = rnd1(rgen);
				if (vertex.y < -1.0 || vertex.y > max_y) vertex.y = rnd0(rgen);
			}

			// recalculate centroid (should be the same but for good measure)
			calculate_centroid();
		}

		for (auto& vertex : vertices) vertex.mutate();
		color.mutate();
	}

	friend std::ofstream& operator<<(std::ofstream& out, const Poly& p) {
		out << "\t{\n";
		out << "\t\t\"vertices\": [\n";
		for (int i = 0; i < p.vertices.size(); i++) {
			out << p.vertices[i];
			if (i != p.vertices.size() - 1) {
				out << ",\n";
			}
		}
		out << "\n\t\t],\n";
		out << "\t\t\"color\": ";
		out << p.color << "\n\t}";
		return out;
	}

	friend std::ostream& operator<<(std::ostream& out, const Poly& p) {
		out << "\t{\n";
		out << "\t\t\"vertices\": [\n";
		for (int i = 0; i < p.vertices.size(); i++) {
			out << p.vertices[i];
			if (i != p.vertices.size() - 1) {
				out << ",\n";
			}
		}
		out << "\n\t\t],\n";
		out << "\t\t\"color\": ";
		out << p.color << "\n\t}";
		return out;
	}
};
