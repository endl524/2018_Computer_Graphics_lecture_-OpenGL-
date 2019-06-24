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

	m_Forward_Vector[0] = 0.0f; m_Forward_Vector[1] = 0.0f; m_Forward_Vector[2] = 1.0f; m_Forward_Vector[3] = 1.0f;
}

void Object::Rotate_Check()
{
	if (m_Rotate_Pitch_Value != 0.0f) Quaternion_Process(m_Quaternion_temp_1, m_Rotate_Pitch_Value, 1);
	if (m_Rotate_Yaw_Value != 0.0f) Quaternion_Process(m_Quaternion_temp_2, m_Rotate_Yaw_Value, 2);
	if (m_Rotate_Roll_Value != 0.0f) Quaternion_Process(m_Quaternion_temp_3, m_Rotate_Roll_Value, 3);

	m_Quaternion_Result->Get_Transform_Matrix(m_Rotate_Transform_Matrix);
}

void Object::Quaternion_Process(Quaternion* quat, float& rot_Value, const int& num)
{
	if (num == 1) quat->Set_By_Angle_Axis(rot_Value, 1.0f, 0.0f, 0.0f);
	else if (num == 2) quat->Set_By_Angle_Axis(rot_Value, 0.0f, 1.0f, 0.0f);
	else if (num == 3) quat->Set_By_Angle_Axis(rot_Value, 0.0f, 0.0f, 1.0f);
	else return;
	rot_Value = 0.0f;
	m_Quaternion_Result->Multiple(quat);

	float temp_Transform[16] = { 0.0f, };
	quat->Get_Transform_Matrix(temp_Transform);
	Multiple_Matrix_And_Vector(temp_Transform, m_Forward_Vector);
	Multiple_Matrix_And_Vector(temp_Transform, m_Right_Vector);
	Multiple_Matrix_And_Vector(temp_Transform, m_Up_Vector);

	if (m_OBB != nullptr)
	{
		for (int i = 0; i < 16; ++i) temp_Transform[i] = 0.0f;
		quat->Get_Transform_Matrix(temp_Transform);
		m_OBB->Multiply_Rotate_Matrix_to_OBB_Axis(temp_Transform);
		Set_Pos_to_Collider_Pos();
	}
}

void Object::Draw()
{
	glPushMatrix();

	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glMultMatrixf(m_Rotate_Transform_Matrix);
	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
	glMateriali(GL_FRONT, GL_SHININESS, 20);

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
	
}

Vector3 Object::Get_Normal_Vector(const float& v1_x, const float& v1_y, const float& v1_z, const float& v2_x, const float& v2_y, const float& v2_z)
{
	Vector3 temp_v;
	temp_v.x = v1_y * v2_z - v1_z * v2_y;
	temp_v.y = v1_z * v2_x - v1_x * v2_z;
	temp_v.z = v1_x * v2_y - v1_y * v2_x;
	return temp_v;
}

void Object::Multiple_Matrix_And_Vector(const float* src_matrix, float* dest_vector)
{
	float temp_Float = 0.0f;
	float temp_Vector[4];

	for (int i = 0; i < 4; ++i)
	{
		temp_Float = 0.0f;
		for (int j = 0; j < 4; ++j) temp_Float += src_matrix[i + j * 4] * dest_vector[j];
		temp_Vector[i] = temp_Float;
	}

	for (int i = 0; i < 4; ++i) dest_vector[i] = temp_Vector[i];
}
// ================================================
