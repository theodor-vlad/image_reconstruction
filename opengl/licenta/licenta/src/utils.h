#pragma once
#include <cstdlib>
#include <random>
#include <cmath>

#define DIMENSIONS 4U // bgra
#define EPS 1e-22
const double INIT_CX_RATE = 0.01;
const double FINAL_CX_RATE = 0.2;
const double CX_TEMP = pow(FINAL_CX_RATE / INIT_CX_RATE, 1.0 / 10000.0);
double CX_RATE = INIT_CX_RATE;
const double TRIGGERED_HYPERM_RATE = 10.0;
double M_RATE = 1.0;

double max_y = 2.0 / 3.0;

const unsigned int POP_MAX = 50;
const unsigned int K = 5;
const unsigned int POLY_MAX = 300;
const unsigned int POLY_MIN = 50;
const unsigned int POINT_MIN = 3;
const unsigned int POINT_MAX = 10;

const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
std::mt19937_64 rgen(seed);
std::uniform_real_distribution<double> rnd0(-1.0, max_y);
std::uniform_real_distribution<double> rnd1(-1.0, 1.0);
std::uniform_real_distribution<double> rnd2(0.0, 1.0);
std::uniform_real_distribution<double> rnd3(-0.01, 0.01);
std::uniform_int_distribution<int> rnd4(0, POLY_MAX);

//#define rnd0 (((double)rand()/RAND_MAX) * (max_y + 1.0) - 1.0) // [-1.0, max_y]
//#define rnd1 (2.0*(double)rand()/RAND_MAX-1.0) // [-1.0, 1.0]
//#define rnd2 ((double)rand()/RAND_MAX) // [0.0, 1.0]
//#define rnd3 (rnd1/(100*rnd1)) // [-0.01, 0.01]
//#define rnd4 (int(rnd2*POLY_MAX)) // integer from interval [0, POLY_MAX]

// chromosome mutation probabilities
const double PROB_SWAP_TWO_POLYS = 1.0 / 100.0;
const double PROB_REVERSE_SECTION = 1.0 / 100.0;
const double PROB_CUT_AND_REARRANGE_SECTIONS = 1.0 / 100.0;
const double PROB_REPLACE_POLY = 1.0 / 10000000.0;
const double PROB_ADD_POLY = 1.0 / 3.0;
const double PROB_REMOVE_POLY = 1.0 / 15.0;

// polygon mutation probabilities
const double PROB_ADD_POINT = 1.0 / 100.0;
const double PROB_REMOVE_POINT = 1.0 / 100.0;
const double PROB_TRANSLATE_POLY = 1.0 / 100.0;
const double PROB_ROTATE_POLY = 1.0 / 100.0;
const double PROB_SCALE_POLY = 1.0 / 100.0;

// point mutation probabilities
const double PROB_TRANSLATE_POINT_MAX = 1.0 / 1000000.0;
const double PROB_TRANSLATE_POINT_MID = 1.0 / 10000.0;
const double PROB_TRANSLATE_POINT_MIN = 1.0 / 100.0;

// color mutation probabilities
const double PROB_ADJUST_HUE = 1.0 / 100.0;

unsigned int num_of_bgra_values;
unsigned char* targetImgPixels;
unsigned char* currChromoPixels;
unsigned int IMG_WIDTH, IMG_HEIGHT;
std::string path_to_image;

char buf[128], buf2[128], buf3[128], buf4[128], buf5[128], buf6[128];

void drawText(char* str, void* font, GLclampf r, GLclampf g, GLclampf b, GLfloat x, GLfloat y) {
	char* ch;
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glPushAttrib(GL_COLOR_BUFFER_BIT);
	glColor4f(r, g, b, 1);
	glRasterPos3f(x, y, 0.0);
	for (ch = str; *ch; ch++) {
		glutBitmapCharacter(font, (int)*ch);
	}
	glPopAttrib();
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}