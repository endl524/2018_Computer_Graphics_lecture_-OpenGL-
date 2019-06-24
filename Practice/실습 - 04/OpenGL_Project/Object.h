#pragma once
#include "stdafx.h"

#define RECT_SIZE_MIN_X 30
#define RECT_SIZE_MAX_X 100
#define RECT_SIZE_MIN_Y 30
#define RECT_SIZE_MAX_Y 100

#define COLOR_SCALE_VALUE_R 0.05
#define COLOR_SCALE_VALUE_G 0.05
#define COLOR_SCALE_VALUE_B 0.05
#define SCALE_VALUE_X 5
#define SCALE_VALUE_Y 5

class Object
{
private: // 멤버변수 선언구역
	int m_Index = -1;
	GLenum m_Drawing_Type;
	vector<Vertex2D> m_Vertexs;

	float m_Position_X;
	float m_Position_Y;
	float m_Size_X;
	float m_Size_Y;
	int m_Color_Scale_Sign_R = 1;
	int m_Color_Scale_Sign_G = 1;
	int m_Color_Scale_Sign_B = 1;
	int m_Scale_Sign_X = 1;
	int m_Scale_Sign_Y = 1;

public: // Getter, Setter 정의구역
	void Set_Index(const int index) { m_Index = index; }
	int const Get_Index() { return m_Index; }
	void Set_Drawing_Type(const GLenum type) { m_Drawing_Type = type; }
	GLenum const Get_Drawing_Type() { return m_Drawing_Type; }

	void Set_Vertexs_vector(vector<Vertex2D>& v_vertexs) { m_Vertexs = v_vertexs; }
	vector<Vertex2D> Get_VertexList() { return m_Vertexs; }
	Vertex2D const Get_Vertex(const int index) { return m_Vertexs[index]; }

	void Set_Position(const float x, const float y) { m_Position_X = x; m_Position_Y = y; }
	void Set_Position_X(const float x) { m_Position_X = x; }
	void Set_Position_Y(const float y) { m_Position_Y = y; }
	float const Get_Position_X() { return m_Position_X; }
	float const Get_Position_Y() { return m_Position_Y; }

	void Set_Size(const float x, const float y) { m_Size_X = x; m_Size_Y = y; }
	void Set_Size_X(const float x) { m_Size_X = x; }
	void Set_Size_Y(const float y) { m_Size_Y = y; }
	float const Get_Size_X() { return m_Size_X; }
	float const Get_Size_Y() { return m_Size_Y; }

public: // public 메소드 정의구역
	Object(const GLenum type, vector<Vertex2D>& v_vertexs, const int index, const float pos_x, const float pos_y, const float size_x, const float size_y);
	~Object();

	void Draw();
	void Auto_Scale();
};

