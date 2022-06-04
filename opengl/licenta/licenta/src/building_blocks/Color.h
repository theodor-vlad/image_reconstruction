#pragma once
#include <tuple>
#include "../utils.h"

struct Color {
	GLfloat r, g, b, a;
	Color() {
		std::tie(r, g, b) = std::make_tuple(rnd2(rgen), rnd2(rgen), rnd2(rgen));
		a = rnd2(rgen) * 0.35 + 0.15;
	}

	Color(double _r, double _g, double _b, double _a) {
		std::tie(r, g, b, a) = std::make_tuple(_r, _g, _b, _a);
	}

	void mutate() {
		if (rnd2(rgen) < PROB_ADJUST_HUE * M_RATE) {
			r += 10 * rnd3(rgen);
			g += 10 * rnd3(rgen);
			b += 10 * rnd3(rgen);
			a += 10 * rnd3(rgen);
			if (r < 0.0 || r > 1.0) r = rnd2(rgen);
			if (g < 0.0 || g > 1.0) g = rnd2(rgen);
			if (b < 0.0 || b > 1.0) b = rnd2(rgen);
			if (a < 0.15 || a > 0.5) a = rnd2(rgen) * 0.35 + 0.15;
		}
	}

	friend std::ofstream& operator<<(std::ofstream& out, const Color& c) {
		out << "{\n";
		out << "\t\t\t\"r\": " << c.r << ",\n";
		out << "\t\t\t\"g\": " << c.g << ",\n";
		out << "\t\t\t\"b\": " << c.b << ",\n";
		out << "\t\t\t\"a\": " << c.a << "\n\t\t}";
		return out;
	}

	friend std::ostream& operator<<(std::ostream& out, const Color& c) {
		out << "{\n";
		out << "\t\t\t\"r\": " << c.r << ",\n";
		out << "\t\t\t\"g\": " << c.g << ",\n";
		out << "\t\t\t\"b\": " << c.b << ",\n";
		out << "\t\t\t\"a\": " << c.a << "\n\t\t}";
		return out;
	}
};