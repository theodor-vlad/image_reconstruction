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