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

	if (m_Middle_Object != nullptr)
	{
		delete m_Middle_Object;
		m_Middle_Object = nullptr;
	}

	if (m_Upper_Object != nullptr)
	{
		delete m_Upper_Object;
		m_Upper_Object = nullptr;
	}
}

void Object::Draw()
{
	// 크레인 하단부 그리기.

	glPushMatrix(); // 다음의 변환들을 스택에 넣는다.

	// 변환 적용.
	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	Rotate_Check();
	glMultMatrixf(m_Rotate_Transform_Matrix);

	glPushMatrix(); // Lower Object Scale Push
	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

	glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
	glutSolidCube(OBJECT_SIZE);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glutWireCube(OBJECT_SIZE * 1.01f);
	glPopMatrix(); // Lower Object Scale Pop


	// 크레인 중단부 그리기.
	if (m_Middle_Object)
	{
		glPushMatrix(); // Middle Object Translate & Rotate Push
		glTranslatef(0.0f, m_Middle_Object->Get_Position_Y() * 0.5f, 0.0f);
		glRotatef(m_Stacked_Mid_Objs_Pitch, 1.0f, 0.0f, 0.0f);
		glRotatef(m_Stacked_Mid_Objs_Yaw, 0.0f, 1.0f, 0.0f);
		glTranslatef(m_Middle_Object->Get_Position_X(), m_Middle_Object->Get_Position_Y() * 0.5f, m_Middle_Object->Get_Position_Z());

		glPushMatrix(); // Middle Object Scale Push
		glScalef(m_Middle_Object->Get_Scale_X(), m_Middle_Object->Get_Scale_Y(), m_Middle_Object->Get_Scale_Z());

		glColor4f(m_Middle_Object->Get_Color_R(), m_Middle_Object->Get_Color_G(), m_Middle_Object->Get_Color_B(), m_Middle_Object->Get_Color_A());
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // Middle Object Scale Pop
		

		// 크레인 상단부 그리기.
		if (m_Upper_Object)
		{
			glTranslatef(0.0f, m_Upper_Object->Get_Position_Y() * 0.5f, 0.0f);
			glRotatef(m_Stacked_Upp_Objs_Pitch, 1.0f, 0.0f, 0.0f);
			glRotatef(m_Stacked_Upp_Objs_Roll, 0.0f, 0.0f, 1.0f);
			glTranslatef(m_Upper_Object->Get_Position_X(), m_Upper_Object->Get_Position_Y() * 0.5f, m_Upper_Object->Get_Position_Z());

			glScalef(m_Upper_Object->Get_Scale_X(), m_Upper_Object->Get_Scale_Y(), m_Upper_Object->Get_Scale_Z());

			glColor4f(m_Upper_Object->Get_Color_R(), m_Upper_Object->Get_Color_G(), m_Upper_Object->Get_Color_B(), m_Upper_Object->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
		}
		glPopMatrix(); // Middle Object Translate & Rotate Pop
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

	m_Auto_Move_Sign = 1;

	m_Rotate_Roll = 0.0f;
	m_Rotate_Yaw = 0.0f;
	m_Rotate_Pitch = 0.0f;

	m_Scale_X = 1.0f;
	m_Scale_Y = 1.0f;
	m_Scale_Z = 1.0f;
}

void Object::Object_Setting()
{
	// 크레인 중단부 생성/설정
	m_Middle_Object = new Object(false);
	m_Middle_Object->Init_Object();
	m_Middle_Object->Set_Color(0.0f, 1.0f, 0.0f, 1.0f);
	m_Middle_Object->Set_Scale(0.25f, 1.0f, 0.25f);
	m_Middle_Object->Set_Translate(0.0f, OBJECT_SIZE * m_Scale_Y, 0.0f);

	// 크레인 상단부 생성/설정
	m_Upper_Object = new Object(false);
	m_Upper_Object->Init_Object();
	m_Upper_Object->Set_Color(0.0f, 0.0f, 1.0f, 1.0f);
	m_Upper_Object->Set_Scale(0.1f, 1.0f, 0.1f);
	m_Upper_Object->Set_Translate(0.0f, OBJECT_SIZE * m_Middle_Object->Get_Scale_Y(), 0.0f);

	m_Stacked_Mid_Objs_Pitch = 0.0f;
	m_Stacked_Mid_Objs_Yaw = 0.0f;
	m_Stacked_Upp_Objs_Pitch = 0.0f;
	m_Stacked_Upp_Objs_Roll = 0.0f;
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

void Object::Auto_Move()
{
	if (m_Position.x + OBJECT_SIZE * 0.5f + OBJECT_AUTO_MOVE_VALUE * m_Auto_Move_Sign > OBJECT_SIZE * 2.5f || m_Position.x - OBJECT_SIZE * 0.5f + OBJECT_AUTO_MOVE_VALUE * m_Auto_Move_Sign < -OBJECT_SIZE * 2.5f)
		m_Auto_Move_Sign *= -1;
	m_Position.x += OBJECT_AUTO_MOVE_VALUE * m_Auto_Move_Sign;
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

void Object::Rotate_Check()
{
	if (m_Rotate_Pitch != 0.0f)
	{
		m_Quaternion_temp_1->Set_By_Angle_Axis(m_Rotate_Pitch, 1.0f, 0.0f, 0.0f);
		m_Rotate_Pitch = 0.0f;
		m_Quaternion_Result->Multiple(m_Quaternion_temp_1);
	}
	if (m_Rotate_Yaw != 0.0f)
	{
		m_Quaternion_temp_2->Set_By_Angle_Axis(m_Rotate_Yaw, 0.0f, 1.0f, 0.0f);
		m_Rotate_Yaw = 0.0f;
		m_Quaternion_Result->Multiple(m_Quaternion_temp_2);
	}
	if (m_Rotate_Roll != 0.0f)
	{
		m_Quaternion_temp_3->Set_By_Angle_Axis(m_Rotate_Roll, 0.0f, 0.0f, 1.0f);
		m_Rotate_Roll = 0.0f;
		m_Quaternion_Result->Multiple(m_Quaternion_temp_3);
	}

	m_Quaternion_Result->Get_Transform_Matrix(m_Rotate_Transform_Matrix);
}

void Object::Update()
{
	Auto_Move();
}

void Object::Middle_Rotate_X(const float& value)
{
	if (m_Stacked_Mid_Objs_Pitch + value <= 90.0f && m_Stacked_Mid_Objs_Pitch + value >= -90.0f)
	{
		m_Stacked_Mid_Objs_Pitch += value;
	}
}
void Object::Middle_Rotate_Y(const float& value)
{
	if (m_Stacked_Mid_Objs_Yaw + value <= 90.0f && m_Stacked_Mid_Objs_Yaw + value >= -90.0f)
	{
		m_Stacked_Mid_Objs_Yaw += value;
	}
}

void Object::Upper_Rotate_X(const float& value)
{
	if (m_Stacked_Upp_Objs_Pitch + value <= 90.0f && m_Stacked_Upp_Objs_Pitch + value >= -90.0f)
	{
		m_Stacked_Upp_Objs_Pitch += value;
	}
}
void Object::Upper_Rotate_Z(const float& value)
{
	if (m_Stacked_Upp_Objs_Roll + value <= 90.0f && m_Stacked_Upp_Objs_Roll + value >= -90.0f)
	{
		m_Stacked_Upp_Objs_Roll += value;
	}
}