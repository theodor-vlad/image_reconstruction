#pragma once
#include <cstdlib>

#define POP_MAX 30
#define GENE_MIN 10
#define GENE_MAX 200
#define CX_RATE_CONST 0.95
#define M_RATE_MIN 0.05
#define M_RATE_MAX 0.1
#define TRIGGERED_HYPER_M_RATE 0.2
double M_RATE = M_RATE_MAX;
double CX_RATE = CX_RATE_CONST;
double TEMPERATURE = 1.0;
#define rnd1 (2.0*(double)rand()/RAND_MAX-1.0) // [-1.0, 1.0]
#define rnd2 ((double)rand()/RAND_MAX) // [0.0, 1.0]
#define rnd3 (rnd1/(100*rnd1)) // [-0.01, 0.01]
#define rnd4 (int(rnd2*GENE_MAX)) // integer from interval [0, GENE_MAX]
#define DIMENSIONS 4U // bgra
#define EPS 1e-22

const int POLY_MAX = 300;
const int POLY_MIN = 100;
const int POINT_MIN = 3;
const int POINT_MAX = 10;

const double PROB_REPLACE_POLY = 0.0000001;
const double PROB_ADD_POLY = 1.0 / 3.0;
const double PROB_REMOVE_POLY = 1.0 / 15.0;

const double PROB_JIGGLE_POINTS = 1.0 / 100.0;
const double PROB_ADJUST_HUE = 1.0 / 100.0;
const double PROB_ADD_POINT = 1.0 / 100.0;
const double PROB_REMOVE_POINT = 1.0 / 100.0;

const double PROB_TRANSLATE_POLY = 1.0 / 100.0;
const double PROB_ROTATE_POLY = 1.0 / 100.0;
const double PROB_SCALE_POLY = 1.0 / 100.0;

const double PROB_TRANSLATE_POINT = 1.0 / 100.0;