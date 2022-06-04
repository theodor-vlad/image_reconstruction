#pragma once
#include "../utils.h"

struct Point {
	GLfloat x, y;
	Point() {
		x = rnd1(rgen);
		y = rnd0(rgen);
	}
	Point(float _x, float _y) {
		x = _x;
		y = _y;
	}

	void mutate() {
		if (rnd2(rgen) < PROB_TRANSLATE_POINT_MAX * M_RATE) {
			x = rnd1(rgen);
			y = rnd0(rgen);
		}

		if (rnd2(rgen) < PROB_TRANSLATE_POINT_MID * M_RATE) {
			x += 10 * rnd3(rgen);
			y += 10 * rnd3(rgen);
			if (x < -1.0 || x > 1.0) x = rnd1(rgen);
			if (y < -1.0 || y > max_y) y = rnd0(rgen);
		}

		if (rnd2(rgen) < PROB_TRANSLATE_POINT_MIN * M_RATE) {
			x += rnd3(rgen);
			y += rnd3(rgen);
			if (x < -1.0 || x > 1.0) x = rnd1(rgen);
			if (y < -1.0 || y > max_y) y = rnd0(rgen);
		}
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