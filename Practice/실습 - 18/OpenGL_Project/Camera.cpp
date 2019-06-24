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
	m_Position_X = 0.0f;
	m_Position_Y = 50.0f;
	m_Position_Z = 300.0f;

	m_Look_At_X = 0.0f;
	m_Look_At_Y = 0.0f;
	m_Look_At_Z = -1.0f;
	

	m_Rotate_Roll = 0.0f;
	m_Rotate_Yaw = 0.0f;
	m_Rotate_Pitch = 0.0f;

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
	for (int i = 0; i < 3; ++i) m_Forward_Vector[i] = m_Position_Vector[i] - m_Look_Vector[i];

	// 2. 구한 벡터의 크기를 구한다.
	position_Vectors_Size =
		sqrtf(m_Forward_Vector[0] * m_Forward_Vector[0]
			+ m_Forward_Vector[1] * m_Forward_Vector[1]
			+ m_Forward_Vector[2] * m_Forward_Vector[2]);

	// 3. 구한 벡터를 정규화하여 방향을 구한 뒤, 이동할 크기를 곱해준다. 
	m_Forward_Vector[0] = m_Forward_Vector[0] / position_Vectors_Size * moving_Value;
	m_Forward_Vector[1] = m_Forward_Vector[1] / position_Vectors_Size * moving_Value;
	m_Forward_Vector[2] = m_Forward_Vector[2] / position_Vectors_Size * moving_Value;

	// 4. 결과 벡터를 카메라 위치 벡터에 더해준다.
	for (int i = 0; i < 3; ++i) m_Position_Vector[i] += m_Forward_Vector[i];
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
	// * Look 벡터에 Position 벡터를 더해주면 "자전",
	// ** 더해주지 않으면 Look 벡터를 기준으로, 각축에 대해 공전을 하게된다.
	// *** 카메라를 Roll 해주고 싶다면, Up 벡터를 변형시켜줘야 한다.
	gluLookAt(
		m_Position_Vector[0], m_Position_Vector[1], m_Position_Vector[2],
		m_Look_At_X, m_Look_At_Y, m_Look_At_Z,
		m_Up_Vector[0], m_Up_Vector[1], m_Up_Vector[2]
	);
	

	// =====================================================
	// =====================================================



	// 2. 이 방법은 카메라는 자기 위치에 고정되어있고, 
	//		World의 회전축 자체를 돌리게 된다.
	/*
	gluLookAt(
		m_Position_X, m_Position_Y, m_Position_Z,
		m_Look_At_X, m_Look_At_Y, m_Look_At_Z,
		0.0f, 1.0f, 0.0f
	);
	*/
	glRotatef(m_Rotate_Pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(m_Rotate_Yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(m_Rotate_Roll, 0.0f, 0.0f, 1.0f);
	
}