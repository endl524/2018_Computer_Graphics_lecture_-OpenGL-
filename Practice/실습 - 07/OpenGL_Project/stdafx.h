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

#define OBJECT_TIMER_ELAPSED_TIME_MS 10

#define COLOR_SCALE_VALUE_R 0.05f
#define COLOR_SCALE_VALUE_G 0.05f
#define COLOR_SCALE_VALUE_B 0.05f
#define RADIUS_MAX_VALUE 50.0f
#define THROUDH_EDGE_RADIUS_MAX_VALUE 400.0f

struct Color3
{
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	//float A = 0.0f;
};

struct Direction2D
{
	float x = 0.0f;
	float y = 0.0f;
};

struct Vertex2D
{
	float point_x = 0.0f;
	float point_y = 0.0f;
	Color3 color;
	Direction2D dir;
};

