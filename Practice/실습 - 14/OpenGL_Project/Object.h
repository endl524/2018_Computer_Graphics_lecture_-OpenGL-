#pragma once
#include "stdafx.h"

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


	float m_Local_Translate_X = 0.0f;
	float m_Local_Translate_Y = 0.0f;
	float m_Local_Translate_Z = 0.0f;
	float m_Auto_Translate_Value_X = 0.0f;
	float m_Auto_Translate_Value_Y = 0.0f;
	float m_Vertexs_Distance_X = 0.0f;
	float m_Vertexs_Distance_Y = 0.0f;
	float m_Distance_Length = 0.0f;
	

	float m_Local_Rotate_Angle = 0.0f;
	float m_Local_Rotate_X = 0.0f;
	float m_Local_Rotate_Y = 0.0f;
	float m_Local_Rotate_Z = 0.0f;
	float m_Auto_Rotate_Angle = 0.0f;
	bool m_is_Auto_Rotating = false;
	int m_Rotate_Direction = 0;


	float m_Local_Scale_X = 1.0f;
	float m_Local_Scale_Y = 1.0f;
	float m_Local_Scale_Z = 1.0f;
	int m_Auto_Scale_Sign = 1;
	bool m_is_Auto_Scaling = false;
	

	Color4f m_Color;
	int m_Color_Scale_Sign_R = 1;
	int m_Color_Scale_Sign_G = 1;
	int m_Color_Scale_Sign_B = 1;

	bool m_is_Destroyed = false;
	bool m_is_Activated;
	float m_Elapsed_Time = 0.0f;


public: // Getter, Setter 정의구역
	void Set_Index(const int& index) { m_Index = index; }
	int const Get_Index() { return m_Index; }

	void Set_Auto_Rotate(const bool& b) { m_is_Auto_Rotating = b; }
	bool const Get_Auto_Rotate() { return m_is_Auto_Rotating; }
	void Set_is_Auto_Scaling(const bool& b) { m_is_Auto_Scaling = b; }
	bool const Get_is_Auto_Scaling() { return m_is_Auto_Scaling; }

	void Set_Local_Translate(const float& x, const float& y, const float& z) { m_Local_Translate_X = x; m_Local_Translate_Y = y; m_Local_Translate_Z = z; }
	void Set_Local_Translate_X(const float& value) { m_Local_Translate_X = value; }
	bool const Get_Local_Translate_X() { return m_Local_Translate_X; }
	void Set_Local_Translate_Y(const float& value) { m_Local_Translate_Y = value; }
	bool const Get_Local_Translate_Y() { return m_Local_Translate_Y; }
	void Set_Local_Translate_Z(const float& value) { m_Local_Translate_Z = value; }
	bool const Get_Local_Translate_Z() { return m_Local_Translate_Z; }


	void Set_Local_Rotate_Angle(const float& value) { m_Local_Rotate_Angle = value; }
	bool const Get_Local_Rotate_Angle() { return m_Local_Rotate_Angle; }
	void Set_Local_Rotate_X(const float& value) { m_Local_Rotate_X = value; }
	bool const Get_Local_Rotate_X() { return m_Local_Rotate_X; }
	void Set_Local_Rotate_Y(const float& value) { m_Local_Rotate_Y = value; }
	bool const Get_Local_Rotate_Y() { return m_Local_Rotate_Y; }
	void Set_Local_Rotate_Z(const float& value) { m_Local_Rotate_Z = value; }
	bool const Get_Local_Rotate_Z() { return m_Local_Rotate_Z; }


	void Set_Local_Scale(const float& x, const float& y, const float& z) { m_Local_Scale_X = x; m_Local_Scale_Y = y; m_Local_Scale_Z = z; }
	bool const Get_Local_Scale_X() { return m_Local_Scale_X; }
	bool const Get_Local_Scale_Y() { return m_Local_Scale_Y; }
	bool const Get_Local_Scale_Z() { return m_Local_Scale_Z; }

	void Set_is_Destroyed(const float& b) { m_is_Destroyed = b; }
	bool const Get_is_Destroyed() { return m_is_Destroyed; }

	void Set_Active(const bool& b) { m_is_Activated = b; }
	bool const Get_is_Activated() { return m_is_Activated; }

	void Reverse_Rotate_Direction() { m_Rotate_Direction *= -1; }


public: // public 메소드 정의구역
	Object(const bool& is_active);
	~Object();

	void Draw();

	void Init_Object();

	void Set_Random_Device();
	void Auto_ColorScale();
	void Auto_Rotating();
	void Auto_Scaling();

	void Local_Scale(const int& sign);

	void Update();
};

