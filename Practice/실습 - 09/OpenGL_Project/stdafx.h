#pragma once
#include <gl/freeglut.h>
#include <vector>
#include <map>
#include <random>
using namespace std;

#define INIT_WINDOW_POSITION_X 100.0f
#define INIT_WINDOW_POSITION_Y 100.0f
#define INIT_WINDOW_SIZE_X 800.0f
#define INIT_WINDOW_SIZE_Y 600.0f

#define FIXED_RECT_SIZE_X 20.0f
#define FIXED_RECT_SIZE_Y 20.0f

#define ERASER_SIZE_X FIXED_RECT_SIZE_X * 2.0f
#define ERASER_SIZE_Y FIXED_RECT_SIZE_Y * 2.0f

#define RANDOM_POSITION_MIN_X 20.0f
#define RANDOM_POSITION_MAX_X 780.0f
#define RANDOM_POSITION_MIN_Y 20.0f
#define RANDOM_POSITION_MAX_Y 580.0f

#define COLOR_SCALE_VALUE_R 0.05f
#define COLOR_SCALE_VALUE_G 0.05f
#define COLOR_SCALE_VALUE_B 0.05f

#define OBJECT_TIMER_ELAPSED_TIME 10

#define OBJECTS_COUNT 100

#define OBJECT_TYPE_NORMAL_RECT 0
#define OBJECT_TYPE_ERASER 1

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

