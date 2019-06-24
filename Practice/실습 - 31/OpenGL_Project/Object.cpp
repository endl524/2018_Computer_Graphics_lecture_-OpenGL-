#include "Object.h"

int Object::s_Object_Count = 0;

Object::Object(const bool& is_Main_Object)
{
	if (is_Main_Object)	Set_Index(s_Object_Count++);
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

	if (m_OBB != nullptr)
	{
		delete m_OBB;
		m_OBB = nullptr;
	}
}

void Object::Draw()
{
	glPushMatrix();

	if (m_Object_Type != SPHERE_OBJECT) glRotatef(m_Point_Light_Rot_Value, 0.0f, 1.0f, 0.0f);
	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glMultMatrixf(m_Rotate_Transform_Matrix);
	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

	if (m_Object_Type != SPHERE_OBJECT) glLightfv(m_Light_Number, GL_POSITION, m_Light_Pos);
	glMaterialfv(GL_FRONT, GL_AMBIENT, m_Ambient_Color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Diffuse_Color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_Specular_Color);
	glMateriali(GL_FRONT, GL_SHININESS, 20);

	//glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

	// ===============================
	switch (m_Object_Type)
	{
	case SPHERE_OBJECT:
		glutSolidSphere(OBJECT_SIZE * 0.5f, 50, 50);
		break;

	case POINT_LIGHT_1_OBJECT:
	case POINT_LIGHT_2_OBJECT:
		glutSolidCone(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 50, 50);
		break;
	}
	// ===============================

	glPopMatrix();
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

	for (int i = 0; i < 16; ++i) m_Rotate_Transform_Matrix[i] = 0;
	m_Rotate_Transform_Matrix[0] = 1.0f; m_Rotate_Transform_Matrix[5] = 1.0f; m_Rotate_Transform_Matrix[10] = 1.0f; m_Rotate_Transform_Matrix[15] = 1.0f;

	int count = 0;
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


void Object::Update()
{
	Auto_Move();
}

void Object::Collision_Procedure()
{
	cout << "충돌 " << count++ << endl;
}

void Object::Collision_Procedure(const Vector4& collider_Normal_Vector)
{
	cout << "충돌 " << count++ << endl;
}

void Object::Auto_Move()
{
	if (m_is_Auto_Moving) Rotate_Light(3.0f);
}
void Object::Light_Switch()
{
	if (glIsEnabled(m_Light_Number)) glDisable(m_Light_Number);
	else glEnable(m_Light_Number);
}

// ================================================



