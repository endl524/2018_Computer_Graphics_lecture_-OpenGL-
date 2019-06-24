#pragma once
#include "stdafx.h"

class Object
{
private: // ������� ���𱸿�
	static int s_Object_Count;
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

	int m_Type;
	bool m_is_Destroyed = false;
	bool m_is_Activated;

public: // Getter, Setter ���Ǳ���
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

	void Set_is_Destroyed(const float b) { m_is_Destroyed = b; }
	bool const Get_is_Destroyed() { return m_is_Destroyed; }

	void Set_Active(const bool b) { m_is_Activated = b; }
	bool const Get_is_Activated() { return m_is_Activated; }

	void Set_Type(const int t) { m_Type = t; }
	int const Get_Type() { return m_Type; }

public: // public �޼ҵ� ���Ǳ���
	Object(const GLenum& type, vector<Vertex2D>& v_vertexs, const float& pos_x, const float& pos_y, const float& size_x, const float& size_y, const bool& is_active, const int& obj_type);
	~Object();

	void Draw();

	void Auto_ColorScale();
	void Position_Update();

	void Collision_Check(const map<int, Object*>& object_list);
};

