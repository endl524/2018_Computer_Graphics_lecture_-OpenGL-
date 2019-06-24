#include "Object.h"

int Object::s_Object_Count = 0;

Object::Object()
{
	Set_Index(s_Object_Count++);
	Init_Object();
}

Object::~Object() 
{
	if (m_Model != nullptr)
	{
		gluDeleteQuadric(m_Model);
		m_Model = nullptr;
	}
}

void Object::Draw()
{
	glPushMatrix(); // 다음의 변환들을 스택에 넣는다.

	// 변환 적용.
	
	if (m_Parent_Rail != nullptr)
	{
		glRotatef(m_Parent_Rail->Get_Rotate_Roll(), 0.0f, 0.0f, 1.0f);
		if (m_is_Small_Sphere)
		{
			if (m_Parent_Rail2 != nullptr) glRotatef(m_Parent_Rail2->Get_Rotate_Roll(), 0.0f, 0.0f, 1.0f);
			glTranslatef(m_Parent_Rail->Get_Position_X(), m_Parent_Rail->Get_Position_Y(), m_Parent_Rail->Get_Position_Z());
		}
	}

	glTranslatef(m_Position.x, m_Position.y, m_Position.z);
	glRotatef(m_Rotate_Pitch, 1.0f, 0.0f, 0.0f);
	glRotatef(m_Rotate_Yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(m_Rotate_Roll, 0.0f, 0.0f, 1.0f);
	
	glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

	glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

	switch (m_Object_Type)
	{
	case OBJECT_TYPE_SPHERE:
		gluSphere(m_Model, OBJECT_SIZE * 0.5f, 30, 30);
		break;

	case OBJECT_TYPE_RAIL:
		glBegin(GL_LINE_LOOP);
		for (auto& data : m_Rail_Vertice) glVertex3f(data.x, data.y, data.z);
		glEnd();
		break;
	}

	glPopMatrix(); // 스택의 top에 있는 변환들을 꺼낸다.
}



void Object::Init_Object()
{
	m_Move_Speed = 0.0f;

	m_Position.x = 0.0f;
	m_Position.y = 0.0f;
	m_Position.z = 0.0f;

	m_Rotate_Roll = 0.0f;
	m_Rotate_Yaw = 0.0f;
	m_Rotate_Pitch = 0.0f;

	m_Scale_X = 1.0f;
	m_Scale_Y = 1.0f;
	m_Scale_Z = 1.0f;

	m_Object_Type = OBJECT_TYPE_CUBE;
}

void Object::Set_Up_Rail(const float& radius)
{
	Vector3 temp_Vertex;

	for (int i = 0; i < RAIL_VERTICE_COUNT; ++i)
	{
		temp_Vertex.x = cos(i * 3.1416f / 180.0f) * radius;
		temp_Vertex.z = sin(i * 3.1416f / 180.0f) * radius;
		m_Rail_Vertice.push_back(temp_Vertex);
	}
}

void Object::Set_Up_Model()
{
	m_Model = gluNewQuadric();
	gluQuadricDrawStyle(m_Model, GLU_FILL);
	gluQuadricNormals(m_Model, GLU_SMOOTH);
	gluQuadricOrientation(m_Model, GLU_OUTSIDE);
}

void Object::Change_Model_Draw_Style(const bool& is_Solid)
{
	if (is_Solid) gluQuadricDrawStyle(m_Model, GLU_FILL);
	else gluQuadricDrawStyle(m_Model, GLU_LINE);
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
	if (m_Parent_Rail != nullptr)
	{
		m_Next_Vertex_index = m_Next_Vertex_index + m_Move_Speed >= RAIL_VERTICE_COUNT ? m_Next_Vertex_index + m_Move_Speed - RAIL_VERTICE_COUNT : m_Next_Vertex_index + m_Move_Speed;
		m_Position = m_Parent_Rail->Get_Rail_Vertex(m_Next_Vertex_index);
	}
}

void Object::Update()
{
	Auto_Move();
}