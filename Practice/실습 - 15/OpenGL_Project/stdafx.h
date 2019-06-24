#pragma once
#include <gl/freeglut.h>
#include <vector>
#include <list>
#include <map>
#include <random>
#include <time.h>
using namespace std;

#define INIT_WINDOW_POSITION_X 100
#define INIT_WINDOW_POSITION_Y 100
#define INIT_WINDOW_SIZE_X 800
#define INIT_WINDOW_SIZE_Y 600

#define OBJECT_TIMER_ELAPSED_TIME 10

#define OBJECT_TYPE_CONE 0
#define OBJECT_TYPE_CUBE 1
#define OBJECT_TYPE_SPHERE 2
#define OBJECT_TYPE_TORUS 3
#define OBJECT_TYPE_TEAPOT 4

#define OBJECT_AUTO_COLOR_SCALE_VALUE 0.01f

#define OBJECT_SCALE_VALUE 0.1f
#define OBJECT_SIZE 30.0f
#define OBJECT_ROTATE_VALUE 3.0f

#define NUMBER_INTERVAL_FAR_VALUE 50.0f
#define NUMBER_INTERVAL_CLOSE_VALUE 20.0f

struct Color4f
{
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	float A = 1.0f;
};

struct Vertex3D
{
	float point_x = 0.0f;
	float point_y = 0.0f;
	float point_z = 0.0f;
	Color4f color;
};

