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

#define CAMERA_MOVE_VALUE 5.0f

#define OBJECT_TIMER_ELAPSED_TIME 10

#define OBJECT_TYPE_SPHERE 0
#define OBJECT_TYPE_CUBE 1
#define OBJECT_TYPE_CONE 2
#define OBJECT_TYPE_TEAPOT 3
#define OBJECT_TYPE_RAIL 4


#define OBJECT_SPEED_MIN 4
#define OBJECT_SPEED_MAX 10
#define OBJECT_AUTO_COLOR_SCALE_VALUE 0.01f
#define OBJECT_AUTO_MOVE_VALUE 1.0f

#define OBJECT_SCALE_VALUE 0.1f
#define OBJECT_SIZE 40.0f
#define OBJECT_ROTATE_VALUE 3.0f

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

struct Vector3
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
};

struct Vector4
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 1.0f;
};

