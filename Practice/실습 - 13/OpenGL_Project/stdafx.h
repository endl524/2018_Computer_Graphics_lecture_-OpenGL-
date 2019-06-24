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

#define SCALE_VALUE 0.1f

#define OBJECT_TIMER_ELAPSED_TIME 10

#define OBJECT_TYPE_PATH_LINE 0
#define OBJECT_TYPE_MOVING_OBJECT 1

#define OBJECT_ROTATE_VALUE 3.0f
#define OBJECT_AUTO_MOVE_SPEED 10.0f
#define OBJECT_SIZE 100.0f

//#define PATH_SIZE 100.0f

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

