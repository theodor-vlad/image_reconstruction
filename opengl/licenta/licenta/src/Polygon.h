#pragma once
#include "Point.h"
#include "Color.h"
#include <vector>
#include <algorithm>

struct Polygon {
	std::vector<Point> vertices;
	Point centroid;
	Color color;

	Polygon() {};

	Polygon(unsigned int n) {
		for (unsigned int i = 0; i < n; i++)
			vertices.push_back(Point());
		calculate_centroid();
		/*std::sort(vertices.begin(), vertices.end(), [](Point a, Point b) {
			return a.x * a.x + a.y * a.y < b.x * b.x + b.y * b.y;
		});*/
	};

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

	friend std::ofstream& operator<<(std::ofstream& out, const Polygon& p) {
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

	friend std::ostream& operator<<(std::ostream& out, const Polygon& p) {
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
