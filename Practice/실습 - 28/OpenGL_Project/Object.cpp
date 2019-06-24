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

void Object::Draw_Line(const int& point_count, const int& start_index)
{
	glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, point_count, &m_Control_Points[start_index][0]);
	glEnable(GL_MAP1_VERTEX_3);
	glBegin(GL_LINE_STRIP);
	for (int j = 0; j <= 10; ++j) glEvalCoord1f(j * 0.1f);
	glEnd();
	glDisable(GL_MAP1_VERTEX_3);
}

void Object::Draw()
{
	glPushMatrix();

	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glMultMatrixf(m_Rotate_Transform_Matrix);
	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

	// ===============================
	int count = m_CP_Count % 4;

	if (m_CP_Count == 2 || m_CP_Count == 3) Draw_Line(count, 0);
	else if (m_CP_Count == 4) Draw_Line(4, 0);
	else if (m_CP_Count == 5 || m_CP_Count == 6) { Draw_Line(4, 0); Draw_Line(count+1, 3); }
	else if (m_CP_Count == 7) { Draw_Line(4, 0); Draw_Line(4, 3); }
	else if (m_CP_Count == 8 || m_CP_Count == 9) { Draw_Line(4, 0); Draw_Line(4, 3); Draw_Line(count+2, 6); }
	else if (m_CP_Count == 10) { Draw_Line(4, 0); Draw_Line(4, 3); Draw_Line(4, 6); }
	else if (m_CP_Count == 11)
	{
		Draw_Line(4, 0); Draw_Line(4, 3); Draw_Line(4, 6); Draw_Line(2, 9);
	}
	else if (m_CP_Count == 12)
	{
		Draw_Line(4, 0); Draw_Line(4, 3); Draw_Line(4, 6); Draw_Line(3, 9);
	}
	else if (m_CP_Count == 13)
	{
		Draw_Line(4, 0); Draw_Line(4, 3); Draw_Line(4, 6); Draw_Line(4, 9);
	}
	else if (m_CP_Count == 14 || m_CP_Count == 15)
	{
		Draw_Line(4, 0); Draw_Line(4, 3); Draw_Line(4, 6); Draw_Line(4, 9); Draw_Line(count, 12);
	}
	else if (m_CP_Count == 16)
	{
		Draw_Line(4, 0); Draw_Line(4, 3); Draw_Line(4, 6); Draw_Line(4, 9); Draw_Line(4, 12);
	}
	else if (m_CP_Count == 17 || m_CP_Count == 18)
	{
		Draw_Line(4, 0); Draw_Line(4, 3); Draw_Line(4, 6); Draw_Line(4, 9); Draw_Line(4, 12); Draw_Line(count+1, 15);
	}
	else if (m_CP_Count == 19)
	{
		Draw_Line(4, 0); Draw_Line(4, 3); Draw_Line(4, 6); Draw_Line(4, 9); Draw_Line(4, 12); Draw_Line(4, 15);
	}
	// ===============================

	glPointSize(5.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < m_CP_Count; ++i) glVertex3fv(&m_Control_Points[i][0]);
	glEnd();

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
	cout << "�浹 " << count++ << endl;
}

void Object::Collision_Procedure(const Vector4& collider_Normal_Vector)
{
	cout << "�浹 " << count++ << endl;
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
}

int Object::Find_Near_CP(const int& x, const int& y)
{
	for (int i = 0; i < m_CP_Count; ++i)
	{
		if (m_Control_Points[i][0] + 3.0f >= x && m_Control_Points[i][0] - 3.0f <= x
			&& m_Control_Points[i][1] + 3.0f >= y && m_Control_Points[i][1] - 3.0f <= y)
		{
			return i;
		}
	}
	return -1;
}

void Object::CP_Move(const int& index, const int& x, const int& y)
{
	m_Control_Points[index][0] = x;
	m_Control_Points[index][1] = y;
}


// ================================================



