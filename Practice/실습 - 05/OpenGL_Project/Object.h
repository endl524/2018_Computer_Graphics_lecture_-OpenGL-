#pragma once
#include "stdafx.h"

class Object
{
private: // 멤버변수 선언구역
	int m_Index = -1;
	GLenum m_Drawing_Type;
	int m_Shape_Type;
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

	float m_Direction_X;
	float m_Direction_Y;
	float m_Speed;

	static float s_Main_Speed;

public: // Getter, Setter 정의구역
	void Set_Index(const int index) { m_Index = index; }
	int const Get_Index() { return m_Index; }
	void Set_Drawing_Type(const GLenum type) { m_Drawing_Type = type; }
	GLenum const Get_Drawing_Type() { return m_Drawing_Type; }
	void Set_Shape_Type(const int type) { m_Shape_Type = type; }
	int const Get_Shape_Type() { return m_Shape_Type; }

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

	void Set_Direction(const float x, const float y) { m_Direction_X = x; m_Direction_Y = y; }
	void Set_Direction_X(const float x) { m_Direction_X = x; }
	void Set_Direction_Y(const float y) { m_Direction_Y = y; }
	float const Get_Direction_X() { return m_Direction_X; }
	float const Get_Direction_Y() { return m_Direction_Y; }

	void Set_Speed(const float y) { m_Speed = y; }
	float const Get_Speed() { return m_Speed; }

public: // public 메소드 정의구역
	Object(const GLenum type, const int m_Shape_Type, vector<Vertex2D>& v_vertexs, const int index, const float pos_x, const float pos_y, const float size_x, const float size_y, const float dir_x, const float dir_y, const float speed);
	~Object();

	void Draw();
	void Auto_Move();
	void Auto_Scale();
	void Change_Speed(const int sign);
};

