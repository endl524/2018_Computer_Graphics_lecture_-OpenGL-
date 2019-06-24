#pragma once
#include "stdafx.h"
#include "Camera.h"

class Object
{
private: // 멤버변수 선언구역
	static int s_Object_Count;
	int m_Index = -1;
	int m_Object_Type;

	random_device m_Random_Device;
	mt19937_64 m_Random_SEED;
	uniform_real_distribution<> m_Random_Color;
	uniform_real_distribution<> m_Random_Axis;
	uniform_real_distribution<> m_Random_Scale;
	uniform_int_distribution<> m_Random_Rotate_Direction;
	uniform_int_distribution<> m_Random_Type;


	float m_Position_X = 0.0f;
	float m_Position_Y = 0.0f;
	float m_Position_Z = 0.0f;
	

	float m_Rotate_Roll = 0.0f; // z축
	float m_Rotate_Yaw = 0.0f; // y축
	float m_Rotate_Pitch = 0.0f; // x축
	static float s_World_Rotate_Pitch;
	static float s_World_Rotate_Yaw;
	static float s_World_Rotate_Roll;


	float m_Scale_X = 1.0f;
	float m_Scale_Y = 1.0f;
	float m_Scale_Z = 1.0f;
	

	Color4f m_Color;
	int m_Color_Scale_Sign_R = 1;
	int m_Color_Scale_Sign_G = 1;
	int m_Color_Scale_Sign_B = 1;

	bool m_is_Destroyed = false;
	bool m_is_Activated = false;
	bool m_is_Solid_Shape = false;
	bool m_is_Shape_Object = false;


public: // Getter, Setter 정의구역
	void Set_Index(const int& index) { m_Index = index; }
	int const Get_Index() { return m_Index; }

	void Set_Object_Type(const int& type) { m_Object_Type = type; }
	int const Get_Object_Type() { return m_Object_Type; }

	void Set_Translate(const float& x, const float& y, const float& z) { m_Position_X = x; m_Position_Y = y; m_Position_Z = z; }
	void Set_Position_X(const float& value) { m_Position_X = value; }
	float const Get_Position_X() { return m_Position_X; }
	void Set_Position_Y(const float& value) { m_Position_Y = value; }
	float const Get_Position_Y() { return m_Position_Y; }
	void Set_Position_Z(const float& value) { m_Position_Z = value; }
	float const Get_Position_Z() { return m_Position_Z; }

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

	void Set_Scale(const float& x, const float& y, const float& z) { m_Scale_X = x; m_Scale_Y = y; m_Scale_Z = z; }
	void Set_Scale_X(const float& value) { m_Scale_X = value; }
	float const Get_Scale_X() { return m_Scale_X; }
	void Set_Scale_Y(const float& value) { m_Scale_Y = value; }
	float const Get_Scale_Y() { return m_Scale_Y; }
	void Set_Scale_Z(const float& value) { m_Scale_Z = value; }
	float const Get_Scale_Z() { return m_Scale_Z; }


	void Set_Color(const float& r, const float& g, const float& b, const float& a) { m_Color.R = r; m_Color.G = g; m_Color.B = b; m_Color.A = a; }


	void Set_is_Destroyed(const float& b) { m_is_Destroyed = b; }
	bool const Get_is_Destroyed() { return m_is_Destroyed; }

	void Set_Active(const bool& b) { m_is_Activated = b; }
	bool const Get_is_Activated() { return m_is_Activated; }

	void Set_is_Solid_Shape(const bool& b) { m_is_Solid_Shape = b; }
	bool const Get_is_Solid_Shape() { return m_is_Solid_Shape; }

	void Set_is_Shape_Object(const bool& b) { m_is_Shape_Object = b; }
	bool const Get_is_Shape_Object() { return m_is_Shape_Object; }
	
	void Set_World_Rotate_Pitch(const float& value)
	{
		if (value >= 360.0f || value <= -360.0f)
		{
			int a = value / 360.0f;
			s_World_Rotate_Pitch = value - a * 360;
		}
		else s_World_Rotate_Pitch = value;
	}
	float const Get_World_Rotate_Pitch() { return s_World_Rotate_Pitch; }

	void Set_World_Rotate_Yaw(const float& value)
	{
		if (value >= 360.0f || value <= -360.0f)
		{
			int a = value / 360.0f;
			s_World_Rotate_Yaw = value - a * 360;
		}
		else s_World_Rotate_Yaw = value;
	}
	float const Get_World_Rotate_Yaw() { return s_World_Rotate_Yaw; }

	void Set_World_Rotate_Roll(const float& value)
	{
		if (value >= 360.0f || value <= -360.0f)
		{
			int a = value / 360.0f;
			s_World_Rotate_Roll = value - a * 360;
		}
		else s_World_Rotate_Roll = value;
	}
	float const Get_World_Rotate_Roll() { return s_World_Rotate_Roll; }

public: // public 메소드 정의구역
	Object();
	Object(const bool& b) {}
	~Object();

	void Draw();

	void Init_Object();

	void Set_Random_Device();
	void Auto_ColorScale();

	void Scale_Once(const int& sign);

	void Shape_Change(const int& type, const bool& is_Left);
	void Rotate(const int& axis, const float& value);
	void World_Rotate(const int& axis, const float& value);

	void Update();
};

