#pragma once
#include "stdafx.h"
#include "Quaternion.h"
#include "OBB.h"

class Object
{
protected: // 멤버변수 선언구역
	static int s_Object_Count;
	int m_Index = -1;
	int m_Object_Type;

	float m_Move_Speed = 0.0f;

	// 위치
	Vector3 m_Position;
	int m_Auto_Move_Dir = 0;

	// 회전
	float m_Rotate_Pitch_Value = 0.0f; // x축
	float m_Rotate_Yaw_Value = 0.0f; // y축
	float m_Rotate_Roll_Value = 0.0f; // z축
	Quaternion* m_Quaternion_Result = nullptr;
	Quaternion* m_Quaternion_temp_1 = nullptr;
	Quaternion* m_Quaternion_temp_2 = nullptr;
	Quaternion* m_Quaternion_temp_3 = nullptr;
	float m_Rotate_Transform_Matrix[16];

	// 신축
	float m_Scale_X = 1.0f;
	float m_Scale_Y = 1.0f;
	float m_Scale_Z = 1.0f;

	// 색상
	Color4f m_Color;
	int m_Color_Scale_Sign_R = 1;
	int m_Color_Scale_Sign_G = 1;
	int m_Color_Scale_Sign_B = 1;

	bool m_is_Destroyed = false;
	bool m_is_Activated = false;
	bool m_is_Static_Object = false;

	OBB* m_OBB = nullptr;

	
	float m_Control_Points[4][7][3]{ 0.0f };
	int m_Control_Points_Y_Sign[4][7]{ 1 };
	int m_CP_Count = 0;
	bool m_is_Make_Over = false;
	bool m_is_Solid = true;
	int m_Plane_Type = GL_FILL;
	vector<Vector3> m_Polygon_Vertice;
	bool m_is_Auto_Moving = false;

public: // Getter, Setter 정의구역
	void Set_Index(const int& index) { m_Index = index; }
	int const Get_Index() { return m_Index; }

	void Set_Object_Count(const int& value) { s_Object_Count = value; }
	int const Get_Object_Count() { return s_Object_Count; }

	void Set_Object_Type(const int& value) { m_Object_Type = value; }
	int const Get_Object_Type() { return m_Object_Type; }

	void Set_Move_Speed(const float& value) { m_Move_Speed = value; }
	float const Get_Move_Speed() { return m_Move_Speed; }

	virtual void Set_Move_Direction(const int& dir) { m_Auto_Move_Dir = dir; }

	void Set_Translate(const float& x, const float& y, const float& z) { m_Position.x = x; m_Position.y = y; m_Position.z = z; }
	Vector3 const Get_Position() { return m_Position; }
	void Set_Position_X(const float& value) { m_Position.x = value; }
	float const Get_Position_X() { return m_Position.x; }
	void Set_Position_Y(const float& value) { m_Position.y = value; }
	float const Get_Position_Y() { return m_Position.y; }
	void Set_Position_Z(const float& value) { m_Position.z = value; }
	float const Get_Position_Z() { return m_Position.z; }


	void Set_Rotate_Pitch(const float& value)
	{
		if (value >= 360.0f || value <= -360.0f)
		{
			int a = value / 360.0f;
			m_Rotate_Pitch_Value = value - a * 360;
		}
		else m_Rotate_Pitch_Value = value;
	}
	float const Get_Rotate_Pitch() { return m_Rotate_Pitch_Value; }
	virtual void Pitch(const float& value) { m_Rotate_Pitch_Value = value; Rotate_Check(); }

	void Set_Rotate_Yaw(const float& value)
	{
		if (value >= 360.0f || value <= -360.0f)
		{
			int a = value / 360.0f;
			m_Rotate_Yaw_Value = value - a * 360;
		}
		else m_Rotate_Yaw_Value = value;
	}
	float const Get_Rotate_Yaw() { return m_Rotate_Yaw_Value; }
	virtual void Yaw(const float& value) { m_Rotate_Yaw_Value = value; Rotate_Check(); }

	void Set_Rotate_Roll(const float& value)
	{
		if (value >= 360.0f || value <= -360.0f)
		{
			int a = value / 360.0f;
			m_Rotate_Roll_Value = value - a * 360;
		}
		else m_Rotate_Roll_Value = value;
	}
	float const Get_Rotate_Roll() { return m_Rotate_Roll_Value; }
	virtual void Roll(const float& value) { m_Rotate_Roll_Value = value; Rotate_Check(); }
	float* Get_Rotate_Transform_Matrix() { return m_Rotate_Transform_Matrix; }


