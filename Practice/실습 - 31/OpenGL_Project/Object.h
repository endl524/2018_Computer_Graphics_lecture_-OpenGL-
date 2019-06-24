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

	bool m_is_Auto_Moving = false;

	


	float m_Ambient_Color[4];
	float m_Diffuse_Color[4];
	float m_Specular_Color[4];

	int m_Light_Number = 0;
	float m_Light_Pos[4];
	float m_Light_Dir[3]{0.0f, 0.0f, -1.0f};
	float m_Point_Light_Rot_Value = 0.0f;
	

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


	bool Set_Light_Number(const int& num)
	{
		bool is_Correct_Num = true;
		switch (num)
		{
		case 0: m_Light_Number = GL_LIGHT0; break;
		case 1: m_Light_Number = GL_LIGHT1; break;
		case 2: m_Light_Number = GL_LIGHT2; break;
		case 3: m_Light_Number = GL_LIGHT3; break;
		case 4: m_Light_Number = GL_LIGHT4; break;
		case 5: m_Light_Number = GL_LIGHT5; break;
		case 6: m_Light_Number = GL_LIGHT6; break;
		case 7: m_Light_Number = GL_LIGHT7; break;
		default: is_Correct_Num = false; break;
		}
		return is_Correct_Num;
	}

	int Get_Light_Number() { return m_Light_Number; }

	void Set_Ambient_Color(const float& r, const float& g, const float& b, const float& a) 
	{ 
		m_Ambient_Color[0] = r; m_Ambient_Color[1] = g; m_Ambient_Color[2] = b; m_Ambient_Color[3] = a;
		if (m_Light_Number != 0) glLightfv(m_Light_Number, GL_AMBIENT, m_Ambient_Color);
	}
	void Set_Diffuse_Color(const float& r, const float& g, const float& b, const float& a) 
	{ 
		m_Diffuse_Color[0] = r; m_Diffuse_Color[1] = g; m_Diffuse_Color[2] = b; m_Diffuse_Color[3] = a;
		if (m_Light_Number != 0) glLightfv(m_Light_Number, GL_DIFFUSE, m_Diffuse_Color);
	}
	void Set_Specular_Color(const float& r, const float& g, const float& b, const float& a) 
	{
		m_Specular_Color[0] = r; m_Specular_Color[1] = g; m_Specular_Color[2] = b; m_Specular_Color[3] = a;
		if (m_Light_Number != 0) glLightfv(m_Light_Number, GL_SPECULAR, m_Specular_Color);
	}
	void Set_Light_Position(const float& x, const float& y, const float& z, const float& a) 
	{ 
		if (m_Light_Number != 0)
		{
			m_Light_Pos[0] = x; m_Light_Pos[1] = y; m_Light_Pos[2] = z; m_Light_Pos[3] = a;
		}
	}
	void Set_Light_Direction(const float& x, const float& y, const float& z)
	{
		if (m_Light_Number != 0)
		{
			m_Light_Dir[0] = x; m_Light_Dir[1] = y; m_Light_Dir[2] = z;
			glLightfv(m_Light_Number, GL_SPOT_DIRECTION, m_Light_Dir);
		}
	}

	void Set_Light_Spot_Exponent(const float& v)
	{
		if (m_Light_Number != 0)
		{
			glLightf(m_Light_Number, GL_SPOT_EXPONENT, v);
		}
	}

	void Rotate_Light(const float& v) 
	{ 
		m_Point_Light_Rot_Value += v;

		if (m_Point_Light_Rot_Value >= 360.0f || m_Point_Light_Rot_Value <= -360.0f)
		{
			int a = m_Point_Light_Rot_Value / 360.0f;
			m_Point_Light_Rot_Value = m_Point_Light_Rot_Value - a * 360;
		}
	}

	void Control_Light_Diffuse(const int& sign)
	{
		m_Diffuse_Color[0] += 0.1f * sign;
		m_Diffuse_Color[1] += 0.1f * sign;
		m_Diffuse_Color[2] += 0.1f * sign;
		if (m_Light_Number != 0) glLightfv(m_Light_Number, GL_DIFFUSE, m_Diffuse_Color);
	}

	void Control_Light_Specular(const int& sign)
	{
		m_Specular_Color[0] += 0.1f * sign;
		m_Specular_Color[1] += 0.1f * sign;
		m_Specular_Color[2] += 0.1f * sign;
		if (m_Light_Number != 0) glLightfv(m_Light_Number, GL_SPECULAR, m_Specular_Color);
	}


public: // public 메소드 정의구역
	Object() { Init_Object(); };
	Object(const bool& b);
	~Object();

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

	void Auto_Moving_Switch()
	{
		m_is_Auto_Moving = m_is_Auto_Moving == true ? false : true;
	}

	void Set_Random_Speed(const int& i, const int& j, const float& speed)
	{
		
	}

	void Light_Switch();
	void Light_On() { glEnable(m_Light_Number); }
	void Light_Off() { glDisable(m_Light_Number); }
};

