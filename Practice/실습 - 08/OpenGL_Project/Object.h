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

	int m_Color_Scale_Sign_R = 1;
	int m_Color_Scale_Sign_G = 1;
	int m_Color_Scale_Sign_B = 1;

	float m_Forward_Move_Distance;

	float m_Speed;
	float m_New_Vertex_Time = 0.0f;
	int m_Round_Number;
	bool m_is_Clockwise;
	bool m_is_Destroyed = false;
	bool m_is_Draw_Over[2]{ false };

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

	void Set_New_Vertex_Time(const float t) { m_New_Vertex_Time = t; }
	float const Get_New_Vertex_Time() { return m_New_Vertex_Time; }

	void Set_Speed(const float y) { m_Speed = y; }
	float const Get_Speed() { return m_Speed; }

	void Set_Round_Number(const int n) { m_Round_Number = n; }
	int const Get_Round_Number() { return m_Round_Number; }

	void Set_is_Clockwise(const float b) { m_is_Clockwise = b; }
	bool const Get_is_Clockwise() { return m_is_Clockwise; }

	void Set_is_Destroyed(const float b) { m_is_Destroyed = b; }
	bool const Get_is_Destroyed() { return m_is_Destroyed; }

	bool const Get_is_Drawing_Over() { return m_is_Draw_Over[1]; }

public: // public 메소드 정의구역
	Object(const GLenum type, vector<Vertex2D>& v_vertexs, const int index, const float pos_x, const float pos_y, const float speed, const int round_num, const bool is_Clockwise);
	~Object();

	void Draw();

	void Auto_ColorScale();
	void Auto_Drawing();
	void Set_New_Vertex();
};

