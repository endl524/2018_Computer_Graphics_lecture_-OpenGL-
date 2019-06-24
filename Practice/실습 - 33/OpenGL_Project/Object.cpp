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
	if (m_Object_Type == SNOW_OBJECT)
	{
		glDisable(GL_LIGHTING);
		glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glColor3f(1.0f, 1.0f, 1.0f);
		glutSolidSphere(OBJECT_SIZE * 0.05f, 15, 15);
		glPopMatrix();
		glEnable(GL_LIGHTING);
		return;
	}

	glPushMatrix();

	if (m_Object_Type == SPHERE_OBJECT) glRotatef(m_Sphere_Rot_Value, 0.0f, 1.0f, 0.0f);

	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glMultMatrixf(m_Rotate_Transform_Matrix);
	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_AMBIENT, m_Ambient_Color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Diffuse_Color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_Specular_Color);
	glMateriali(GL_FRONT, GL_SHININESS, 20);

	if (m_Object_Type == FLOOR_OBJECT)
	{
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-FLOOR_PIECE_SIZE, 0.0f, -FLOOR_PIECE_SIZE);
		glVertex3f(-FLOOR_PIECE_SIZE, 0.0f, FLOOR_PIECE_SIZE);
		glVertex3f(FLOOR_PIECE_SIZE, 0.0f, FLOOR_PIECE_SIZE);
		glVertex3f(FLOOR_PIECE_SIZE, 0.0f, -FLOOR_PIECE_SIZE);
		glEnd();
	}
	else if (m_Object_Type == PIRAMID_OBJECT)
	{
		if (m_is_Normal_On)
		{
			Vector3 temp_v;
			glBegin(GL_QUADS);
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glEnd();
			glBegin(GL_TRIANGLES);
			temp_v = Get_Normal_Vector(-OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE, OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE);
			glNormal3f(temp_v.x, temp_v.y, temp_v.z);
			glVertex3f(0.0f, OBJECT_SIZE, 0.0f);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glEnd();
			glBegin(GL_TRIANGLES);
			temp_v = Get_Normal_Vector(OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE, OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE);
			glNormal3f(temp_v.x, temp_v.y, temp_v.z);
			glVertex3f(0.0f, OBJECT_SIZE, 0.0f);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glEnd();
			glBegin(GL_TRIANGLES);
			temp_v = Get_Normal_Vector(OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE);
			glNormal3f(temp_v.x, temp_v.y, temp_v.z);
			glVertex3f(0.0f, OBJECT_SIZE, 0.0f);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glEnd();
			glBegin(GL_TRIANGLES);
			temp_v = Get_Normal_Vector(-OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE);
			glNormal3f(temp_v.x, temp_v.y, temp_v.z);
			glVertex3f(0.0f, OBJECT_SIZE, 0.0f);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glEnd();
		}
		else
		{
			glBegin(GL_QUADS);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glEnd();
			glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, OBJECT_SIZE, 0.0f);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glEnd();
			glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, OBJECT_SIZE, 0.0f);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glEnd();
			glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, OBJECT_SIZE, 0.0f);
			glVertex3f(OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glEnd();
			glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, OBJECT_SIZE, 0.0f);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE);
			glVertex3f(-OBJECT_SIZE, -OBJECT_SIZE, OBJECT_SIZE);
			glEnd();
		}
	}
	else if (m_Object_Type == SPHERE_OBJECT) glutSolidSphere(OBJECT_SIZE * 0.5f, 50, 50);

	glPopMatrix();
}

void Object::Update()
{
	Auto_Move();
}

void Object::Collision_Check(Object* other)
{
	if (m_OBB != nullptr && other->Get_OBB() != nullptr)
	{
		if (m_OBB->OBB_Intersection(other->Get_OBB())) Collision_Procedure();
	}
}

void Object::Collision_Procedure()
{
	cout << "Ãæµ¹ " << count++ << endl;
}

void Object::Auto_Move()
{
	if (m_Object_Type == SPHERE_OBJECT)
	{
		m_Sphere_Rot_Value += 3.0f;

		if (m_Sphere_Rot_Value >= 360.0f || m_Sphere_Rot_Value <= -360.0f)
		{
			int a = m_Sphere_Rot_Value / 360.0f;
			m_Sphere_Rot_Value = m_Sphere_Rot_Value - a * 360;
		}
	}

	else if (m_Object_Type == SNOW_OBJECT && m_is_Snowing)
	{
		m_Position.y -= m_Snow_Falling_Speed;
		if (m_Position.y <= -OBJECT_SIZE)
		{
			m_Position.y = SNOW_INIT_POSITION_Y;
			m_is_Touched_Floor = true;
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
