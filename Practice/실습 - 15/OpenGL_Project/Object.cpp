#include "Object.h"

int Object::s_Object_Count = 0;
float Object::s_World_Rotate_Angle = 0.0f;
float Object::s_World_Rotate_Y = 1.0f;


Object::Object()
{
	Set_Index(s_Object_Count++);
	//Set_Random_Device();
	Init_Object();
}

Object::Object(const bool& a) {}

Object::~Object() {}

void Object::Draw()
{
	glLoadIdentity();

	glPushMatrix(); // 다음의 변환들을 스택에 넣는다.
	glRotatef(s_World_Rotate_Angle, 0.0f, s_World_Rotate_Y, 0.0f);

	// 로컬 변환 적용.
	glRotatef(m_Local_Rotate_Angle, m_Local_Rotate_X, m_Local_Rotate_Y, m_Local_Rotate_Z);
	glTranslatef(m_Local_Translate_X, m_Local_Translate_Y, m_Local_Translate_Z);
	
	glScalef(m_Local_Scale_X, m_Local_Scale_Y, m_Local_Scale_Z);

	
	switch (m_Object_Type)
	{
	case OBJECT_TYPE_CUBE:
		if (m_is_Activated)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			glutSolidCube(OBJECT_SIZE);
		}
		else
		{
			glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
			glutWireCube(OBJECT_SIZE);
		}
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
	m_Local_Translate_X = 0.0f;
	m_Local_Translate_Y = 0.0f;

	m_Local_Rotate_Angle = 30.0f;
	m_Local_Rotate_X = 1.0f;
	m_Local_Rotate_Y = 1.0f;
	m_Local_Rotate_Z = 0.0f;

	m_Local_Scale_X = 1.0f;
	m_Local_Scale_Y = 1.0f;
	m_Local_Scale_Z = 1.0f;

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

void Object::Local_Scale(const int& sign)
{
	m_Local_Scale_X = m_Local_Scale_X + OBJECT_SCALE_VALUE * sign > 0.2f && m_Local_Scale_X + OBJECT_SCALE_VALUE * sign < 1.8f ? m_Local_Scale_X + OBJECT_SCALE_VALUE * sign : m_Local_Scale_X;
	m_Local_Scale_Y = m_Local_Scale_Y + OBJECT_SCALE_VALUE * sign > 0.2f && m_Local_Scale_Y + OBJECT_SCALE_VALUE * sign < 1.8f ? m_Local_Scale_Y + OBJECT_SCALE_VALUE * sign : m_Local_Scale_Y;
	m_Local_Scale_Z = m_Local_Scale_Z + OBJECT_SCALE_VALUE * sign > 0.2f && m_Local_Scale_Z + OBJECT_SCALE_VALUE * sign < 1.8f ? m_Local_Scale_Z + OBJECT_SCALE_VALUE * sign : m_Local_Scale_Z;
}

void Object::World_Rotate(const int& sign)
{
	s_World_Rotate_Angle = s_World_Rotate_Angle + OBJECT_ROTATE_VALUE * sign > -360.0f && s_World_Rotate_Angle + OBJECT_ROTATE_VALUE * sign < 360.0f ? s_World_Rotate_Angle + OBJECT_ROTATE_VALUE * sign : 0.0f;
}

void Object::Update()
{
}