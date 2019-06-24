#pragma once
#include "stdafx.h"
#include "Object.h"

class Camera
{
private:
	float m_Rotate_Transform_Matrix[16]; // 자전 행렬
	float m_Rotate_Transform_Matrix_World[16]; // 공전 행렬
	float m_Position_Vector[4];
	float m_Look_Vector[4];
	float m_Normal_Vector[3];

	float m_Forward_Vector[4]{ 0.0f, 0.0f, -1.0f, 1.0f };
	float m_Right_Vector[4]{ 1.0f, 0.0f, 0.0f, 1.0f };
	float m_Up_Vector[4]{ 0.0f, 1.0f, 0.0f, 1.0f };

	float m_Rotation_Value[3];

	float m_Speed;
	bool m_is_Zoomming_to_Main = false;

	Object* m_Target = nullptr;
	float m_Distance = 0.0f;
	bool m_is_Top_View = true;
	bool m_is_Front_View = true;
	bool m_is_Free_View = false;
	bool m_is_FPS = false;

	Quaternion* m_Quaternion_Result = nullptr;
	Quaternion* m_Quaternion_temp_1 = nullptr;
	Quaternion* m_Quaternion_temp_2 = nullptr;
	Quaternion* m_Quaternion_temp_3 = nullptr;
	float m_Rotate_Pitch_Value = 0.0f; // x축 회전 변화량
	float m_Rotate_Yaw_Value = 0.0f; // y축 회전 변화량
	float m_Rotate_Roll_Value = 0.0f; // z축 회전 변화량

	float m_Yaw_Rotate_Stack = 0.0f;

public:
	void Set_Position(const float& x, const float& y, const float& z) { m_Position_Vector[0] = x; m_Position_Vector[1] = y; m_Position_Vector[2] = z; }
	void Set_Position_X(const float& value) { m_Position_Vector[0] = value; }
	void Set_Position_Y(const float& value) { m_Position_Vector[1] = value; }
	void Set_Position_Z(const float& value) { m_Position_Vector[2] = value; }
	void Plus_Position(const float& x, const float& y, const float& z) { m_Position_Vector[0] += x; m_Position_Vector[1] += y; m_Position_Vector[2] += z; }
	float const Get_Position_X() { return m_Position_Vector[0]; }
	float const Get_Position_Y() { return m_Position_Vector[1]; }
	float const Get_Position_Z() { return m_Position_Vector[2]; }

	void Set_Look_At(const float& x, const float& y, const float& z) { m_Look_Vector[0] = x; m_Look_Vector[1] = y; m_Look_Vector[2] = z; }
	void Set_Look_At_X(const float& value) { m_Look_Vector[0] = value; }
	void Set_Look_At_Y(const float& value) { m_Look_Vector[1] = value; }
	void Set_Look_At_Z(const float& value) { m_Look_Vector[2] = value; }
	void Plus_Look(const float& x, const float& y, const float& z) { m_Look_Vector[0] += x; m_Look_Vector[1] += y; m_Look_Vector[2] += z; }
	float const Get_Look_At_X() { return m_Look_Vector[0]; }
	float const Get_Look_At_Y() { return m_Look_Vector[1]; }
	float const Get_Look_At_Z() { return m_Look_Vector[2]; }

	void Set_Rotation_Value(const int& index, const float& value)
	{
		if (index >= 0 && index <= 2) m_Rotation_Value[index] += value;
	}

	void Set_is_Zoomming_to_Main(const bool& b) { m_is_Zoomming_to_Main = b; }
	bool const Get_is_Zoomming_to_Main() { return m_is_Zoomming_to_Main; }

	void Set_is_FPS(const bool& b) { m_is_FPS = b; }
	bool const Get_is_FPS() { return m_is_FPS; }
	void Switching_View_Dimension() { m_is_FPS = m_is_FPS == true ? false : true; }
	void Set_Target(Object* obj) { m_Target = obj; }
	Object* Get_Target() { return m_Target; }
	void Release_Target() 
	{ 
		m_Look_Vector[0] = 0.0f; m_Look_Vector[1] = 0.0f; m_Look_Vector[2] = -1.0f; m_Look_Vector[3] = 1.0f;
		Calc_Direction_Vector(0); Calc_Direction_Vector(1); Calc_Direction_Vector(2);
		m_Target = nullptr; 
	}

	void Pitch(const float& value) { m_Rotate_Pitch_Value = value; Rotate_Check(); }
	void Yaw(const float& value) { m_Rotate_Yaw_Value = value; Rotate_Check(); }
	void Roll(const float& value) { m_Rotate_Roll_Value = value; Rotate_Check(); }


	bool Get_is_Free_View() { return m_is_Free_View; }
	bool Get_is_Top_View() { return m_is_Top_View; }
	bool Get_is_Front_View() { return m_is_Front_View; }

public:
	Camera();
	~Camera();

	void Init_Camera();

	void Multiple_Matrix_And_Vector(const float* src_matrix, float* dest_vector);
	void Calc_Direction_Vector(const int& type);
	void Forward_Moving(const int& sign);
	void Right_Moving(const int& sign);

	void Work();

	bool Zoom_to_Main();


	void Top_View();
	void Front_View();
	void Free_View();

	void Look();

	void Rotate_Check();
	void Quaternion_Process(Quaternion* quat, float& rot_Value, const int& num);
	void Vector_CrossProduct(float* result, const float* p1, const float* p2);

};

