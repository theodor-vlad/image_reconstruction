#pragma once
#include <cstdlib>

#define POP_MAX 100
#define GENE_MIN 10
#define GENE_MAX 300
#define POINT_MIN 3
#define POINT_MAX 4 // change this to (1 + maximum number of desired vertices)
#define CX_RATE_CONST 0.8
#define M_RATE_MIN 0.01
#define M_RATE_MAX 0.1
#define TRIGGERED_HYPER_M_RATE 0.2
double M_RATE = M_RATE_MAX;
double CX_RATE = CX_RATE_CONST;
#define TEMPERATURE 0.99
#define rnd1 (2.0*(double)rand()/RAND_MAX-1.0) // [-1.0, 1.0]
#define rnd2 ((double)rand()/RAND_MAX) // [0.0, 1.0]
#define rnd3 (rnd1/(100*rnd1)) // [-0.01, 0.01]
#define rnd4 (int(rnd2*GENE_MAX)) // integer from interval [0, GENE_MAX]
#define rnd5 (int(POINT_MIN + rnd2 * (POINT_MAX - POINT_MIN))) // integer from interval [POINT_MIN, POINT_MAX - 1]
#define IMG_WIDTH 400U
#define IMG_HEIGHT 300U
#define DIMENSIONS 4U // bgra
#define EPS 1e-22