#include "Camera.h"


Camera::Camera()
{
	Init_Camera();
}


Camera::~Camera()
{
}

void Camera::Init_Camera()
{
	m_Position_Vector[0] = 0.0f;
	m_Position_Vector[1] = 50.0f;
	m_Position_Vector[2] = 300.0f;
	m_Position_Vector[3] = 1.0f;

	m_Look_Vector[0] = 0.0f;
	m_Look_Vector[1] = 0.0f;
	m_Look_Vector[2] = -1.0f;

	m_Up_Vector[0] = 0.0f;
	m_Up_Vector[1] = 1.0f;
	m_Up_Vector[2] = 0.0f;

	m_Rotation_Value[0] = 0.0f;
	m_Rotation_Value[1] = 0.0f;
	m_Rotation_Value[2] = 0.0f;
}

void Camera::Multiple_Mat_And_Vec()
{
	float temp_Float = 0.0f;

	for (int i = 0; i < 4; ++i)
	{
		temp_Float = 0.0f;
		for (int j = 0; j < 4; ++j) temp_Float += m_Transform_Matrix[i + j * 4] * m_Position_Vector[j];
		temp_Position_Vector[i] = temp_Float;
	}

	for (int i = 0; i < 4; ++i) m_Position_Vector[i] = temp_Position_Vector[i];
}

void Camera::Forward_Moving(const float& moving_Value)
{
	float position_Vectors_Size = 0.0f;

	// 1. 카메라의 주시 방향에 대한 벡터를 구한다.
	for (int i = 0; i < 3; ++i) m_Normal_Vector[i] = m_Position_Vector[i] - m_Look_Vector[i];

	// 2. 구한 벡터의 크기를 구한다.
	position_Vectors_Size =
		sqrtf(m_Normal_Vector[0] * m_Normal_Vector[0]
			+ m_Normal_Vector[1] * m_Normal_Vector[1]
			+ m_Normal_Vector[2] * m_Normal_Vector[2]);

	// 3. 구한 벡터를 정규화하여 방향을 구한 뒤, 이동할 크기를 곱해준다. 
	m_Normal_Vector[0] = m_Normal_Vector[0] / position_Vectors_Size * moving_Value;
	m_Normal_Vector[1] = m_Normal_Vector[1] / position_Vectors_Size * moving_Value;
	m_Normal_Vector[2] = m_Normal_Vector[2] / position_Vectors_Size * moving_Value;

	// 4. 결과 벡터를 카메라 위치 벡터에 더해준다.
	for (int i = 0; i < 3; ++i) m_Position_Vector[i] += m_Normal_Vector[i];
}

void Camera::XY_Moving(const bool& is_Axis_Y, const float& moving_Value)
{
	m_Position_Vector[is_Axis_Y] += moving_Value;
}

void Camera::Work()
{
	// 1. 이 방법은 World 자체를 회전시킨 후
	//		변경된 카메라 위치를 얻어내어 적용시키게 된다.
	glPushMatrix();
	glRotatef(m_Rotation_Value[0], 1.0f, 0.0f, 0.0f);
	glRotatef(m_Rotation_Value[1], 0.0f, 1.0f, 0.0f);
	glRotatef(m_Rotation_Value[2], 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, m_Transform_Matrix); // Transform을 모두 한 상태의 행렬을 받아온다.
	glPopMatrix();
	

	// 회전값 초기화
	m_Rotation_Value[0] = 0.0f;
	m_Rotation_Value[1] = 0.0f;
	m_Rotation_Value[2] = 0.0f;

	Multiple_Mat_And_Vec(); // 현재 위치에 Transform을 적용시킨 "새 위치"를 얻어내는 메소드.


	// (준비가 다 되었으니 카메라를 설정해주기만 하면 된다.)
	// *** 카메라를 Roll 해주고 싶다면, Up 벡터를 변형시켜줘야 한다.
	gluLookAt(
		m_Position_Vector[0], m_Position_Vector[1], m_Position_Vector[2],
		m_Look_Vector[0], m_Look_Vector[1], m_Look_Vector[2],
		m_Up_Vector[0], m_Up_Vector[1], m_Up_Vector[2]
	);
}