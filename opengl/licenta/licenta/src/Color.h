#pragma once
#include <GLFW/glfw3.h>
#include <tuple>
#include "utils.h"

struct Color {
	GLfloat r, g, b;
	Color() {
		std::tie(r, g, b) = std::make_tuple(rnd2, rnd2, rnd2);
	}
};