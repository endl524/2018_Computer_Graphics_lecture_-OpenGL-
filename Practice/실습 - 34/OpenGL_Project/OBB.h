#pragma once
#include "stdafx.h"

class OBB
{
private:
	float m_Center_Position[4];
	Vector4 m_OBB_Axis_Dir[4];
	float m_OBB_Axis_Distance[3];

public:
	void Set_Center_Position(const Vector3& pos) 
	{ m_Center_Position[0] = pos.x; m_Center_Position[1] = pos.y; m_Center_Position[2] = pos.z; }
	float* Get_Center_Position() { return m_Center_Position; }


	Vector4* Get_OBB_Axis_Dir() { return m_OBB_Axis_Dir; }


	float* Get_OBB_Axis_Distance() { return m_OBB_Axis_Distance; }

public:
	OBB() {};
	~OBB() {};

	void OBB_Init(const Vector3& pos, const float& scale_x, const float& scale_y, const float& scale_z)
	{
		m_Center_Position[0] = pos.x;
		m_Center_Position[1] = pos.y;
		m_Center_Position[2] = pos.z;
		m_Center_Position[3] = 0.0f;

		m_OBB_Axis_Dir[0].x = 1.0f;
		m_OBB_Axis_Dir[0].y = 0.0f;
		m_OBB_Axis_Dir[0].z = 0.0f;
		m_OBB_Axis_Dir[0].w = 1.0f;

		m_OBB_Axis_Dir[1].x = 0.0f;
		m_OBB_Axis_Dir[1].y = 1.0f;
		m_OBB_Axis_Dir[1].z = 0.0f;
		m_OBB_Axis_Dir[1].w = 1.0f;

		m_OBB_Axis_Dir[2].x = 0.0f;
		m_OBB_Axis_Dir[2].y = 0.0f;
		m_OBB_Axis_Dir[2].z = 1.0f;
		m_OBB_Axis_Dir[2].w = 1.0f;

		m_OBB_Axis_Distance[0] = OBJECT_SIZE * scale_x;
		m_OBB_Axis_Distance[1] = OBJECT_SIZE * scale_y;
		m_OBB_Axis_Distance[2] = OBJECT_SIZE * scale_z;
	}



