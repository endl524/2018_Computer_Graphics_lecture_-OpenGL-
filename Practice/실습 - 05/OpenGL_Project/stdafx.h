#pragma once
#include <gl/freeglut.h>
#include <vector>
#include <map>
#include <random>
using namespace std;

#define INIT_WINDOW_POSITION_X 100
#define INIT_WINDOW_POSITION_Y 100
#define INIT_WINDOW_SIZE_X 800
#define INIT_WINDOW_SIZE_Y 600

#define RECT_SIZE_MIN_X 30
#define RECT_SIZE_MAX_X 100
#define RECT_SIZE_MIN_Y 30
#define RECT_SIZE_MAX_Y 100

#define RANDOM_DIR_MIN_X -1.0f
#define RANDOM_DIR_MAX_X 1.0f
#define RANDOM_DIR_MIN_Y -1.0f
#define RANDOM_DIR_MAX_Y 1.0f
#define RANDOM_SPEED_MIN 5.0f
#define RANDOM_SPEED_MAX 10.0f

#define MAIN_SPEED_RELATIVE_VALUE 0.1f

#define COLOR_SCALE_VALUE_R 0.05f
#define COLOR_SCALE_VALUE_G 0.05f
#define COLOR_SCALE_VALUE_B 0.05f
#define SCALE_VALUE_X 5
#define SCALE_VALUE_Y 5

#define SHAPE_TYPE_TRIANGLE 0
#define SHAPE_TYPE_RECTANGLE 1

struct Color3
{
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	//float A = 0.0f;
};

struct Vertex2D
{
	float point_x = 0.0f;
	float point_y = 0.0f;
	Color3 color;
};

