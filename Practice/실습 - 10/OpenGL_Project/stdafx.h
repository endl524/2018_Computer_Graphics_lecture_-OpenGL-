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

#define ERASER_SIZE_X 40
#define ERASER_SIZE_Y 40

#define RANDOM_POSITION_MIN_X 20.0f
#define RANDOM_POSITION_MAX_X 780.0f
#define RANDOM_POSITION_MIN_Y 20.0f
#define RANDOM_POSITION_MAX_Y 580.0f
#define RANDOM_SIZE_MIN_X 20.0f
#define RANDOM_SIZE_MAX_X 60.0f
#define RANDOM_SIZE_MIN_Y 20.0f
#define RANDOM_SIZE_MAX_Y 60.0f
#define RANDOM_DIR_MIN_X -1.0f
#define RANDOM_DIR_MAX_X 1.0f
#define RANDOM_DIR_MIN_Y -1.0f
#define RANDOM_DIR_MAX_Y 1.0f
#define RANDOM_SPEED_MIN 0.1f
#define RANDOM_SPEED_MAX 1.0f

#define MAIN_SPEED_RELATIVE_VALUE 0.1f

#define MOVE_TYPE_BOUNDING 0
#define MOVE_TYPE_WALL_RIDING 1

#define WALL_RIDING_CLOCKWISE 0
#define WALL_RIDING_ANTICLOCKWISE 1

#define COLOR_SCALE_VALUE_R 0.02f
#define COLOR_SCALE_VALUE_G 0.02f
#define COLOR_SCALE_VALUE_B 0.02f
#define SCALE_VALUE 0.8f
#define TRANSFORM_SCALE_VALUE 0.5f
#define TRANSFORM_SPEED 0.01f

#define ELAPSED_TIME_VALUE 0.01f
#define SIZE_SCALE_WAIT_TIME 3.0f
#define TRANSFORM_RECT_WAIT_TIME 1.0f

#define OBJECTS_COUNT 30

#define OBJECT_TIMER_ELAPSED_TIME 10

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

