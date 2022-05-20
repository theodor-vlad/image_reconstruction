#pragma once
#include <GLFW/glfw3.h>
#include <tuple>
#include "utils.h"

struct Color {
	GLfloat r, g, b, a;
	Color() {
		std::tie(r, g, b) = std::make_tuple(rnd2, rnd2, rnd2);
		a = rnd2 * 0.35 + 0.15;
	}
};