	bool OBB_Intersection(OBB* other)
	{
		float distance[3] =
		{
			other->Get_Center_Position()[0] - m_Center_Position[0],
			other->Get_Center_Position()[1] - m_Center_Position[1],
			other->Get_Center_Position()[2] - m_Center_Position[2]
		};
		float matrix[3][3];
		float absMatrix[3][3];
		float ad[3];
		float R_0, R_1, R_ADD, R;

		// A0 ~ A2
		for (int i = 0; i < 3; ++i)
		{
			Vector_DotProduct(matrix[i][0], m_OBB_Axis_Dir[i], other->Get_OBB_Axis_Dir()[0]);
			Vector_DotProduct(matrix[i][1], m_OBB_Axis_Dir[i], other->Get_OBB_Axis_Dir()[1]);
			Vector_DotProduct(matrix[i][2], m_OBB_Axis_Dir[i], other->Get_OBB_Axis_Dir()[2]);
			Vector_DotProduct(ad[i], m_OBB_Axis_Dir[i], distance);
			absMatrix[i][0] = fabsf(matrix[i][0]); absMatrix[i][1] = fabsf(matrix[i][1]); absMatrix[i][2] = fabsf(matrix[i][2]);

			R = fabsf(ad[i]);
			R_1 = other->Get_OBB_Axis_Distance()[0] * absMatrix[i][0] + other->Get_OBB_Axis_Distance()[1] * absMatrix[i][1] + other->Get_OBB_Axis_Distance()[2] * absMatrix[i][2];
			R_ADD = m_OBB_Axis_Distance[i] + R_1;
			if (R > R_ADD) return false;
		}

		// B0 ~ B2
		for (int i = 0; i < 3; ++i)
		{
			Vector_DotProduct(R, other->Get_OBB_Axis_Dir()[i], distance); R = fabsf(R);
			R_0 = m_OBB_Axis_Distance[0] * absMatrix[0][i] + m_OBB_Axis_Distance[1] * absMatrix[1][i] + m_OBB_Axis_Distance[2] * absMatrix[2][i];
			R_ADD = R_0 + other->Get_OBB_Axis_Distance()[i];
			if (R > R_ADD) return false;
		}

		// A0 x B0
		R = fabsf(ad[2] * matrix[1][0] - ad[1] * matrix[2][0]);
		R_0 = m_OBB_Axis_Distance[1] * absMatrix[2][0] + m_OBB_Axis_Distance[2] * absMatrix[1][0];
		R_1 = other->Get_OBB_Axis_Distance()[1] * absMatrix[0][2] + other->Get_OBB_Axis_Distance()[2] * absMatrix[0][1];
		R_ADD = R_0 + R_1; if (R > R_ADD) return false;

		// A0 x B1
		R = fabsf(ad[2] * matrix[1][1] - ad[1] * matrix[2][1]);
		R_0 = m_OBB_Axis_Distance[1] * absMatrix[2][1] + m_OBB_Axis_Distance[2] * absMatrix[1][1];
		R_1 = other->Get_OBB_Axis_Distance()[0] * absMatrix[0][2] + other->Get_OBB_Axis_Distance()[2] * absMatrix[0][0];
		R_ADD = R_0 + R_1; if (R > R_ADD) return false;

		// A0 x B2
		R = fabsf(ad[2] * matrix[1][2] - ad[1] * matrix[2][2]);
		R_0 = m_OBB_Axis_Distance[1] * absMatrix[2][2] + m_OBB_Axis_Distance[2] * absMatrix[1][2];
		R_1 = other->Get_OBB_Axis_Distance()[0] * absMatrix[0][1] + other->Get_OBB_Axis_Distance()[1] * absMatrix[0][0];
		R_ADD = R_0 + R_1; if (R > R_ADD) return false;

		// A1 x B0
		R = fabsf(ad[0] * matrix[2][0] - ad[2] * matrix[0][0]);
		R_0 = m_OBB_Axis_Distance[0] * absMatrix[2][0] + m_OBB_Axis_Distance[2] * absMatrix[0][0];
		R_1 = other->Get_OBB_Axis_Distance()[1] * absMatrix[1][2] + other->Get_OBB_Axis_Distance()[2] * absMatrix[1][1];
		R_ADD = R_0 + R_1; if (R > R_ADD) return false;

		// A1 x B1
		R = fabsf(ad[0] * matrix[2][1] - ad[2] * matrix[0][1]);
		R_0 = m_OBB_Axis_Distance[0] * absMatrix[2][1] + m_OBB_Axis_Distance[2] * absMatrix[0][1];
		R_1 = other->Get_OBB_Axis_Distance()[0] * absMatrix[1][2] + other->Get_OBB_Axis_Distance()[2] * absMatrix[1][0];
		R_ADD = R_0 + R_1; if (R > R_ADD) return false;

		// A1 x B2
		R = fabsf(ad[0] * matrix[2][2] - ad[2] * matrix[0][2]);
		R_0 = m_OBB_Axis_Distance[0] * absMatrix[2][2] + m_OBB_Axis_Distance[2] * absMatrix[0][2];
		R_1 = other->Get_OBB_Axis_Distance()[0] * absMatrix[1][1] + other->Get_OBB_Axis_Distance()[1] * absMatrix[1][0];
		R_ADD = R_0 + R_1; if (R > R_ADD) return false;

		// A2 x B0
		R = fabsf(ad[1] * matrix[0][0] - ad[0] * matrix[1][0]);
		R_0 = m_OBB_Axis_Distance[0] * absMatrix[1][0] + m_OBB_Axis_Distance[1] * absMatrix[0][0];
		R_1 = other->Get_OBB_Axis_Distance()[1] * absMatrix[2][2] + other->Get_OBB_Axis_Distance()[2] * absMatrix[2][1];
		R_ADD = R_0 + R_1; if (R > R_ADD) return false;

		// A2 x B1
		R = fabsf(ad[1] * matrix[0][1] - ad[0] * matrix[1][1]);
		R_0 = m_OBB_Axis_Distance[0] * absMatrix[1][1] + m_OBB_Axis_Distance[1] * absMatrix[0][1];
		R_1 = other->Get_OBB_Axis_Distance()[0] * absMatrix[2][2] + other->Get_OBB_Axis_Distance()[2] * absMatrix[2][0];
		R_ADD = R_0 + R_1; if (R > R_ADD) return false;

		// A2 x B2
		R = fabsf(ad[1] * matrix[0][2] - ad[0] * matrix[1][2]);
		R_0 = m_OBB_Axis_Distance[0] * absMatrix[1][2] + m_OBB_Axis_Distance[1] * absMatrix[0][2];
		R_1 = other->Get_OBB_Axis_Distance()[0] * absMatrix[2][1] + other->Get_OBB_Axis_Distance()[1] * absMatrix[2][0];
		R_ADD = R_0 + R_1; if (R > R_ADD) return false;

		return true;
	}


