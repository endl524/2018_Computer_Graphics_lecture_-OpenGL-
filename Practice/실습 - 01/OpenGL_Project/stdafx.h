#pragma once
#include <gl/freeglut.h>
#include <vector>
#include <map>
using namespace std;


struct Color4
{
	float R = 0.0f;
	float G = 0.0f;
	float B = 0.0f;
	float A = 0.0f;
};

struct Vertex2D
{
	float point_x = 0.0f;
	float point_y = 0.0f;
	Color4 color;
};

