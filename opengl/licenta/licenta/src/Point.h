#pragma once
#include <tuple>
#include "utils.h"

struct Point {
	GLfloat x, y;
	Point() {
		std::tie(x, y) = std::make_tuple(rnd1(rgen), rnd0(rgen));
	}
	Point(float _x, float _y) {
		std::tie(x, y) = std::make_tuple(_x, _y);
	}

	friend std::ofstream& operator<<(std::ofstream& out, const Point& p) {
		out << "\t\t\t{\n";
		out << "\t\t\t\t\"x\": " << p.x << ",\n";
		out << "\t\t\t\t\"y\": " << p.y << "\n\t\t\t}";
		return out;
	}

	friend std::ostream& operator<<(std::ostream& out, const Point& p) {
		out << "\t\t\t{\n";
		out << "\t\t\t\t\"x\": " << p.x << ",\n";
		out << "\t\t\t\t\"y\": " << p.y << "\n\t\t\t}";
		return out;
	}
};