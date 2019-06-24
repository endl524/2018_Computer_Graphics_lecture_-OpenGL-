#pragma once
#include "stdafx.h"

class Camera
{
private:
	float m_Position_X;
	float m_Position_Y;
	float m_Position_Z;

	float m_Look_At_X;
	float m_Look_At_Y;
	float m_Look_At_Z;

	float m_Rotate_Roll; // zÃà
	float m_Rotate_Yaw; // yÃà
	float m_Rotate_Pitch; // xÃà


public:
	void Set_Position(const float& x, const float& y, const float& z) { m_Position_X = x; m_Position_Y = y; m_Position_Z = z; }
	void Set_Position_X(const float& value) { m_Position_X = value; }
	void Set_Position_Y(const float& value) { m_Position_Y = value; }
	void Set_Position_Z(const float& value) { m_Position_Z = value; }
	float Get_Position_X() { return m_Position_X; }
	float Get_Position_Y() { return m_Position_Y; }
	float Get_Position_Z() { return m_Position_Z; }

	void Set_Look_At(const float& x, const float& y, const float& z) { m_Look_At_X = x; m_Look_At_Y = y; m_Look_At_Z = z; }
	void Set_Look_At_X(const float& value) { m_Look_At_X = value; }
	void Set_Look_At_Y(const float& value) { m_Look_At_Y = value; }
	void Set_Look_At_Z(const float& value) { m_Look_At_Z = value; }
	float Get_Look_At_X() { return m_Look_At_X; }
	float Get_Look_At_Y() { return m_Look_At_Y; }
	float Get_Look_At_Z() { return m_Look_At_Z; }

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


public:
	Camera();
	~Camera();

	void Init_Camera();

	void Work();
};

