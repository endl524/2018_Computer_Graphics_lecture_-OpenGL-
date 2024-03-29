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

	if (m_Object_Type != FOG_OBJECT)
	{
		// 변환 적용.
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);
	}

	else
	{
		// 변환 적용.
		glRotatef(m_Rotate_Yaw_Value, 0.0f, 1.0f, 0.0f);
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		
	}
	glPushMatrix();
	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);
	// 색상 적용 및 그리기.
	glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

	switch (m_Object_Type)
	{
	case FOG_OBJECT: glutSolidSphere(OBJECT_SIZE * 0.5f, 20, 20); glPopMatrix(); break;

	case SPHERE_OBJECT: glutWireSphere(OBJECT_SIZE * 0.5f, 20, 20); glPopMatrix(); break;

	case FLOOR_OBJECT:
		glutSolidCube(OBJECT_SIZE);
		glPopMatrix();

		glScalef(1.0f, 1.0f, 1.0f);

		// NW 바닥
		glPushMatrix();
		glTranslatef(-OBJECT_SIZE * 0.3f * m_Scale_X, 0.5f, -OBJECT_SIZE * 0.3f * m_Scale_Z);
		glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
		glBegin(GL_TRIANGLES);
		glVertex3f(-FLOOR_TRIANGLE_SIZE, 0.0f, -FLOOR_TRIANGLE_SIZE);
		glVertex3f(-FLOOR_TRIANGLE_SIZE, 0.0f, FLOOR_TRIANGLE_SIZE);
		glVertex3f(FLOOR_TRIANGLE_SIZE, 0.0f, -FLOOR_TRIANGLE_SIZE);
		glEnd();
		glPopMatrix();

		// NE 바닥
		glPushMatrix();
		glTranslatef(OBJECT_SIZE * 0.3f * m_Scale_X, 0.5f, -OBJECT_SIZE * 0.3f * m_Scale_Z);
		glColor4f(0.5f, 0.2f, 0.2f, 1.0f);
		glBegin(GL_TRIANGLES);
		glVertex3f(-FLOOR_TRIANGLE_SIZE, 0.0f, -FLOOR_TRIANGLE_SIZE);
		glVertex3f(FLOOR_TRIANGLE_SIZE, 0.0f, FLOOR_TRIANGLE_SIZE);
		glVertex3f(FLOOR_TRIANGLE_SIZE, 0.0f, -FLOOR_TRIANGLE_SIZE);
		glEnd();
		glPopMatrix();

		// SW 바닥
		glPushMatrix();
		glTranslatef(-OBJECT_SIZE * 0.3f * m_Scale_X, 0.5f, OBJECT_SIZE * 0.3f * m_Scale_Z);
		glColor4f(0.2f, 0.2f, 0.5f, 1.0f);
		glBegin(GL_TRIANGLES);
		glVertex3f(-FLOOR_TRIANGLE_SIZE , 0.0f, -FLOOR_TRIANGLE_SIZE );
		glVertex3f(-FLOOR_TRIANGLE_SIZE , 0.0f, FLOOR_TRIANGLE_SIZE );
		glVertex3f(FLOOR_TRIANGLE_SIZE , 0.0f, FLOOR_TRIANGLE_SIZE );
		glEnd();
		glPopMatrix();

		// SE 바닥
		glPushMatrix();
		glTranslatef(OBJECT_SIZE * 0.3f * m_Scale_X, 0.5f, OBJECT_SIZE * 0.3f * m_Scale_Z);
		glColor4f(0.2f, 0.5f, 0.2f, 1.0f);
		glBegin(GL_TRIANGLES);
		glVertex3f(FLOOR_TRIANGLE_SIZE , 0.0f, -FLOOR_TRIANGLE_SIZE );
		glVertex3f(-FLOOR_TRIANGLE_SIZE , 0.0f, FLOOR_TRIANGLE_SIZE );
		glVertex3f(FLOOR_TRIANGLE_SIZE , 0.0f, FLOOR_TRIANGLE_SIZE );
		glEnd();
		glPopMatrix();

		break;

	default:
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
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

	m_Rotate_Pitch_Value = 0.0f;
	m_Rotate_Yaw_Value = 0.0f;
	m_Rotate_Roll_Value = 0.0f;

	m_Scale_X = 1.0f;
	m_Scale_Y = 1.0f;
	m_Scale_Z = 1.0f;
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
	switch (m_Auto_Move_Dir)
	{
	case 0:
		if (m_Position.z - OBJECT_SIZE * 0.5f * m_Scale_Z + OBJECT_AUTO_MOVE_VALUE < -OBJECT_SIZE * 10.0f)
			m_Auto_Move_Dir = 1;
		m_Position.z -= OBJECT_AUTO_MOVE_VALUE;
		Pitch(-SPHERE_AUTO_ROTATE_VALUE);
		break;

	case 1:
		if (m_Position.z + OBJECT_SIZE * 0.5f * m_Scale_Z + OBJECT_AUTO_MOVE_VALUE > OBJECT_SIZE * 10.0f)
			m_Auto_Move_Dir = 0;
		m_Position.z += OBJECT_AUTO_MOVE_VALUE;
		Pitch(SPHERE_AUTO_ROTATE_VALUE);
		break;

	case 2:
		if (m_Position.x - OBJECT_SIZE * 0.5f * m_Scale_X + OBJECT_AUTO_MOVE_VALUE < -OBJECT_SIZE * 10.0f)
			m_Auto_Move_Dir = 3;
		m_Position.x -= OBJECT_AUTO_MOVE_VALUE;
		Roll(-SPHERE_AUTO_ROTATE_VALUE);
		break;

	case 3:
		if (m_Position.x + OBJECT_SIZE * 0.5f * m_Scale_X + OBJECT_AUTO_MOVE_VALUE > OBJECT_SIZE * 10.0f)
			m_Auto_Move_Dir = 2;
		m_Position.x += OBJECT_AUTO_MOVE_VALUE;
		Roll(SPHERE_AUTO_ROTATE_VALUE);
		break;
	}

	
	Collider_Update();
}

