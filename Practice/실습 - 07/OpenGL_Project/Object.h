#pragma once
#include "stdafx.h"

class Object
{
private: // 멤버변수 선언구역
	int m_Index = -1;
	GLenum m_Drawing_Type;
	vector<Vertex2D> m_Vertexs;

	float m_Position_X;
	float m_Position_Y;
	float m_Radius;

	int m_Color_Scale_Sign_R = 1;
	int m_Color_Scale_Sign_G = 1;
	int m_Color_Scale_Sign_B = 1;

	float m_Speed;

	bool m_is_Max_Radius = false;
	bool m_is_Through_Edge = false;
	bool m_is_Destroyed = false;

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

	void Set_Radius(const float r) { m_Radius = r; }
	float const Get_Radius() { return m_Radius; }

	void Set_Speed(const float y) { m_Speed = y; }
	float const Get_Speed() { return m_Speed; }

	void Set_is_Through_Edge(const bool b) { m_is_Through_Edge = b; }
	bool const Get_is_Through_Edge() { return m_is_Through_Edge; }

	void Set_is_Destroyed(const bool b) { m_is_Destroyed = b; }
	bool const Get_is_Destroyed() { return m_is_Destroyed; }

public: // public 메소드 정의구역
	Object(const GLenum type, vector<Vertex2D>& v_vertexs, const int index, const float pos_x, const float pos_y, const float speed, const bool through_edge);
	~Object();

	void Draw();

	void Auto_ColorScale();
	void Auto_Scale();
	void Scale_Refresh();
};

