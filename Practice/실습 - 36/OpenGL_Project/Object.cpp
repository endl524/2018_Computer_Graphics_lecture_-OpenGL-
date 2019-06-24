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

	if (m_OBB != nullptr)
	{
		delete m_OBB;
		m_OBB = nullptr;
	}
}

void Object::Init_Object()
{
	m_Quaternion_Result = new Quaternion();
	m_Quaternion_temp_1 = new Quaternion();
	m_Quaternion_temp_2 = new Quaternion();
	m_Quaternion_temp_3 = new Quaternion();

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

void Object::Draw()
{
	glPushMatrix();

	if (m_Object_Type == SPHERE_OBJECT) glRotatef(m_Sphere_Rot_Value, 0.0f, 1.0f, 0.0f);

	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glMultMatrixf(m_Rotate_Transform_Matrix);
	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
	glMateriali(GL_FRONT, GL_SHININESS, 20);
	
	if (m_Object_Type == SPHERE_OBJECT) glutSolidSphere(OBJECT_SIZE * 0.5f, 50, 50);

	glPopMatrix();
}

void Object::Update()
{
	Auto_Move();
}

bool Object::Collision_Check(Object* other)
{
	if (m_OBB != nullptr && other->Get_OBB() != nullptr)
	{
		if (m_OBB->OBB_Intersection(other->Get_OBB()))
		{
			Collision_Procedure(); return true;
		}
	}
	return false;
}

void Object::Collision_Procedure()
{
	cout << "Ãæµ¹ " << count++ << endl;
}

void Object::Auto_Move()
{
	if (m_Object_Type == SPHERE_OBJECT)
	{
		m_Sphere_Rot_Value += -3.0f;

		if (m_Sphere_Rot_Value >= 360.0f || m_Sphere_Rot_Value <= -360.0f)
		{
			int a = m_Sphere_Rot_Value / 360.0f;
			m_Sphere_Rot_Value = m_Sphere_Rot_Value - a * 360;
		}
	}
}

Vector3 Object::Get_Normal_Vector(const float& v1_x, const float& v1_y, const float& v1_z, const float& v2_x, const float& v2_y, const float& v2_z)
{
	Vector3 temp_v;
	temp_v.x = v1_y * v2_z - v1_z * v2_y;
	temp_v.y = v1_z * v2_x - v1_x * v2_z;
	temp_v.z = v1_x * v2_y - v1_y * v2_x;
	return temp_v;
}
// ================================================
