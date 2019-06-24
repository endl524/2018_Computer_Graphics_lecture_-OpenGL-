#include "Object.h"

int Object::s_Object_Count = 0;

Object::Object(const bool& is_active)
{
	Set_Index(s_Object_Count++);
	Set_Active(is_active);
	Set_Random_Device();
	Init_Object();

	m_Object_Type = m_Random_Type(m_Random_SEED);

	m_Color.R = m_Random_Color(m_Random_SEED);
	m_Color.G = m_Random_Color(m_Random_SEED);
	m_Color.B = m_Random_Color(m_Random_SEED);

	m_Local_Scale_X = m_Random_Scale(m_Random_SEED);
	m_Local_Scale_Y = m_Local_Scale_X;
	m_Local_Scale_Z = m_Local_Scale_X;

	m_Rotate_Direction = m_Random_Rotate_Direction(m_Random_SEED);

	if (m_Rotate_Direction == 0) m_Rotate_Direction = 1;
	else if (m_Rotate_Direction == -2) m_Rotate_Direction = -1;

	m_Local_Rotate_X = m_Random_Axis(m_Random_SEED);
	m_Local_Rotate_Y = m_Random_Axis(m_Random_SEED);
	m_Local_Rotate_Z = m_Random_Axis(m_Random_SEED);
}

Object::~Object() {}

void Object::Draw()
{
	if (m_is_Activated)
	{
		glLoadIdentity();

		glPushMatrix(); // 다음의 변환들을 스택에 넣는다.

		// 로컬 변환 적용.
		
		glTranslatef(m_Local_Translate_X, m_Local_Translate_Y, m_Local_Translate_Z);
		glRotatef(m_Local_Rotate_Angle, m_Local_Rotate_X, m_Local_Rotate_Y, m_Local_Rotate_Z);
		glScalef(m_Local_Scale_X, m_Local_Scale_Y, m_Local_Scale_Z);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

		switch (m_Object_Type)
		{
		case OBJECT_TYPE_CONE: glutWireCone(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 30, 30); break;
		case OBJECT_TYPE_CUBE: glutWireCube(OBJECT_SIZE); break;
		case OBJECT_TYPE_SPHERE: glutWireSphere(OBJECT_SIZE * 0.5f, 30, 30); break;
		case OBJECT_TYPE_TORUS: glutWireTorus(OBJECT_SIZE * 0.2f, OBJECT_SIZE * 0.5f, 30, 30); break;
		case OBJECT_TYPE_TEAPOT: glutWireTeapot(OBJECT_SIZE * 0.5f); break;
		}
		
		glPopMatrix(); // 스택의 top에 있는 변환들을 꺼낸다.
	}
}

void Object::Set_Random_Device()
{
	mt19937_64 seed(m_Random_Device());
	m_Random_SEED = seed;

	uniform_real_distribution<> rand_Color(0.0f, 1.0f);
	m_Random_Color = rand_Color;

	uniform_real_distribution<> rand_Axis(0.0f, 1.0f);
	m_Random_Axis = rand_Axis;

	uniform_real_distribution<> rand_Scale(0.3f, 1.2f);
	m_Random_Scale = rand_Scale;

	uniform_int_distribution<> rand_RotDir(-2, 1);
	m_Random_Rotate_Direction = rand_RotDir;

	uniform_int_distribution<> rand_Type(0, 4);
	m_Random_Type = rand_Type;
}

void Object::Init_Object()
{
	m_Local_Translate_X = 0.0f;
	m_Local_Translate_Y = 0.0f;
	m_Auto_Translate_Value_X = 0.0f;
	m_Auto_Translate_Value_Y = 0.0f;

	m_Local_Rotate_Angle = 0.0f;
	m_Local_Rotate_X = 0.0f;
	m_Local_Rotate_Y = 0.0f;
	m_Local_Rotate_Z = 0.0f;
	m_Auto_Rotate_Angle = 0.0f;

	m_Local_Scale_X = 1.0f;
	m_Local_Scale_Y = 1.0f;
	m_Local_Scale_Z = 1.0f;
	m_Auto_Scale_Sign = 1;

	m_is_Auto_Rotating = true;
	m_is_Auto_Scaling = true;
}

void Object::Auto_ColorScale()
{
	if (m_is_Auto_Scaling)
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
}

void Object::Auto_Rotating()
{
	float angle = m_Local_Rotate_Angle + OBJECT_AUTO_ROTATE_VALUE * m_Rotate_Direction;
	m_Local_Rotate_Angle = angle >= -360.0f || angle <= 360.0f ? angle: 0.0f;
}

void Object::Auto_Scaling()
{
	m_Local_Scale_X += OBJECT_AUTO_SCALE_VALUE;
	m_Local_Scale_Y += OBJECT_AUTO_SCALE_VALUE;
	m_Local_Scale_Z += OBJECT_AUTO_SCALE_VALUE;

	if (m_Local_Scale_X <= 0.0f && m_Local_Scale_Y <= 0.0f && m_Local_Scale_Z <= 0.0f)
		m_is_Destroyed = true;
}

void Object::Local_Scale(const int& sign)
{
	m_Local_Scale_X = m_Local_Scale_X + OBJECT_SCALE_VALUE * sign > 0.2f && m_Local_Scale_X + OBJECT_SCALE_VALUE * sign < 1.8f ? m_Local_Scale_X + OBJECT_SCALE_VALUE * sign : m_Local_Scale_X;
	m_Local_Scale_Y = m_Local_Scale_Y + OBJECT_SCALE_VALUE * sign > 0.2f && m_Local_Scale_Y + OBJECT_SCALE_VALUE * sign < 1.8f ? m_Local_Scale_Y + OBJECT_SCALE_VALUE * sign : m_Local_Scale_Y;
	m_Local_Scale_Z = m_Local_Scale_Z + OBJECT_SCALE_VALUE * sign > 0.2f && m_Local_Scale_Z + OBJECT_SCALE_VALUE * sign < 1.8f ? m_Local_Scale_Z + OBJECT_SCALE_VALUE * sign : m_Local_Scale_Z;
}

void Object::Update()
{
	Auto_Rotating();
	Auto_Scaling();
	Auto_ColorScale();
}