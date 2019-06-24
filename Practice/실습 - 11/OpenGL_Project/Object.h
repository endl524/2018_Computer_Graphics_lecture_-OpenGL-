#pragma once
#include "stdafx.h"

class Object
{
private: // 멤버변수 선언구역
	static int s_Object_Count;
	int m_Index = -1;
	GLenum m_Drawing_Type;
	int m_Object_Type;

	list<Vertex2D> m_Vertexs;

	random_device m_Random_Device;
	mt19937_64 m_Random_SEED;
	uniform_real_distribution<> m_Random_Color;

	float m_Size;
	int m_Animation_Sign = 0;
	int m_Color_Scale_Sign_R = 1;
	int m_Color_Scale_Sign_G = 1;
	int m_Color_Scale_Sign_B = 1;

	bool m_is_Destroyed = false;
	bool m_is_Activated;
	float m_Elapsed_Time = 0.0f;
	float m_Radian = 0.0f;
	float m_Spring_Center_X = 0.0f;
	bool m_is_Moved_Once = false;
	float m_Scale_Size = 1.0f;
	float m_Scale_Pos = 0.0f;

public: // Getter, Setter 정의구역
	void Set_Index(const int& index) { m_Index = index; }
	int const Get_Index() { return m_Index; }
	void Set_Drawing_Type(const GLenum& type) { m_Drawing_Type = type; }
	GLenum const Get_Drawing_Type() { return m_Drawing_Type; }

	void Set_Vertexs_vector(list<Vertex2D>& v_vertexs) { m_Vertexs = v_vertexs; }
	list<Vertex2D> Get_VertexList() { return m_Vertexs; }

	void Set_is_Destroyed(const float& b) { m_is_Destroyed = b; }
	bool const Get_is_Destroyed() { return m_is_Destroyed; }

	void Set_Active(const bool& b) { m_is_Activated = b; }
	bool const Get_is_Activated() { return m_is_Activated; }

	void Set_Animation_Sign(const int& s) { m_Animation_Sign = s; }
	int const Get_Animation_Sign() { return m_Animation_Sign; }

	void Set_Radian(const float& r) { m_Radian = r; }
	float const Get_Radian() { return m_Radian; }

	void Set_is_Moved_Once(const bool& b) { m_is_Moved_Once = b; }
	bool const Get_is_Moved_Once() { return m_is_Moved_Once; }

public: // public 메소드 정의구역
	Object(const bool& is_active);
	~Object();

	void Draw();

	void Make_Sin_Curve();
	void Make_Spring();
	void Make_Ribbon();
	void Make_Rectangle();

	void Set_Random_Device();
	void Shape_Change(const int& type);
	void Auto_ColorScale();
	void Animation();
	void Move(const int& dir);
	void Scale(const int& sign);
	void Update();
};

