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
}


void Camera::Work()
{
	// 2. 이 방법은 카메라는 자기 위치에 고정되어있고, 
	//		World의 회전축 자체를 돌리게 된다.
	glRotatef(m_Rotate_Pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(m_Rotate_Yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(m_Rotate_Roll, 0.0f, 0.0f, 1.0f);
}