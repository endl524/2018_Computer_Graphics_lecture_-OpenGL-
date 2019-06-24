#pragma once
#include "stdafx.h"
#include "Quaternion.h"
#include "OBB.h"
#include "Texture.h"

#define EDGE_COLLIDE_PROCEDURE 0
#define OBJECT_COLLIDE_PROCEDURE 1

class Object
{
protected: // 멤버변수 선언구역
	static int s_Object_Count;
	int m_Index = -1;
	int m_Object_Type;

	// 위치
	Vector3 m_Position;
	float m_Head_Position[4] = { 0.0f, };
	float m_Look_Vector_For_Camera[3];
	float m_Forward_Vector[4]{0.0f, 0.0f, 1.0f, 1.0f};
	float m_Right_Vector[4]{ -1.0f, 0.0f, 0.0f, 1.0f };
	float m_Up_Vector[4]{ 0.0f, 1.0f, 0.0f, 1.0f };

	// 회전
	float m_Rotate_Pitch_Value = 0.0f; // x축 회전 변화량
	float m_Rotate_Yaw_Value = 0.0f; // y축 회전 변화량
	float m_Rotate_Roll_Value = 0.0f; // z축 회전 변화량

	// 회전 - 사원수 객체
	Quaternion* m_Quaternion_Result = nullptr;
	Quaternion* m_Quaternion_temp_1 = nullptr;
	Quaternion* m_Quaternion_temp_2 = nullptr;
	Quaternion* m_Quaternion_temp_3 = nullptr;
	
	// 사원수의 결과를 통해 얻은
	// 현재의 회전 행렬
	float m_Rotate_Transform_Matrix[16];


	// 신축
	float m_Scale_X = 1.0f;
	float m_Scale_Y = 1.0f;
	float m_Scale_Z = 1.0f;

	// 색상
	Color4f m_Color;

	// 머티리얼
	Material m_Material;

	// 텍스쳐
	Texture* m_Texture = nullptr;

	// 충돌체
	OBB* m_OBB = nullptr;

	// 조건들
	bool m_is_Destroyed = false; // 제거된 오브젝트 표시. (true가 될 경우 해당 프레임에서 소멸 진행.)
	bool m_is_Activated = false; // 활성화된 오브젝트 표시. (true일 경우에만 Update문을 수행한다.)
	bool m_is_Static_Object = false; // 충돌체크를 하지 않는 오브젝트 표시.
	bool m_is_Auto_Moving = false; // "자동 이동"을 수행할것인가의 조건.
	bool m_is_Move = false;
	bool m_is_Forward_Move = false;
	bool m_is_Backward_Move = false;
	bool m_is_Left_Move = false;
	bool m_is_Right_Move = false;

	int m_Forward_Move_Sign = 1;
	int m_Side_Move_Sign = 1;

	int m_HP;

public: // Getter, Setter 정의구역
	void Set_Index(const int& index) { m_Index = index; }
	int const Get_Index() { return m_Index; }

	void Set_Object_Count(const int& value) { s_Object_Count = value; }
	int const Get_Object_Count() { return s_Object_Count; }

	void Set_Object_Type(const int& value) { m_Object_Type = value; }
	int const Get_Object_Type() { return m_Object_Type; }

	void Set_Translate(const float& x, const float& y, const float& z) { m_Position.x = x; m_Position.y = y; m_Position.z = z; }
	Vector3 const Get_Position() { return m_Position; }
	void Set_Position_X(const float& value) { m_Position.x = value; }
	float const Get_Position_X() { return m_Position.x; }
	void Set_Position_Y(const float& value) { m_Position.y = value; }
	float const Get_Position_Y() { return m_Position.y; }
	void Set_Position_Z(const float& value) { m_Position.z = value; }
	float const Get_Position_Z() { return m_Position.z; }

	virtual void Init_Head_Position() { }
	float* const Get_Head_Position() { return m_Head_Position; }
	float const Get_Head_Position_X() { return m_Head_Position[0]; }
	float const Get_Head_Position_Y() { return m_Head_Position[1]; }
	float const Get_Head_Position_Z() { return m_Head_Position[2]; }

	float* const Get_Forward_Vector() { return m_Forward_Vector; }
	float const Get_Forward_Vector_X() { return m_Forward_Vector[0]; }
	float const Get_Forward_Vector_Y() { return m_Forward_Vector[1]; }
	float const Get_Forward_Vector_Z() { return m_Forward_Vector[2]; }

	float* const Get_Right_Vector() { return m_Right_Vector; }
	float const Get_Right_Vector_X() { return m_Right_Vector[0]; }
	float const Get_Right_Vector_Y() { return m_Right_Vector[1]; }
	float const Get_Right_Vector_Z() { return m_Right_Vector[2]; }

	virtual void Set_Up_Vector() {}
	float* const Get_Up_Vector() { return m_Up_Vector; }
	float const Get_Up_Vector_X() { return m_Up_Vector[0]; }
	float const Get_Up_Vector_Y() { return m_Up_Vector[1]; }
	float const Get_Up_Vector_Z() { return m_Up_Vector[2]; }

	virtual float const Get_Look_Vector(const int& num)
	{
		if (num >= 0 && num <= 2)
			return m_Look_Vector_For_Camera[num];
		return 0;
	}

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
	
