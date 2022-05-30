#pragma once
#include <cstdlib>
#include <random>

#define DIMENSIONS 4U // bgra
#define EPS 1e-22
double CX_RATE = 0.25;
double max_y = 2.0 / 3.0;

const int POP_MAX = 50;
const int POLY_MAX = 300;
const int POLY_MIN = 50;
const int POINT_MIN = 3;
const int POINT_MAX = 10;

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