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

	// 1. ī�޶��� �ֽ� ���⿡ ���� ���͸� ���Ѵ�.
	for (int i = 0; i < 3; ++i) m_Forward_Vector[i] = m_Position_Vector[i] - m_Look_Vector[i];

	// 2. ���� ������ ũ�⸦ ���Ѵ�.
	position_Vectors_Size =
		sqrtf(m_Forward_Vector[0] * m_Forward_Vector[0]
			+ m_Forward_Vector[1] * m_Forward_Vector[1]
			+ m_Forward_Vector[2] * m_Forward_Vector[2]);

	// 3. ���� ���͸� ����ȭ�Ͽ� ������ ���� ��, �̵��� ũ�⸦ �����ش�. 
	m_Forward_Vector[0] = m_Forward_Vector[0] / position_Vectors_Size * moving_Value;
	m_Forward_Vector[1] = m_Forward_Vector[1] / position_Vectors_Size * moving_Value;
	m_Forward_Vector[2] = m_Forward_Vector[2] / position_Vectors_Size * moving_Value;

	// 4. ��� ���͸� ī�޶� ��ġ ���Ϳ� �����ش�.
	for (int i = 0; i < 3; ++i) m_Position_Vector[i] += m_Forward_Vector[i];
}

void Camera::Work()
{

	// 1. �� ����� World ��ü�� ȸ����Ų ��
	//		����� ī�޶� ��ġ�� ���� �����Ű�� �ȴ�.
	glPushMatrix();
	glRotatef(m_Rotation_Value[0], 1.0f, 0.0f, 0.0f);
	glRotatef(m_Rotation_Value[1], 0.0f, 1.0f, 0.0f);
	glRotatef(m_Rotation_Value[2], 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, m_Transform_Matrix); // Transform�� ��� �� ������ ����� �޾ƿ´�.
	glPopMatrix();

	// ȸ���� �ʱ�ȭ
	m_Rotation_Value[0] = 0.0f;
	m_Rotation_Value[1] = 0.0f;
	m_Rotation_Value[2] = 0.0f;

	Multiple_Mat_And_Vec(); // ���� ��ġ�� Transform�� �����Ų "�� ��ġ"�� ���� �޼ҵ�.

	// (�غ� �� �Ǿ����� ī�޶� �������ֱ⸸ �ϸ� �ȴ�.)
	// * Look ���Ϳ� Position ���͸� �����ָ� "����",
	// ** �������� ������ Look ���͸� ��������, ���࿡ ���� ������ �ϰԵȴ�.
	// *** ī�޶� Roll ���ְ� �ʹٸ�, Up ���͸� ����������� �Ѵ�.
	gluLookAt(
		m_Position_Vector[0], m_Position_Vector[1], m_Position_Vector[2],
		m_Look_At_X, m_Look_At_Y, m_Look_At_Z,
		m_Up_Vector[0], m_Up_Vector[1], m_Up_Vector[2]
	);
	

	// =====================================================
	// =====================================================



	// 2. �� ����� ī�޶�� �ڱ� ��ġ�� �����Ǿ��ְ�, 
	//		World�� ȸ���� ��ü�� ������ �ȴ�.
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