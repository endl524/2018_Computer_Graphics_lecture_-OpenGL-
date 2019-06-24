#pragma once
#include <gl/freeglut.h>
#include <iostream>
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

#define OBJECT_SPEED_MIN -6
#define OBJECT_SPEED_MAX 6
#define OBJECT_AUTO_COLOR_SCALE_VALUE 0.01f
#define OBJECT_AUTO_MOVE_VALUE 3.0f
#define OBJECT_AUTO_SCALE_VALUE 0.1f

#define SPHERE_AUTO_ROTATE_VALUE 3.0f
#define RUNNING_MACHINE_BELT_ROTATE_VALUE 2.0f
#define RUNNING_MACHINE_MAN_ROTATE_VALUE 3.0f
#define BAR_MAN_ROTATE_VALUE 3.0f
#define BENCH_PRESS_SPEED 3.0f

#define OBJECT_SIZE 40.0f
#define FLOOR_PIECE_SIZE 5.0f

#define SPHERE_OBJECT 0 
#define FLOOR_OBJECT 1
#define PIRAMID_OBJECT 2
#define SPOT_LIGHT_1_OBJECT 3
#define SPOT_LIGHT_2_OBJECT 4




struct Color4f
{
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	float A = 1.0f;
};

struct Vertex3D
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	Color4f color;
};

struct Vector2
{
	int x = 0;
	int y = 0;
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

