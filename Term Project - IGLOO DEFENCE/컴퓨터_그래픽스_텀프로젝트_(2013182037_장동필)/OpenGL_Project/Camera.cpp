#include "Camera.h"


Camera::Camera()
{
	Init_Camera();
}

Camera::~Camera()
{
	if (m_Target != nullptr)
	{
		m_Target = nullptr;
	}

	if (m_Quaternion_Result != nullptr)
	{
		delete m_Quaternion_Result;
		m_Quaternion_Result = nullptr;
	}

	if (m_Quaternion_temp_1 != nullptr)
	{
		delete m_Quaternion_temp_1;
		m_Quaternion_temp_1 = nullptr;
	}

	if (m_Quaternion_temp_2 != nullptr)
	{
		delete m_Quaternion_temp_2;
		m_Quaternion_temp_2 = nullptr;
	}

	if (m_Quaternion_temp_3 != nullptr)
	{
		delete m_Quaternion_temp_3;
		m_Quaternion_temp_3 = nullptr;
	}
}

void Camera::Init_Camera()
{
	m_Quaternion_Result = new Quaternion();
	m_Quaternion_temp_1 = new Quaternion();
	m_Quaternion_temp_2 = new Quaternion();
	m_Quaternion_temp_3 = new Quaternion();

	Top_View();

	m_Rotation_Value[0] = 0.0f;
	m_Rotation_Value[1] = 0.0f;
	m_Rotation_Value[2] = 0.0f;

	Calc_Direction_Vector(0); Calc_Direction_Vector(1); Calc_Direction_Vector(2);

	m_Speed = 10.0f;

	for (int i = 0; i < 16; ++i) m_Rotate_Transform_Matrix[i] = 0;
	m_Rotate_Transform_Matrix[0] = 1.0f; m_Rotate_Transform_Matrix[5] = 1.0f; m_Rotate_Transform_Matrix[10] = 1.0f; m_Rotate_Transform_Matrix[15] = 1.0f;
}

void Camera::Rotate_Check()
{
	if (m_Rotate_Pitch_Value != 0.0f) Quaternion_Process(m_Quaternion_temp_1, m_Rotate_Pitch_Value, 1);
	if (m_Rotate_Yaw_Value != 0.0f) Quaternion_Process(m_Quaternion_temp_2, m_Rotate_Yaw_Value, 2);
	if (m_Rotate_Roll_Value != 0.0f) Quaternion_Process(m_Quaternion_temp_3, m_Rotate_Roll_Value, 3);

	m_Quaternion_Result->Get_Transform_Matrix(m_Rotate_Transform_Matrix);
}

void Camera::Quaternion_Process(Quaternion* quat, float& rot_Value, const int& num)
{
	if (num == 1) quat->Set_By_Angle_Axis(rot_Value, 1.0f, 0.0f, 0.0f);
	else if (num == 2) quat->Set_By_Angle_Axis(rot_Value,0.0f,1.0f,0.0f);
	else if (num == 3) quat->Set_By_Angle_Axis(rot_Value, 0.0f,0.0f,1.0f);
	else return;
	rot_Value = 0.0f;
	m_Quaternion_Result->Multiple(quat);

	float temp_Transform[16] = { 0.0f, };
	quat->Get_Transform_Matrix(temp_Transform);
	Multiple_Matrix_And_Vector(temp_Transform, m_Forward_Vector);
	Multiple_Matrix_And_Vector(temp_Transform, m_Right_Vector);
	Multiple_Matrix_And_Vector(temp_Transform, m_Up_Vector);
}

void Camera::Multiple_Matrix_And_Vector(const float* src_matrix, float* dest_vector)
{
	float temp_Float = 0.0f;
	float temp_Vector[4];

	for (int i = 0; i < 4; ++i)
	{
		temp_Float = 0.0f;
		for (int j = 0; j < 4; ++j) temp_Float += src_matrix[i + j * 4] * dest_vector[j];
		temp_Vector[i] = temp_Float;
	}

	for (int i = 0; i < 4; ++i) dest_vector[i] = temp_Vector[i];
}

void Camera::Calc_Direction_Vector(const int& type)
{
	float position_Vectors_Size = 0.0f;

	switch (type)
	{
	case 0: // Forward_Vector
		// 1. 카메라의 주시 방향에 대한 벡터를 구한다.
		for (int i = 0; i < 3; ++i) m_Normal_Vector[i] = m_Look_Vector[i] - m_Position_Vector[i];

		// 2. 구한 벡터의 크기를 구한다.
		position_Vectors_Size =
			sqrtf(m_Normal_Vector[0] * m_Normal_Vector[0]
				+ m_Normal_Vector[1] * m_Normal_Vector[1]
				+ m_Normal_Vector[2] * m_Normal_Vector[2]);

		m_Forward_Vector[0] = m_Normal_Vector[0] / position_Vectors_Size;
		m_Forward_Vector[1] = m_Normal_Vector[1] / position_Vectors_Size;
		m_Forward_Vector[2] = m_Normal_Vector[2] / position_Vectors_Size;
		break;

	case 1: // Right_Vector
		m_Right_Vector[0] = m_Forward_Vector[2]; m_Right_Vector[1] = 0.0f; m_Right_Vector[2] = -m_Forward_Vector[0];
		break;

	case 2: // Up_Vector
		Vector_CrossProduct(m_Up_Vector, m_Forward_Vector, m_Right_Vector);
		break;
	}
	
}

