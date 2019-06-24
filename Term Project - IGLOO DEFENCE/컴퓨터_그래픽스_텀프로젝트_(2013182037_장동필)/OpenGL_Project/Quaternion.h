#pragma once
#include "stdafx.h"

class Quaternion
{
private:
	Vector4 m_Result;
	Vector4 m_Normal;

public:
	Vector4 Get_Quaternion() { return m_Result; }
	float Get_X() { return m_Result.x; }
	float Get_Y() { return m_Result.y; }
	float Get_Z() { return m_Result.z; }
	float Get_W() { return m_Result.w; }

	/*
	Vector4 Get_Normal() { return m_Normal; }
	float Get_Normal_X() { return m_Normal.x; }
	float Get_Normal_Y() { return m_Normal.y; }
	float Get_Normal_Z() { return m_Normal.z; }
	float Get_Normal_W() { return m_Normal.w; }
	*/

	void Set_X(const float& value) { m_Result.x = value; }
	void Set_Y(const float& value) { m_Result.y = value; }
	void Set_Z(const float& value) { m_Result.z = value; }
	void Set_W(const float& value) { m_Result.w = value; }

public:
	Quaternion() {};
	~Quaternion() {};

	void Set_By_Angle_Axis(const float& angle, const float& axis_x, const float& axis_y, const float& axis_z)
	{
		const float temp_Angle = angle * 3.1416f / 180.0f;
		const float sin_half = sin(temp_Angle * 0.5f);

		m_Result.x = sin_half * axis_x;
		m_Result.y = sin_half * axis_y;
		m_Result.z = sin_half * axis_z;
		m_Result.w = cos(temp_Angle * 0.5f);

		// 이미 정규화된 (1,0,0), (0,1,0), (0,0,1)을 사용하기 때문에 딱히
		// 또 정규화할 필요가 없다.
		/*
		const float normal_Vector_Size =
			sqrtf(m_Result.x * m_Result.x
				+ m_Result.y * m_Result.y
				+ m_Result.z * m_Result.z
				+ m_Result.w * m_Result.w);

		m_Normal.x = m_Result.x / normal_Vector_Size;
		m_Normal.y = m_Result.y / normal_Vector_Size;
		m_Normal.z = m_Result.z / normal_Vector_Size;
		m_Normal.w = m_Result.w / normal_Vector_Size;
		*/
	}
	
	void Multiple(Quaternion* multiplier)
	{
		Quaternion q = *this;
		m_Result.w = q.Get_W() * multiplier->Get_W() - q.Get_X() * multiplier->Get_X()
			- q.Get_Y() * multiplier->Get_Y() - q.Get_Z() * multiplier->Get_Z();
		m_Result.x = q.Get_W() * multiplier->Get_X() + q.Get_X() * multiplier->Get_W()
			+ q.Get_Y() * multiplier->Get_Z() - q.Get_Z() * multiplier->Get_Y();
		m_Result.y = q.Get_W() * multiplier->Get_Y() + q.Get_Y() * multiplier->Get_W()
			+ q.Get_Z() * multiplier->Get_X() - q.Get_X() * multiplier->Get_Z();
		m_Result.z = q.Get_W() * multiplier->Get_Z() + q.Get_Z() * multiplier->Get_W()
			+ q.Get_X() * multiplier->Get_Y() - q.Get_Y() * multiplier->Get_X();
	}

	void Get_Transform_Matrix(float* mat)
	{
		mat[0] = 1.0f - 2.0f * (m_Result.y * m_Result.y + m_Result.z * m_Result.z);
		mat[1] = 2.0f * (m_Result.x * m_Result.y - m_Result.w * m_Result.z);
		mat[2] = 2.0f * (m_Result.x * m_Result.z + m_Result.w * m_Result.y);
		mat[3] = 0.0f;

		mat[4] = 2.0f * (m_Result.x * m_Result.y + m_Result.w * m_Result.z);
		mat[5] = 1.0f - 2.0f * (m_Result.x * m_Result.x + m_Result.z * m_Result.z);
		mat[6] = 2.0f * (m_Result.y * m_Result.z - m_Result.w * m_Result.x);
		mat[7] = 0.0f;

		mat[8] = 2.0f * (m_Result.x * m_Result.z - m_Result.w * m_Result.y);
		mat[9] = 2.0f * (m_Result.y * m_Result.z + m_Result.w * m_Result.x);
		mat[10] = 1.0f - 2.0f * (m_Result.x * m_Result.x + m_Result.y * m_Result.y);
		mat[11] = 0.0f;

		mat[12] = 0.0f;
		mat[13] = 0.0f;
		mat[14] = 0.0f;
		mat[15] = 1.0f;
	}
	
};