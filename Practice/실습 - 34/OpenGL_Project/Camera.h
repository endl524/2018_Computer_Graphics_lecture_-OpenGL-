#pragma once
#include "stdafx.h"

class Camera
{
private:
	// 카메라 변환 및 설정에 사용되는 변수들.
	float m_Transform_Matrix[16];
	float m_Position_Vector[4];
	float temp_Position_Vector[4];
	float m_Look_Vector[4];
	float m_Normal_Vector[3];
	float m_Up_Vector[4];

	float m_Rotation_Value[3];

	bool m_is_Zoomming_to_Main = false;

public:
	void Set_Position(const float& x, const float& y, const float& z) { m_Position_Vector[0] = x; m_Position_Vector[1] = y; m_Position_Vector[2] = z; }
	void Set_Position_X(const float& value) { m_Position_Vector[0] = value; }
	void Set_Position_Y(const float& value) { m_Position_Vector[1] = value; }
	void Set_Position_Z(const float& value) { m_Position_Vector[2] = value; }
	float Get_Position_X() { return m_Position_Vector[0]; }
	float Get_Position_Y() { return m_Position_Vector[1]; }
	float Get_Position_Z() { return m_Position_Vector[2]; }

	void Set_Look_At(const float& x, const float& y, const float& z) { m_Look_Vector[0] = x; m_Look_Vector[1] = y; m_Look_Vector[2] = z; }
	void Set_Look_At_X(const float& value) { m_Look_Vector[0] = value; }
	void Set_Look_At_Y(const float& value) { m_Look_Vector[1] = value; }
	void Set_Look_At_Z(const float& value) { m_Look_Vector[2] = value; }
	float Get_Look_At_X() { return m_Look_Vector[0]; }
	float Get_Look_At_Y() { return m_Look_Vector[1]; }
	float Get_Look_At_Z() { return m_Look_Vector[2]; }

	void Set_Rotation_Value(const int& index, const float& value)
	{
		if (index >= 0 && index <= 2) m_Rotation_Value[index] += value;
	}

	void const Set_is_Zoomming_to_Main(const bool& b) { m_is_Zoomming_to_Main = b; }
	bool const Get_is_Zoomming_to_Main() { return m_is_Zoomming_to_Main; }

public:
	Camera();
	~Camera();

	void Init_Camera();

	void Multiple_Mat_And_Vec();
	void Forward_Moving(const float& moving_Value);
	void XY_Moving(const bool& is_Axis_Y, const float& moving_Value);

	void Work();

	bool Zoom_to_Main();
};

