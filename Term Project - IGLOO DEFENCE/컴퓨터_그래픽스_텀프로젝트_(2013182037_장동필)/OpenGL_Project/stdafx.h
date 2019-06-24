#pragma once
#include <gl/freeglut.h>
#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <random>
#include <time.h>
using namespace std;


// =================================
// Window data
#define INIT_WINDOW_POSITION_X 100
#define INIT_WINDOW_POSITION_Y 100
#define INIT_WINDOW_SIZE_X 800
#define INIT_WINDOW_SIZE_Y 600


// =================================
// Timer
#define OBJECT_TIMER_ELAPSED_TIME 10
#define BULLET_RELOAD_TIMER_ELAPSED_TIME 500


// =================================
// Random value
#define OBJECT_SPEED_MIN -6
#define OBJECT_SPEED_MAX 6
#define OBJECT_AUTO_COLOR_SCALE_VALUE 0.01f
#define OBJECT_AUTO_MOVE_VALUE 3.0f
#define OBJECT_AUTO_SCALE_VALUE 0.1f


// =================================
// Object data
#define OBJECT_SIZE 40.0f
#define FLOOR_PIECE_WIDTH_SIZE 32
#define FLOOR_PIECE_HEIGHT_SIZE 32
#define FLOOR_WIDTH 64
#define FLOOR_HEIGHT 64
#define DASH_MODE_SPEED_RELATIVE_VALUE 2.0f
#define PLAYER_MAX_HP 3


// =================================
// Object type
#define FLOOR_OBJECT 0
#define ROBOT_OBJECT 1
#define DIRECTIONAL_LIGHT_OBJECT 2
#define ROCK_OBSTACLE 3
#define TREE_OBSTACLE 4
#define IGLOO_OBJECT 5
#define BULLET_OBJECT 6
#define FENCE_OBJECT 7
#define SNOW_OBJECT 8
#define SNOW_BALL_OBJECT 9
#define SMALL_ENEMY_1 10
#define SMALL_ENEMY_2 11



// =================================
// Particle data
#define WP_INIT_POSITION_Y 400.0f
#define WP_FALLING_SPEED_MIN 1.0f
#define WP_FALLING_SPEED_MAX 3.0f
#define PARTICLE_SPEED_MIN 1.5f
#define PARTICLE_SPEED_MAX 4.0f
#define PARTICLE_DIR_MIN 0.1f
#define PARTICLE_DIR_MAX 1.0f


// =================================
// Wheather type
#define WHEATHER_TYPE_SUNNY 0
#define WHEATHER_TYPE_SNOW 1
#define WHEATHER_TYPE_RAIN 2


// =================================
// UI type
#define UI_TITLE_LOGO 0
#define UI_GAME_START_BUTTON 1
#define UI_TITLE_EXIT_BUTTON 2
#define UI_FADE_IMAGE 3
#define UI_FOG_IMAGE 4
#define UI_GAUGE_BAR_IMAGE 5
#define UI_GAUGE_IMAGE 6
#define UI_HP_IMAGE 7
#define UI_IGLOO_HP_BAR_IMAGE 8
#define UI_IGLOO_HP_GAUGE_IMAGE 9

// =================================


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

struct Material
{
	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
};