void Camera::Forward_Moving(const int& sign)
{
	if (m_is_Free_View)
	{
		if (m_Target != nullptr)
		{
			Calc_Direction_Vector(0);
			for (int i = 0; i < 3; ++i)
			{
				m_Position_Vector[i] += m_Forward_Vector[i] * m_Speed * sign;
				m_Look_Vector[i] += m_Forward_Vector[i] * m_Speed * sign;
			}
		}
		else
		{
			for (int i = 0; i < 3; ++i)
			{
				m_Position_Vector[i] += m_Forward_Vector[i] * m_Speed * sign;
			}
			m_Look_Vector[0] = 0.0f;
			m_Look_Vector[1] = 0.0f;
			m_Look_Vector[2] = -1.0f;
			m_Look_Vector[3] = 1.0f;
		}
	}
	else if (m_is_Top_View || m_is_Front_View)
	{
		m_Distance += m_Speed * sign;
	}
}

void Camera::Right_Moving(const int& sign)
{
	if (m_Target == nullptr)
	{
		for (int i = 0; i < 3; ++i)
		{
			m_Position_Vector[i] += m_Right_Vector[i] * m_Speed * sign;
			m_Look_Vector[i] += m_Right_Vector[i] * m_Speed * sign;
		}
	}
	else 
	{
		for (int i = 0; i < 3; ++i)
		{
			m_Position_Vector[i] += m_Right_Vector[i] * m_Speed * sign;
		}
		m_Look_Vector[0] = 0.0f;
		m_Look_Vector[1] = 0.0f;
		m_Look_Vector[2] = -1.0f;
		m_Look_Vector[3] = 1.0f;
	}
}


void Camera::Work()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	Look();
}

bool Camera::Zoom_to_Main()
{
	bool is_X_end = false;
	bool is_Y_end = false;
	bool is_Z_end = false;

	if (m_Position_Vector[0] > 20.0f) { m_Position_Vector[0] -= 20.0f; }
	else if (m_Position_Vector[0] < -20.0f) { m_Position_Vector[0] += 20.0f; }
	else { m_Position_Vector[0] = 0.0f; is_X_end = true; }

	if (m_Position_Vector[1] > 620.0f) { m_Position_Vector[1] -= 20.0f; }
	else if (m_Position_Vector[1] < -620.0f) { m_Position_Vector[1] += 20.0f; }
	else { m_Position_Vector[1] = 600.0f; is_Y_end = true; }

	if (m_Position_Vector[2] > 720.0f) { m_Position_Vector[2] -= 20.0f; }
	else if (m_Position_Vector[2] < -720.0f) { m_Position_Vector[2] += 20.0f; }
	else { m_Position_Vector[2] = 700.0f; is_Z_end = true; }

	if (is_X_end && is_Y_end && is_Z_end)
	{
		Calc_Direction_Vector(0); Calc_Direction_Vector(1); Calc_Direction_Vector(2);
		return true;
	}
	return false;
}

void Camera::Top_View()
{
	m_Distance = 1.0f;

	m_Position_Vector[0] = 0.0f;
	m_Position_Vector[1] = 500.0f;
	m_Position_Vector[2] = 0.0f;
	m_Position_Vector[3] = 1.0f;

	m_Look_Vector[0] = 0.0f;
	m_Look_Vector[1] = 0.0f;
	m_Look_Vector[2] = -1.0f;
	m_Look_Vector[3] = 1.0f;

	m_is_Top_View = true;
	m_is_Front_View = false;
	m_is_Free_View = false;

	Calc_Direction_Vector(0); Calc_Direction_Vector(1); Calc_Direction_Vector(2);
}

void Camera::Front_View()
{
	m_Distance = 1.0f;

	m_Position_Vector[0] = 0.0f;
	m_Position_Vector[1] = 10.0f;
	m_Position_Vector[2] = 200.0f;
	m_Position_Vector[3] = 1.0f;

	m_Look_Vector[0] = 0.0f;
	m_Look_Vector[1] = 0.0f;
	m_Look_Vector[2] = -1.0f;
	m_Look_Vector[3] = 1.0f;

	m_is_Top_View = false;
	m_is_Front_View = true;
	m_is_Free_View = false;

	Calc_Direction_Vector(0); Calc_Direction_Vector(1); Calc_Direction_Vector(2);
}