	virtual void Set_Scale(const float& x, const float& y, const float& z) { m_Scale_X = x; m_Scale_Y = y; m_Scale_Z = z; }
	void Set_Scale_X(const float& value) { m_Scale_X = value; }
	float const Get_Scale_X() { return m_Scale_X; }
	void Set_Scale_Y(const float& value) { m_Scale_Y = value; }
	float const Get_Scale_Y() { return m_Scale_Y; }
	void Set_Scale_Z(const float& value) { m_Scale_Z = value; }
	float const Get_Scale_Z() { return m_Scale_Z; }


	void Set_Color(const float& r, const float& g, const float& b, const float& a) { m_Color.R = r; m_Color.G = g; m_Color.B = b; m_Color.A = a; }
	float const Get_Color_R() { return m_Color.R; }
	float const Get_Color_G() { return m_Color.G; }
	float const Get_Color_B() { return m_Color.B; }
	float const Get_Color_A() { return m_Color.A; }

	void Set_is_Destroyed(const float& b) { m_is_Destroyed = b; }
	bool const Get_is_Destroyed() { return m_is_Destroyed; }

	void Set_Active(const bool& b) { m_is_Activated = b; }
	bool const Get_is_Activated() { return m_is_Activated; }

	void Set_is_Static(const bool& b) { m_is_Static_Object = b; }
	bool const Get_is_Static() { return m_is_Static_Object; }

	void Make_OBB() 
	{ 
		if (m_OBB == nullptr)
		{
			delete m_OBB; m_OBB = nullptr;
		}

		m_OBB = new OBB();
		m_OBB->OBB_Init(m_Position, m_Scale_X, m_Scale_Y, m_Scale_Z);
	}
	OBB* const Get_OBB() { return m_OBB; }
	virtual OBB* Get_Cube_Collider(const int& index) { return m_OBB; }
	virtual Vector4 const Get_Collider_Normal_Vector(const int& index) { Vector4 temp; return temp; }

public: // public 메소드 정의구역
	Object() { Init_Object(); };
	Object(const bool& b);
	~Object();

	void Draw_Plane(const int& point_count, const int& start_index);
	virtual void Draw();

	void Init_Object();
	virtual void Make_Model_Vertice() {};

	void Auto_Move();
	void Auto_ColorScale();
	void Auto_Scale();
	void Move(const int& axis, const float& value);
	void Rotate_Check();

	virtual void Update();
	virtual void Collision_Check(Object* other) {};
	virtual void Collision_Procedure();
	virtual void Collision_Procedure(const Vector4& collider_Normal_Vector);
	int count = 0;

