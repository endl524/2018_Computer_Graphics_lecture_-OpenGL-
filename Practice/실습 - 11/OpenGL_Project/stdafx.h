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

#define OBJECT_TYPE_SIN_CURVE 0
#define OBJECT_TYPE_SPRING 1
#define OBJECT_TYPE_RIBBON 2
#define OBJECT_TYPE_RECTANGLE 3

#define MOVE_UP 0
#define MOVE_DOWN 1
#define MOVE_LEFT 2
#define MOVE_RIGHT 3

#define MOVE_SPEED 8.0f

#define SIZE_SCALE_VALUE 0.1f
#define SIZE_MIN 10.0f
#define SIZE_MAX 300.0f

#define SPRING_RADIUS 40.0f
#define SPRING_ANIMATE_VALUE 1.1f
#define RIBBON_ANIMATE_VALUE 10.0f
#define RECT_SIZE_VALUE 30.0f
#define RECT_COUNT 30

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
	float point_z = 0.0f;
	Color3 color;
};