	void Set_Texture(const char* image_file_path, const int& width, const int& height)
	{
		if (m_Texture != nullptr)
		{
			delete m_Texture;
			m_Texture = nullptr;
		}
		m_Texture = new Texture();
		m_Texture->Set_Texture(image_file_path, width, height);
	}

	void Set_is_Destroyed(const float& b) { m_is_Destroyed = b; }
	bool const Get_is_Destroyed() { return m_is_Destroyed; }

	void Set_Active(const bool& b) { m_is_Activated = b; }
	bool const Get_is_Activated() { return m_is_Activated; }

	void Set_is_Static(const bool& b) { m_is_Static_Object = b; }
	bool const Get_is_Static() { return m_is_Static_Object; }
	
	void Set_is_Move(const bool& b) { m_is_Move = b; }
	bool const Get_is_Move() { return m_is_Move; }



	void Make_OBB() 
	{ 
		if (m_OBB == nullptr) { delete m_OBB; m_OBB = nullptr; }

		m_OBB = new OBB();
		m_OBB->OBB_Init(m_Position, m_Scale_X, m_Scale_Y, m_Scale_Z);
	}
	virtual OBB* const Get_OBB() { return m_OBB; }
	virtual OBB** const Get_OBBs() { return &m_OBB; }
	virtual OBB* Get_Cube_Collider(const int& index) { return m_OBB; }
	virtual Vector4 const Get_Collider_Normal_Vector(const int& index) { Vector4 temp; return temp; }

	void Set_Pos_to_Collider_Pos()
	{
		m_Position.x = m_OBB->Get_Center_Position()[0];
		m_Position.y = m_OBB->Get_Center_Position()[1];
		m_Position.z = m_OBB->Get_Center_Position()[2];
	}

	void Set_Material_Ambient_Color(const float& r, const float& g, const float& b, const float& a)
	{ 
		m_Material.Ambient[0] = r; m_Material.Ambient[1] = g; m_Material.Ambient[2] = b; m_Material.Ambient[3] = a;
	}
	void Set_Material_Diffuse_Color(const float& r, const float& g, const float& b, const float& a)
	{ 
		m_Material.Diffuse[0] = r; m_Material.Diffuse[1] = g; m_Material.Diffuse[2] = b; m_Material.Diffuse[3] = a;
	}
	void Set_Material_Specular_Color(const float& r, const float& g, const float& b, const float& a)
	{
		m_Material.Specular[0] = r; m_Material.Specular[1] = g; m_Material.Specular[2] = b; m_Material.Specular[3] = a;
	}

	virtual void Control_Ambient_Color(const int& sign)
	{
		m_Material.Ambient[0] += 0.1f * sign; m_Material.Ambient[1] += 0.1f * sign; m_Material.Ambient[2] += 0.1f * sign;
	}
	virtual void Control_Diffuse_Color(const int& sign)
	{
		m_Material.Diffuse[0] += 0.1f * sign; m_Material.Diffuse[1] += 0.1f * sign; m_Material.Diffuse[2] += 0.1f * sign;
	}
	virtual void Control_Specular_Color(const int& sign)
	{
		m_Material.Specular[0] += 0.1f * sign; m_Material.Specular[1] += 0.1f * sign; m_Material.Specular[2] += 0.1f * sign;
	}

	void Forward_Move_On()
	{
		m_is_Move = true;
		m_is_Forward_Move = true;
	}
	void Forward_Move_Off() { m_is_Forward_Move = false; if (!m_is_Backward_Move && !m_is_Left_Move && !m_is_Right_Move) m_is_Move = false; }

	void Backward_Move_On()
	{
		m_is_Move = true;
		m_is_Backward_Move = true;
	}
	void Backward_Move_Off() { m_is_Backward_Move = false; if (!m_is_Forward_Move && !m_is_Left_Move && !m_is_Right_Move) m_is_Move = false; }

	void Left_Move_On()
	{
		m_is_Move = true;
		m_is_Left_Move = true;
	}
	void Left_Move_Off() { m_is_Left_Move = false; if (!m_is_Forward_Move && !m_is_Backward_Move && !m_is_Right_Move) m_is_Move = false; }

	void Right_Move_On()
	{
		m_is_Move = true;
		m_is_Right_Move = true;
	}
	void Right_Move_Off() { m_is_Right_Move = false; if (!m_is_Forward_Move && !m_is_Backward_Move && !m_is_Left_Move) m_is_Move = false; }

	virtual void HP_Reduce()
	{
		m_HP -= 1;
		if (m_HP <= 0) m_is_Destroyed = true;
	}
	virtual int Get_HP() { return m_HP; }


public: // public 메소드 정의구역
	Object();
	virtual ~Object();

	virtual void Draw();

	void Init_Object();
	virtual void Make_Model_Vertice() {};

	virtual void Move() {};
	virtual void Auto_Move();
	void Rotate_Check();
	virtual void Quaternion_Process(Quaternion* quat, float& rot_Value, const int& num);

	virtual void Update();
	virtual bool Collision_Check(Object* other);
	virtual void Collision_Procedure();
	int count = 0;

	void Auto_Moving_Switch()
	{
		m_is_Auto_Moving = m_is_Auto_Moving == true ? false : true;
	}

	Vector3 Get_Normal_Vector(const float& v1_x, const float& v1_y, const float& v1_z, const float& v2_x, const float& v2_y, const float& v2_z);

	void Multiple_Matrix_And_Vector(const float* src_matrix, float* dest_vector);
};