	void Insert_Control_Point(const float& cp_x, const float& cp_z) 
	{ 
		if (m_CP_Count >= 8) return;

		if (m_CP_Count == 0)
		{
			m_Control_Points[0][0][0] = cp_x;
			m_Control_Points[0][0][2] = cp_z;
		}
		else if (m_CP_Count == 1)
		{
			for (int i = 1; i <= 3; ++i)
			{
				m_Control_Points[i][0][0] = m_Control_Points[0][0][0] + ((cp_x - m_Control_Points[0][0][0]) / 3.0f * i);
				m_Control_Points[i][0][2] = m_Control_Points[0][0][2] + ((cp_z - m_Control_Points[0][0][2]) / 3.0f * i);
			}
		}
		else if (m_CP_Count == 2)
		{
			for (int i = 1; i <= 2; ++i)
			{
				m_Control_Points[0][i][0] = m_Control_Points[0][0][0] + ((cp_x - m_Control_Points[0][0][0]) / 2.0f * i);
				m_Control_Points[0][i][2] = m_Control_Points[0][0][2] + ((cp_z - m_Control_Points[0][0][2]) / 2.0f * i);
			}
		}
		else if (m_CP_Count == 3)
		{
			for (int i = 1; i <= 3; ++i)
			{
				m_Control_Points[i][2][0] = m_Control_Points[0][2][0] + ((cp_x - m_Control_Points[0][2][0]) / 3.0f * i);
				m_Control_Points[i][2][2] = m_Control_Points[0][2][2] + ((cp_z - m_Control_Points[0][2][2]) / 3.0f * i);
			}

			for (int i = 1; i <= 3; ++i)
			{
				m_Control_Points[i][1][0] = m_Control_Points[i][0][0] + ((m_Control_Points[i][2][0] - m_Control_Points[i][0][0]) / 2.0f);
				m_Control_Points[i][1][2] = m_Control_Points[i][0][2] + ((m_Control_Points[i][2][2] - m_Control_Points[i][0][2]) / 2.0f);
			}
		}

		// ====================================

		else if (m_CP_Count == 4)
		{
			for (int i = 1; i <= 2; ++i)
			{
				m_Control_Points[0][i+2][0] = m_Control_Points[0][2][0] + ((cp_x - m_Control_Points[0][2][0]) / 2.0f * i);
				m_Control_Points[0][i+2][2] = m_Control_Points[0][2][2] + ((cp_z - m_Control_Points[0][2][2]) / 2.0f * i);
			}
		}
		
		else if (m_CP_Count == 5)
		{
			for (int i = 1; i <= 3; ++i)
			{
				m_Control_Points[i][4][0] = m_Control_Points[0][4][0] + ((cp_x - m_Control_Points[0][4][0]) / 3.0f * i);
				m_Control_Points[i][4][2] = m_Control_Points[0][4][2] + ((cp_z - m_Control_Points[0][4][2]) / 3.0f * i);
			}
			for (int i = 1; i <= 3; ++i)
			{
				m_Control_Points[i][3][0] = m_Control_Points[i][2][0] + ((m_Control_Points[i][4][0] - m_Control_Points[i][2][0]) / 2.0f);
				m_Control_Points[i][3][2] = m_Control_Points[i][2][2] + ((m_Control_Points[i][4][2] - m_Control_Points[i][2][2]) / 2.0f);
			}
		}

		// =====================================

		else if (m_CP_Count == 6)
		{
			for (int i = 1; i <= 2; ++i)
			{
				m_Control_Points[0][i + 4][0] = m_Control_Points[0][4][0] + ((cp_x - m_Control_Points[0][4][0]) / 2.0f * i);
				m_Control_Points[0][i + 4][2] = m_Control_Points[0][4][2] + ((cp_z - m_Control_Points[0][4][2]) / 2.0f * i);
			}
		}
		else if (m_CP_Count == 7)
		{
			for (int i = 1; i <= 3; ++i)
			{
				m_Control_Points[i][6][0] = m_Control_Points[0][6][0] + ((cp_x - m_Control_Points[0][6][0]) / 3.0f * i);
				m_Control_Points[i][6][2] = m_Control_Points[0][6][2] + ((cp_z - m_Control_Points[0][6][2]) / 3.0f * i);
			}
			for (int i = 1; i <= 3; ++i)
			{
				m_Control_Points[i][5][0] = m_Control_Points[i][4][0] + ((m_Control_Points[i][6][0] - m_Control_Points[i][4][0]) / 2.0f);
				m_Control_Points[i][5][2] = m_Control_Points[i][4][2] + ((m_Control_Points[i][6][2] - m_Control_Points[i][4][2]) / 2.0f);
			}
		}

		++m_CP_Count;

		m_is_Make_Over = m_CP_Count >= 8 ? true : false;
	}
	
	Vector2 Find_Near_CP(const int& x, const int& y);
	void CP_Move(const Vector2& index, const int& x, const int& y);

	void Switch_is_Solid() { m_is_Solid = m_is_Solid == true ? false : true; if (m_is_Solid) m_Plane_Type = GL_FILL; else m_Plane_Type = GL_LINE; }
	bool const Get_is_Solid() { return m_is_Solid; }
	void Set_is_Make_Over(const bool& b) { m_is_Make_Over = b; }
	bool const Get_is_Make_Over() { return m_is_Make_Over; }
	void Insert_Polygon_Vertex(const float& x, const float& z)
	{
		Vector3 v;
		v.x = x; v.z = z; v.y = 0.0f;
		m_Polygon_Vertice.push_back(v);
	}

	void Auto_Moving_Switch()
	{
		m_is_Auto_Moving = m_is_Auto_Moving == true ? false : true;
	}

	void Set_Random_Speed(const int& i, const int& j, const float& speed)
	{
		m_Control_Points_Y_Sign[i][j] *= speed;
	}
};

