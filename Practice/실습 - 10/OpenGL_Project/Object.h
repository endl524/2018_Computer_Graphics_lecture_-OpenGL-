#pragma once
#include "stdafx.h"

class Object
{
private: // 멤버변수 선언구역
	static int s_Object_Count;
	int m_Index = -1;
	GLenum m_Drawing_Type;
	vector<Vertex2D> m_Vertexs;

	float m_Position_X;
	float m_Position_Y;
	float m_Prev_Position_X;
	float m_Prev_Position_Y;
	float m_Size_X;
	float m_Size_Y;
	int m_Color_Scale_Sign_R = 1;
	int m_Color_Scale_Sign_G = 1;
	int m_Color_Scale_Sign_B = 1;
	int m_SizeScale_Sign = 1;

	float m_Direction_X;
	float m_Direction_Y;
	float m_Prev_Direction_X;
	float m_Prev_Direction_Y;
	float m_Speed;
	static float s_Main_Speed;
	static bool m_is_Stop_Moving;
	int m_Moving_Type;
	int m_Wall_Riding_Direction;
	bool m_Wall_Riding_Touch_Check[6]{ false };

	int m_Type;
	bool m_is_Destroyed = false;
	bool m_is_Activated;
	bool m_is_Transforming = false;
	bool m_is_Transform_Triangle = false;
	bool m_is_Transform_Rectangle = false;
	float m_Transform_Relative_Value_X = 1.0f;
	float m_Elapsed_Time = 0.0f;
	bool m_is_Over_Wait_to_Scale_Size = false;
	bool m_is_Over_Wait_to_Transform_Rect = false;

public: // Getter, Setter 정의구역
	void Set_Index(const int& index) { m_Index = index; }
	int const Get_Index() { return m_Index; }
	void Set_Drawing_Type(const GLenum& type) { m_Drawing_Type = type; }
	GLenum const Get_Drawing_Type() { return m_Drawing_Type; }

	void Set_Vertexs_vector(vector<Vertex2D>& v_vertexs) { m_Vertexs = v_vertexs; }
	vector<Vertex2D> Get_VertexList() { return m_Vertexs; }
	Vertex2D const Get_Vertex(const int& index) { return m_Vertexs[index]; }

	void Set_Position(const float& x, const float& y) { m_Position_X = x; m_Position_Y = y; }
	void Set_Position_X(const float& x) { m_Position_X = x; }
	void Set_Position_Y(const float& y) { m_Position_Y = y; }
	float const Get_Position_X() { return m_Position_X; }
	float const Get_Position_Y() { return m_Position_Y; }

	void Set_Size(const float& x, const float& y) { m_Size_X = x; m_Size_Y = y; }
	void Set_Size_X(const float& x) { m_Size_X = x; }
	void Set_Size_Y(const float& y) { m_Size_Y = y; }
	float const Get_Size_X() { return m_Size_X; }
	float const Get_Size_Y() { return m_Size_Y; }

	void Set_Direction(const float& x, const float& y) { m_Direction_X = x; m_Direction_Y = y; }
	void Set_Direction_X(const float& x) { m_Direction_X = x; }
	void Set_Direction_Y(const float& y) { m_Direction_Y = y; }
	float const Get_Direction_X() { return m_Direction_X; }
	float const Get_Direction_Y() { return m_Direction_Y; }

	void Set_Speed(const float& y) { m_Speed = y; }
	float const Get_Speed() { return m_Speed; }

	void Set_Moving_Type(const int& type) { m_Moving_Type = type; }
	int const Get_Moving_Type() { return m_Moving_Type; }

	void Set_Wall_Riding_Direction(const int& dir_type) { m_Wall_Riding_Direction = dir_type; }
	int const Get_Wall_Riding_Direction() { return m_Wall_Riding_Direction; }

	void Set_is_Destroyed(const float& b) { m_is_Destroyed = b; }
	bool const Get_is_Destroyed() { return m_is_Destroyed; }

	void Set_Active(const bool& b) { m_is_Activated = b; }
	bool const Get_is_Activated() { return m_is_Activated; }

	void Set_is_Transforming(const bool& b) { m_is_Transforming = b; }
	bool const Get_is_Transforming() { return m_is_Transforming; }

	void Set_Type(const int t) { m_Type = t; }
	int const Get_Type() { return m_Type; }

public: // public 메소드 정의구역
	Object(const GLenum& type, vector<Vertex2D>& v_vertexs, const float& pos_x, const float& pos_y, const float& size_x, const float& size_y, const float& dir_x, const float& dir_y, const float& speed, const bool& is_active, const int& obj_type);
	~Object();

	void Draw();
	void Auto_Bounding_Move();
	void Auto_Wall_Riding_Move();
	void Start_Wall_Riding(const int wall_riding_dir);
	void Finish_Wall_Riding();

	void Auto_ColorScale();
	void Auto_SizeScale();
	void Position_Update();
	void Change_Speed(const int sign);
	void Stop_Move();
	void Start_Move();

	void Collision_Check(const map<int, Object*>& object_list);
	void Transforming_Update();
};

