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
/*
void Object::Draw_Line(const int& point_count, const int& start_index)
{
	glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, point_count, &m_Control_Points[start_index][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glBegin(GL_LINE_STRIP);
	for (int j = 0; j <= 10; ++j) glEvalCoord1f(j * 0.1f);
	glEnd();
	glDisable(GL_MAP1_VERTEX_3);
}
*/
void Object::Draw()
{
	glPushMatrix();

	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glMultMatrixf(m_Rotate_Transform_Matrix);
	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

	// ===============================
	if (!m_is_Make_Over)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_TRIANGLE_STRIP);
		for (auto& vertex : m_Polygon_Vertice)
			glVertex3f(vertex.x, vertex.y, vertex.z);
		glEnd();
	}
	else
	{
		glColor3f(0.0f, 0.0f, 0.0f);
		glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, 3, 7, 0.0f, 1.0f, 21, 4, &m_Control_Points[0][0][0]);
		glEnable(GL_MAP2_VERTEX_3);
		glMapGrid2f(10, 0.0f, 1.0f, 10, 0.0f, 1.0f);
		glEvalMesh2(m_Plane_Type, 0, 10, 0, 10);
		glDisable(GL_MAP2_VERTEX_3);
	}
	

	glPointSize(5.0f); glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 7; ++j)
			if (m_Control_Points[i][j][0] != -99999.0f && m_Control_Points[i][j][2] != -99999.0f)
				glVertex3fv(m_Control_Points[i][j]);
	glEnd();
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

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 7; ++j)
		{
			m_Control_Points_Y_Sign[i][j] = (i + j) % 2 == 0 ? 1 : -1;

			for (int k = 0; k < 3; ++k)
			{
				if (k == 1) { continue; }
				m_Control_Points[i][j][k] = -99999.0f;
			}
		}
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
	if (m_is_Make_Over && m_is_Auto_Moving)
	{
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 7; ++j)
			{
				if ((i == 0 && j == 0) || (i == 3 && j == 0) || (i == 0 && j == 6) || (i == 3 && j == 6))
					continue;
				if (m_Control_Points[i][j][1] > 200.0f)
				{
					m_Control_Points[i][j][1] = 200.0f;
					m_Control_Points_Y_Sign[i][j] *= -1;
				}
				else if (m_Control_Points[i][j][1] < -200.0f)
				{
					m_Control_Points[i][j][1] = -200.0f;
					m_Control_Points_Y_Sign[i][j] *= -1;
				}
				else m_Control_Points[i][j][1] += 1.5f * m_Control_Points_Y_Sign[i][j];
			}
		}
	}
}

Vector2 Object::Find_Near_CP(const int& x, const int& y)
{
	Vector2 v;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			if (m_Control_Points[i][j][0] + 10.0f >= x && m_Control_Points[i][j][0] - 10.0f <= x
				&& m_Control_Points[i][j][2] + 10.0f >= -y && m_Control_Points[i][j][2] - 10.0f <= -y)
			{
				v.x = i; v.y = j; return v;
			}
		}
	}
	v.x = -1; v.y = -1;
	return v;
}

void Object::CP_Move(const Vector2& index, const int& x, const int& y)
{
	m_Control_Points[index.x][index.y][0] = x;
	m_Control_Points[index.x][index.y][1] = y;
}


// ================================================



