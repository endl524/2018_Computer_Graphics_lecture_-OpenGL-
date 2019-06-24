#pragma once
#include <gl/freeglut.h>
#include <vector>
#include <list>
#include <map>
#include <random>
using namespace std;

#define INIT_WINDOW_POSITION_X 100
#define INIT_WINDOW_POSITION_Y 100
#define INIT_WINDOW_SIZE_X 800
#define INIT_WINDOW_SIZE_Y 600

#define COLOR_SCALE_VALUE_R 0.02f
#define COLOR_SCALE_VALUE_G 0.02f
#define COLOR_SCALE_VALUE_B 0.02f
#define SCALE_VALUE 0.8f

#define OBJECT_TIMER_ELAPSED_TIME 10

#define OBJECT_ROTATE_VALUE 3.0f
#define OBJECT_AUTO_ROTATE_SPEED 6.0f
#define OBJECT_AUTO_MOVE_SPEED 10.0f

#define OBJECT_SIZE 15.0f
#define OBJECT_TYPE_PATH_LINE 0
#define OBJECT_TYPE_TRIANGLE 1
#define OBJECT_TYPE_RECTANGLE 2

#define PATH_RADIUS 100.0f
#define PATH_TYPE_CIRCLE 0
#define PATH_TYPE_SIN_CURVE 1
#define PATH_TYPE_TORNADO_SIDE 2
#define PATH_TYPE_ZIGZAG 3
#define PATH_TYPE_CUSTOM 4
#define PATH_TYPE_TORNADO_TOP 5

#define ZIGZAG_WIDTH 40.0f
#define TORNADO_SIDE_X_RELATIVE_VALUE 6.0f
#define TORNADO_SIDE_Y_RELATIVE_VALUE 1.5f
#define TORNADO_TOP_RADIUS_RELATIVE_VALUE 1.0f

struct Color3
{
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	//float A = 0.0f;
};

struct Vertex3D
{
	float point_x = 0.0f;
	float point_y = 0.0f;
	float point_z = 0.0f;
	Color3 color;
};

