#pragma once
#include <cstdlib>
#include <random>

GLFWwindow* window;

#define POP_MAX 30
#define GENE_MIN 10
#define GENE_MAX 200
#define CX_RATE_CONST 0.8
#define M_RATE_MIN 0.01
#define M_RATE_MAX 0.1
#define DIMENSIONS 4U // bgra
#define EPS 1e-22
double M_RATE = M_RATE_MAX;
double CX_RATE = CX_RATE_CONST;
double TEMPERATURE = 1.0;

const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937_64 rgen(seed);
std::uniform_real_distribution<double> rnd1(-1.0, 1.0);
std::uniform_real_distribution<double> rnd2(0.0, 1.0);
std::uniform_real_distribution<double> rnd3(-0.01, 0.01);
std::uniform_int_distribution<int> rnd4(0, GENE_MAX);

const int POLY_MAX = 300;
const int POLY_MIN = 50;
const int POINT_MIN = 3;
const int POINT_MAX = 10;

const double PROB_REPLACE_POLY = 0.0000001;
const double PROB_ADD_POLY = 1.0 / 7.5;
const double PROB_REMOVE_POLY = 1.0 / 15.0;

const double PROB_JIGGLE_POINTS = 1.0 / 100.0;
const double PROB_ADJUST_HUE = 1.0 / 100.0;
const double PROB_ADD_POINT = 1.0 / 100.0;
const double PROB_REMOVE_POINT = 1.0 / 100.0;

const double PROB_TRANSLATE_POLY = 1.0 / 100.0;
const double PROB_ROTATE_POLY = 1.0 / 100.0;
const double PROB_SCALE_POLY = 1.0 / 100.0;

const double PROB_TRANSLATE_POINT = 1.0 / 100.0;