	void Multiply_Rotate_Matrix_to_OBB_Axis(float* rot_Transform)
	{
		float temp_Float = 0.0f;
		float temp_Vector[4];
		float Vector[4];

		for (int k = 0; k < 3; ++k)
		{
			Vector[0] = m_OBB_Axis_Dir[k].x;
			Vector[1] = m_OBB_Axis_Dir[k].y;
			Vector[2] = m_OBB_Axis_Dir[k].z;
			Vector[3] = m_OBB_Axis_Dir[k].w;

			for (int i = 0; i < 4; ++i)
			{
				temp_Float = 0.0f;
				for (int j = 0; j < 4; ++j) temp_Float += rot_Transform[i + j * 4] * Vector[j];
				temp_Vector[i] = temp_Float;
			}

			m_OBB_Axis_Dir[k].x = temp_Vector[0];
			m_OBB_Axis_Dir[k].y = temp_Vector[1];
			m_OBB_Axis_Dir[k].z = temp_Vector[2];
			m_OBB_Axis_Dir[k].w = temp_Vector[3];
		}
		
	}

	void Multiply_Rotate_Matrix_to_OBB_Position(float* transform)
	{
		float temp_Float = 0.0f;
		float temp_Vector[4];
		float Vector[4];

		Vector[0] = m_Center_Position[0];
		Vector[1] = m_Center_Position[1];
		Vector[2] = m_Center_Position[2];
		Vector[3] = m_Center_Position[3];

		for (int i = 0; i < 4; ++i)
		{
			temp_Float = 0.0f;
			for (int j = 0; j < 4; ++j) temp_Float += transform[i + j * 4] * Vector[j];
			temp_Vector[i] = temp_Float;
		}

		for (int i = 0; i < 4; ++i) m_Center_Position[i] = temp_Vector[i];
	}

	
	void Vector_CrossProduct(Vector3& result, const Vector4& p1, const Vector4& p2, const Vector4& p3)
	{
		Vector3 temp_Vector_1;
		Vector3 temp_Vector_2;
		float temp_Length;

		temp_Vector_1.x = p2.x - p1.x;
		temp_Vector_1.y = p2.y - p1.y;
		temp_Vector_1.z = p2.z - p1.z;

		temp_Vector_2.x = p3.x - p2.x;
		temp_Vector_2.y = p3.y - p2.y;
		temp_Vector_2.z = p3.z - p2.z;

		result.x = (temp_Vector_1.y * temp_Vector_2.z - temp_Vector_1.z * temp_Vector_2.y);
		result.y = -(temp_Vector_1.x * temp_Vector_2.z - temp_Vector_1.z * temp_Vector_2.x);
		result.z = (temp_Vector_1.x * temp_Vector_2.y - temp_Vector_2.x * temp_Vector_1.y);


		temp_Length = sqrtf(result.x * result.x + result.y * result.y + result.z * result.z);
		result.x /= temp_Length;
		result.y /= temp_Length;
		result.z /= temp_Length;

	}

	void Vector_DotProduct(float& result, const Vector4& v0, const Vector4& v1)
	{
		result = v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	}

	void Vector_DotProduct(float& result, const Vector4& v0, const float* v1)
	{
		result = v0.x * v1[0] + v0.y * v1[1] + v0.z * v1[2];
	}

	void Vector_DotProduct(float& result, const float* v0, const float* v1)
	{
		result = v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
	}
};

