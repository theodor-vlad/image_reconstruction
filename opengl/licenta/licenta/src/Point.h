#pragma once
#include <GLFW/glfw3.h>
#include <tuple>
#include "utils.h"

struct Point {
	GLfloat x, y;
	Point() {
		std::tie(x, y) = std::make_tuple(rnd1, rnd1);
	}
	Point(float _x, float _y) {
		std::tie(x, y) = std::make_tuple(_x, _y);
	}
};