void Camera::Free_View()
{
	m_is_Top_View = false;
	m_is_Front_View = false;
	m_is_Free_View = true;

	m_Look_Vector[0] = 0.0f;
	m_Look_Vector[1] = -OBJECT_SIZE;
	m_Look_Vector[2] = -1.0f;
	m_Look_Vector[3] = 1.0f;

	Calc_Direction_Vector(0); Calc_Direction_Vector(1); Calc_Direction_Vector(2);
}

void Camera::Look()
{
	if (m_Target != nullptr)
	{
		if (m_is_FPS)
		{
			m_Position_Vector[0] = m_Target->Get_Head_Position_X() + m_Target->Get_Forward_Vector_X() * 10.0f;
			m_Position_Vector[1] = m_Target->Get_Head_Position_Y() + m_Target->Get_Forward_Vector_Y() * 10.0f + m_Target->Get_Up_Vector_Y();
			m_Position_Vector[2] = m_Target->Get_Head_Position_Z() + m_Target->Get_Forward_Vector_Z() * 10.0f;
			m_Look_Vector[0] = m_Target->Get_Head_Position_X() + m_Target->Get_Forward_Vector_X() * 11.0f;
			m_Look_Vector[1] = m_Target->Get_Head_Position_Y() + m_Target->Get_Forward_Vector_Y() * 11.0f;
			m_Look_Vector[2] = m_Target->Get_Head_Position_Z() + m_Target->Get_Forward_Vector_Z() * 11.0f;
		}
		else
		{
			m_Position_Vector[0] = m_Target->Get_Head_Position_X() + m_Target->Get_Forward_Vector_X() * -150.0f;
			m_Position_Vector[1] = m_Target->Get_Head_Position_Y() + m_Target->Get_Forward_Vector_Y() * -150.0f + m_Target->Get_Up_Vector_Y() * 150.0f;
			m_Position_Vector[2] = m_Target->Get_Head_Position_Z() + m_Target->Get_Forward_Vector_Z() * -150.0f;

			m_Look_Vector[0] = m_Target->Get_Head_Position_X() + m_Target->Get_Forward_Vector_X();
			m_Look_Vector[1] = m_Target->Get_Head_Position_Y() + m_Target->Get_Forward_Vector_Y();
			m_Look_Vector[2] = m_Target->Get_Head_Position_Z() + m_Target->Get_Forward_Vector_Z();
		}

		Calc_Direction_Vector(0); Calc_Direction_Vector(1); Calc_Direction_Vector(2);
		
		gluLookAt(
			m_Position_Vector[0], m_Position_Vector[1], m_Position_Vector[2],
			m_Look_Vector[0], m_Look_Vector[1], m_Look_Vector[2],
			m_Up_Vector[0], m_Up_Vector[1], m_Up_Vector[2]
		);
		
	}

	else
	{
		if (m_is_Free_View)
		{
			glPushMatrix();
			glRotatef(m_Rotation_Value[0], 1.0f, 0.0f, 0.0f);
			glRotatef(m_Rotation_Value[1], 0.0f, 1.0f, 0.0f);
			glRotatef(m_Rotation_Value[2], 0.0f, 0.0f, 1.0f);
			glGetFloatv(GL_MODELVIEW_MATRIX, m_Rotate_Transform_Matrix_World); // 통합 회전행렬을 받아온다.

			glPopMatrix();

			m_Rotation_Value[0] = 0.0f;
			m_Rotation_Value[1] = 0.0f;
			m_Rotation_Value[2] = 0.0f;

			Multiple_Matrix_And_Vector(m_Rotate_Transform_Matrix_World, m_Position_Vector);

			Calc_Direction_Vector(0); Calc_Direction_Vector(1); Calc_Direction_Vector(2);

			gluLookAt(
				m_Position_Vector[0], m_Position_Vector[1], m_Position_Vector[2],
				m_Look_Vector[0], m_Look_Vector[1], m_Look_Vector[2],
				0.0f, 1.0f, 0.0f
			);
		}
		else if (m_is_Top_View)
		{
			glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
			glScalef(0.5f, 0.5f, 0.5f);
		}
		else if (m_is_Front_View)
		{
			glScalef(0.5f, 0.5f, 0.5f);
		}
	}

	
}

void Camera::Vector_CrossProduct(float* result, const float* p1, const float* p2)
{
	result[0] = p1[1] * p2[2] - p1[2] * p2[1];
	result[1] = p1[2] * p2[0] - p1[0] * p2[2];
	result[2] = p1[0] * p2[1] - p1[1] * p2[0];
}