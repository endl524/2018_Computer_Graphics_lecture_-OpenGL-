#include "Object.h"

int Object::s_Object_Count = 0;

Object::Object()
{
	Set_Index(s_Object_Count++);
	Init_Object();
}

Object::~Object() 
{
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

void Object::Draw()
{
	glPushMatrix(); // 다음의 변환들을 스택에 넣는다.

	// 변환 적용.
	glTranslatef(m_Position.x, m_Position.y, m_Position.z);

	m_is_Pitch_Shift = m_Rotate_Pitch != 0.0f ? true : false;
	m_is_Yaw_Shift = m_Rotate_Yaw != 0.0f ? true : false;
	m_is_Roll_Shift = m_Rotate_Roll != 0.0f ? true : false;

	if (m_is_Pitch_Shift)
	{
		m_Quaternion_temp_1->Set_By_Angle_Axis(m_Rotate_Pitch, 1.0f, 0.0f, 0.0f);
		m_Rotate_Pitch = 0.0f;
		m_Quaternion_Result->Multiple(m_Quaternion_temp_1);
	}
	if (m_is_Yaw_Shift)
	{
		m_Quaternion_temp_2->Set_By_Angle_Axis(m_Rotate_Yaw, 0.0f, 1.0f, 0.0f);
		m_Rotate_Yaw = 0.0f;
		m_Quaternion_Result->Multiple(m_Quaternion_temp_2);
	}
	if (m_is_Roll_Shift)
	{
		m_Quaternion_temp_3->Set_By_Angle_Axis(m_Rotate_Roll, 0.0f, 0.0f, 1.0f);
		m_Rotate_Roll = 0.0f;
		m_Quaternion_Result->Multiple(m_Quaternion_temp_3);
	}

	m_Quaternion_Result->Get_Transform_Matrix(m_Transform);
	glMultMatrixf(m_Transform);

	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

	glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

	switch (m_Object_Type)
	{
	case OBJECT_TYPE_CUBE:
		if (m_is_Solid_Shape) glutSolidCube(OBJECT_SIZE);
		else glutWireCube(OBJECT_SIZE);
		break;

	case OBJECT_TYPE_SPHERE:
		if (m_is_Solid_Shape) glutSolidSphere(OBJECT_SIZE * 0.5f, 20, 20);
		else glutWireSphere(OBJECT_SIZE * 0.5f, 20, 20);
		break;
	}

	glPopMatrix(); // 스택의 top에 있는 변환들을 꺼낸다.
}



void Object::Init_Object()
{
	m_Quaternion_Result = new Quaternion();
	m_Quaternion_temp_1 = new Quaternion();
	m_Quaternion_temp_2 = new Quaternion();
	m_Quaternion_temp_3 = new Quaternion();

	m_Move_Speed = 0.0f;

	m_Position.x = 0.0f;
	m_Position.y = 0.0f;
	m_Position.z = 0.0f;

	m_Rotate_Roll = 0.0f;
	m_Rotate_Yaw = 0.0f;
	m_Rotate_Pitch = 0.0f;

	m_Scale_X = 1.0f;
	m_Scale_Y = 1.0f;
	m_Scale_Z = 1.0f;

	m_Object_Type = OBJECT_TYPE_CUBE;
}

void Object::Auto_ColorScale()
{
	m_Color.R += OBJECT_AUTO_COLOR_SCALE_VALUE * m_Color_Scale_Sign_R;
	if (m_Color.R <= 0.0f || m_Color.R >= 1.0f)
	{
		m_Color_Scale_Sign_R *= -1;
		m_Color.R = m_Color.R <= 0.0f ? 0.0f : 1.0f;
	}
	m_Color.G += OBJECT_AUTO_COLOR_SCALE_VALUE * m_Color_Scale_Sign_G;
	if (m_Color.G <= 0.0f || m_Color.G >= 1.0f)
	{
		m_Color_Scale_Sign_G *= -1;
		m_Color.G = m_Color.G <= 0.0f ? 0.0f : 1.0f;
	}
	m_Color.B += OBJECT_AUTO_COLOR_SCALE_VALUE * m_Color_Scale_Sign_B;
	if (m_Color.B <= 0.0f || m_Color.B >= 1.0f)
	{
		m_Color_Scale_Sign_B *= -1;
		m_Color.B = m_Color.B <= 0.0f ? 0.0f : 1.0f;
	}
}

void Object::Move(const int& axis, const float& value)
{
	switch (axis)
	{
	case 0: Roll(value * 3);
		if (m_Position.x + value < OBJECT_SIZE * 2.5f && m_Position.x + value > -OBJECT_SIZE * 2.5f)
			m_Position.x += value;
		break;
	case 1: Yaw(value * 3); break;
	case 2: Pitch(-value * 3); 
		if (m_Position.z + value < OBJECT_SIZE * 2.5f && m_Position.z + value > -OBJECT_SIZE * 2.5f)
			m_Position.z += value;
		break;
	}
}


void Object::Update()
{
}