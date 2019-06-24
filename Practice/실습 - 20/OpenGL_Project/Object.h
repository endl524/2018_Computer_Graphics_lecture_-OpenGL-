#pragma once
#include "stdafx.h"
#include "Quaternion.h"

class Object
{
private: // 멤버변수 선언구역
	static int s_Object_Count;
	int m_Index = -1;

	float m_Move_Speed = 0.0f;

	Vector3 m_Position;
	int m_Auto_Move_Sign = 1;

	float m_Rotate_Pitch = 0.0f; // x축
	float m_Rotate_Yaw = 0.0f; // y축
	float m_Rotate_Roll = 0.0f; // z축
	Quaternion* m_Quaternion_Result = nullptr;
	Quaternion* m_Quaternion_temp_1 = nullptr;
	Quaternion* m_Quaternion_temp_2 = nullptr;
	Quaternion* m_Quaternion_temp_3 = nullptr;
	float m_Rotate_Transform_Matrix[16];


	float m_Scale_X = 1.0f;
	float m_Scale_Y = 1.0f;
	float m_Scale_Z = 1.0f;
	

	Color4f m_Color;
	int m_Color_Scale_Sign_R = 1;
	int m_Color_Scale_Sign_G = 1;
	int m_Color_Scale_Sign_B = 1;

	bool m_is_Destroyed = false;
	bool m_is_Activated = false;
	

	Object* m_Middle_Object = nullptr;
	float m_Stacked_Mid_Objs_Pitch;
	float m_Stacked_Mid_Objs_Yaw;
	Object* m_Upper_Object = nullptr;
	float m_Stacked_Upp_Objs_Pitch;
	float m_Stacked_Upp_Objs_Roll;


public: // Getter, Setter 정의구역
	void Set_Index(const int& index) { m_Index = index; }
	int const Get_Index() { return m_Index; }

	void Set_Object_Count(const int& value) { s_Object_Count = value; }
	int const Get_Object_Count() { return s_Object_Count; }

	void Set_Move_Speed(const float& value) { m_Move_Speed = value; }
	float const Get_Move_Speed() { return m_Move_Speed; }


	void Set_Translate(const float& x, const float& y, const float& z) { m_Position.x = x; m_Position.y = y; m_Position.z = z; }
	void Set_Position_X(const float& value) { m_Position.x = value; }
	float const Get_Position_X() { return m_Position.x; }
	void Set_Position_Y(const float& value) { m_Position.y = value; }
	float const Get_Position_Y() { return m_Position.y; }
	void Set_Position_Z(const float& value) { m_Position.z = value; }
	float const Get_Position_Z() { return m_Position.z; }

	void Set_Rotate_Roll(const float& value)
	{
		if (value >= 360.0f || value <= -360.0f)
		{
			int a = value / 360.0f;
			m_Rotate_Roll = value - a * 360;
		}
		else m_Rotate_Roll = value;
	}
	float const Get_Rotate_Roll() { return m_Rotate_Roll; }
	void Roll(const float& value) { m_Rotate_Roll = value; }

	void Set_Rotate_Yaw(const float& value)
	{
		if (value >= 360.0f || value <= -360.0f)
		{
			int a = value / 360.0f;
			m_Rotate_Yaw = value - a * 360;
		}
		else m_Rotate_Yaw = value;
	}
	float const Get_Rotate_Yaw() { return m_Rotate_Yaw; }
	void Yaw(const float& value) { m_Rotate_Yaw = value; }

	void Set_Rotate_Pitch(const float& value)
	{
		if (value >= 360.0f || value <= -360.0f)
		{
			int a = value / 360.0f;
			m_Rotate_Pitch = value - a * 360;
		}
		else m_Rotate_Pitch = value;
	}
	float const Get_Rotate_Pitch() { return m_Rotate_Pitch; }
	void Pitch(const float& value) { m_Rotate_Pitch = value; }

	float* Get_Rotate_Transform_Matrix() { return m_Rotate_Transform_Matrix; }


	void Set_Scale(const float& x, const float& y, const float& z) { m_Scale_X = x; m_Scale_Y = y; m_Scale_Z = z; }
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

public: // public 메소드 정의구역
	Object();
	Object(const bool& b) {}
	~Object();

	void Draw();

	void Init_Object();
	void Object_Setting();

	void Auto_ColorScale();
	void Auto_Move();
	void Move(const int& axis, const float& value);
	void Rotate_Check();

	void Update();

	void Middle_Rotate_X(const float& value);
	void Middle_Rotate_Y(const float& value);

	void Upper_Rotate_X(const float& value);
	void Upper_Rotate_Z(const float& value);
};

