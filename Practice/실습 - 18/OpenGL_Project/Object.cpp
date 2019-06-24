#include "Object.h"

int Object::s_Object_Count = 0;
float Object::s_World_Rotate_Pitch = 0.0f;
float Object::s_World_Rotate_Yaw = 0.0f;
float Object::s_World_Rotate_Roll = 0.0f;

Object::Object()
{
	Set_Index(s_Object_Count++);
	//Set_Random_Device();
	Init_Object();
}

Object::~Object() {}

void Object::Draw()
{
	glPushMatrix(); // 다음의 변환들을 스택에 넣는다.

	// 변환 적용.
	if (m_is_Shape_Object)
	{
		glRotatef(s_World_Rotate_Pitch, 1.0f, 0.0f, 0.0f);
		glRotatef(s_World_Rotate_Yaw, 0.0f, 1.0f, 0.0f);
		glRotatef(s_World_Rotate_Roll, 0.0f, 0.0f, 1.0f);
	}

	glTranslatef(m_Position_X, m_Position_Y, m_Position_Z);
	glRotatef(m_Rotate_Pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(m_Rotate_Yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(m_Rotate_Roll, 0.0f, 0.0f, 1.0f);
	
	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

	glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

	switch (m_Object_Type)
	{
	case OBJECT_TYPE_CONE:
		if (m_is_Solid_Shape) glutSolidCone(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 30, 30);
		else glutWireCone(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 30, 30);
		break;

	case OBJECT_TYPE_CUBE:
		if (m_is_Solid_Shape) glutSolidCube(OBJECT_SIZE);
		else glutWireCube(OBJECT_SIZE);
		break;

	case OBJECT_TYPE_SPHERE:
		if (m_is_Solid_Shape) glutSolidSphere(OBJECT_SIZE * 0.5f, 30, 30);
		else glutWireSphere(OBJECT_SIZE * 0.5f, 30, 30);
		break;

	case OBJECT_TYPE_TEAPOT:
		if (m_is_Solid_Shape) glutSolidTeapot(OBJECT_SIZE * 0.5f);
		else glutWireTeapot(OBJECT_SIZE * 0.5f);
		break;
	}

	glPopMatrix(); // 스택의 top에 있는 변환들을 꺼낸다.
}

void Object::Set_Random_Device()
{
	mt19937_64 seed(m_Random_Device());
	m_Random_SEED = seed;

	uniform_real_distribution<> rand_Color(0.0f, 1.0f);
	m_Random_Color = rand_Color;
}

void Object::Init_Object()
{
	m_Position_X = 0.0f;
	m_Position_Y = 0.0f;
	m_Position_Z = 0.0f;

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

void Object::Scale_Once(const int& sign)
{
	m_Scale_X = m_Scale_X + OBJECT_SCALE_VALUE * sign > 0.2f && m_Scale_X + OBJECT_SCALE_VALUE * sign < 1.8f ? m_Scale_X + OBJECT_SCALE_VALUE * sign : m_Scale_X;
	m_Scale_Y = m_Scale_Y + OBJECT_SCALE_VALUE * sign > 0.2f && m_Scale_Y + OBJECT_SCALE_VALUE * sign < 1.8f ? m_Scale_Y + OBJECT_SCALE_VALUE * sign : m_Scale_Y;
	m_Scale_Z = m_Scale_Z + OBJECT_SCALE_VALUE * sign > 0.2f && m_Scale_Z + OBJECT_SCALE_VALUE * sign < 1.8f ? m_Scale_Z + OBJECT_SCALE_VALUE * sign : m_Scale_Z;
}

void Object::Shape_Change(const int& type, const bool& is_Left)
{
	s_World_Rotate_Pitch = 0.0f;
	s_World_Rotate_Yaw = 0.0f;
	s_World_Rotate_Roll = 0.0f;

	switch (type)
	{
	case OBJECT_TYPE_SPHERE:
		Set_Object_Type(OBJECT_TYPE_SPHERE);
		Set_Scale(1.0f, 1.0f, 1.0f);
		Set_Rotate_Pitch(0.0f);
		Set_Rotate_Yaw(0.0f);
		Set_Rotate_Roll(0.0f);
		if (is_Left) Set_Translate(-OBJECT_SIZE, -OBJECT_SIZE * 0.5f, 0.0f);
		else Set_Translate(OBJECT_SIZE, -OBJECT_SIZE * 0.5f, 0.0f);
		break;

	case OBJECT_TYPE_CUBE:
		Set_Object_Type(OBJECT_TYPE_CUBE);
		Set_Scale(1.0f, 2.0f, 1.0f);
		Set_Rotate_Pitch(0.0f);
		Set_Rotate_Yaw(0.0f);
		Set_Rotate_Roll(0.0f);
		if (is_Left) Set_Translate(-OBJECT_SIZE, 0.0f, 0.0f);
		else Set_Translate(OBJECT_SIZE, 0.0f, 0.0f);
		break;

	case OBJECT_TYPE_CONE:
		Set_Object_Type(OBJECT_TYPE_CONE);
		Set_Scale(1.0f, 1.0f, 2.0f);
		Set_Rotate_Pitch(-90.0f);
		Set_Rotate_Yaw(0.0f);
		Set_Rotate_Roll(0.0f);
		if (is_Left) Set_Translate(-OBJECT_SIZE, -OBJECT_SIZE, 0.0f);
		else Set_Translate(OBJECT_SIZE, -OBJECT_SIZE, 0.0f);
		break;

	case OBJECT_TYPE_TEAPOT:
		Set_Object_Type(OBJECT_TYPE_TEAPOT);
		Set_Scale(1.0f, 1.0f, 1.0f);
		Set_Rotate_Pitch(0.0f);
		Set_Rotate_Yaw(0.0f);
		Set_Rotate_Roll(0.0f);
		if (is_Left) Set_Translate(-OBJECT_SIZE, -OBJECT_SIZE * 0.55f, 0.0f);
		else Set_Translate(OBJECT_SIZE, -OBJECT_SIZE * 0.55f, 0.0f);
		break;
	}
}

void Object::Rotate(const int& axis, const float& value)
{
	switch (axis)
	{
	case 0: Set_Rotate_Pitch(m_Rotate_Pitch + value); break;
	case 1: if (m_Object_Type == OBJECT_TYPE_CONE) Set_Rotate_Roll(m_Rotate_Roll + value); else Set_Rotate_Yaw(m_Rotate_Yaw + value); break;
	case 2: Set_Rotate_Roll(m_Rotate_Roll + value); break;
	}
}

void Object::World_Rotate(const int& axis, const float& value)
{
	switch (axis)
	{
	case 0: Set_World_Rotate_Pitch(s_World_Rotate_Pitch + value); break;
	case 1: if (m_Object_Type == OBJECT_TYPE_CONE) Set_World_Rotate_Roll(s_World_Rotate_Roll + value); else Set_World_Rotate_Yaw(s_World_Rotate_Yaw + value); break;
	case 2: Set_World_Rotate_Roll(s_World_Rotate_Roll + value); break;
	}
}

void Object::Update()
{
	Rotate(1, 3.0f);
}