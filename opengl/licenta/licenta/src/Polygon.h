#pragma once
#include "Point.h"
#include "Color.h"
#include <vector>
#include <algorithm>

struct Polygon {
	std::vector<Point> vertices;
	Point centroid;
	Color color;

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
};