void Object::Auto_Scale()
{
	if (m_Scale_X >= 0.0f)
	{
		m_Scale_X -= 0.05f; m_Scale_Y -= 0.05f; m_Scale_Z -= 0.05f;
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

void Object::Rotate_Check()
{
	if (m_Rotate_Pitch_Value != 0.0f)
	{
		m_Quaternion_temp_1->Set_By_Angle_Axis(m_Rotate_Pitch_Value, 1.0f, 0.0f, 0.0f);
		m_Rotate_Pitch_Value = 0.0f;
		m_Quaternion_Result->Multiple(m_Quaternion_temp_1);
	}
	if (m_Rotate_Yaw_Value != 0.0f)
	{
		m_Quaternion_temp_2->Set_By_Angle_Axis(m_Rotate_Yaw_Value, 0.0f, 1.0f, 0.0f);
		m_Rotate_Yaw_Value = 0.0f;
		m_Quaternion_Result->Multiple(m_Quaternion_temp_2);
	}
	if (m_Rotate_Roll_Value != 0.0f)
	{
		m_Quaternion_temp_3->Set_By_Angle_Axis(m_Rotate_Roll_Value, 0.0f, 0.0f, 1.0f);
		m_Rotate_Roll_Value = 0.0f;
		m_Quaternion_Result->Multiple(m_Quaternion_temp_3);
	}

	m_Quaternion_Result->Get_Transform_Matrix(m_Rotate_Transform_Matrix);
}

void Object::Collider_Update()
{
	m_Collider_Box[0] = m_Position.x - OBJECT_SIZE * 0.5f * m_Scale_X;
	m_Collider_Box[1] = m_Position.x + OBJECT_SIZE * 0.5f * m_Scale_X;
	m_Collider_Box[2] = m_Position.z - OBJECT_SIZE * 0.5f * m_Scale_Z;
	m_Collider_Box[3] = m_Position.z + OBJECT_SIZE * 0.5f * m_Scale_Z;
	m_Collider_Box[4] = m_Position.y - OBJECT_SIZE * 0.5f * m_Scale_Y;
	m_Collider_Box[5] = m_Position.y + OBJECT_SIZE * 0.5f * m_Scale_Y;
}

void Object::Update()
{
	if (m_Object_Type != FOG_OBJECT) Auto_Move();
	else Auto_Scale();
}

void Object::Collision_Procedure()
{
	if (!m_is_Static_Object)
	{
		switch (m_Auto_Move_Dir)
		{
		case 0: m_Auto_Move_Dir = 1; break;
		case 1: m_Auto_Move_Dir = 0; break;
		case 2: m_Auto_Move_Dir = 3; break;
		case 3: m_Auto_Move_Dir = 2; break;
		}
		Auto_Move();
	}
}