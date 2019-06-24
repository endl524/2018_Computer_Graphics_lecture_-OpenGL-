#include "Object.h"

int Object::s_Object_Count = 0;
float Object::s_World_Rotate_Angle = 0.0f;
float Object::s_World_Rotate_X = 0.0f;
float Object::s_World_Rotate_Y = 0.0f;
float Object::s_World_Rotate_Z = 0.0f;
float Object::s_World_Scale_X = 0.0f;
float Object::s_World_Scale_Y = 0.0f;
float Object::s_World_Scale_Z = 0.0f;

bool Object::s_is_Scaled = false;
int Object::s_Correction_Processed_Count = 0;

Object::Object() {}

Object::Object(const bool& is_active)
{
	Set_Index(s_Object_Count++);
	Set_Active(is_active);
	Set_Random_Device();
}

Object::~Object() {}

void Object::Draw()
{
	if (m_is_Activated)
	{
		glLoadIdentity();

		glPushMatrix(); // 다음의 변환들을 스택에 넣는다.
		glScalef(1.0f + s_World_Scale_X, 1.0f + s_World_Scale_Y, 1.0f + s_World_Scale_Z);
		glRotatef(s_World_Rotate_Angle, s_World_Rotate_X, s_World_Rotate_Y, s_World_Rotate_Z);

		glPushMatrix(); // 다음의 변환들을 스택에 넣는다.

		// 로컬 변환 적용.
		glRotatef(m_Local_Rotate_Angle, m_Local_Rotate_X, m_Local_Rotate_Y, m_Local_Rotate_Z);
		glTranslatef(m_Local_Translate_X, m_Local_Translate_Y, m_Local_Translate_Z);
		glScalef(m_Local_Scale_X, m_Local_Scale_Y, m_Local_Scale_Z);

		glLineWidth(2.0f);
		glBegin(m_Drawing_Type); // 그리기 시작
		for (auto& data : m_Vertexs)
		{
			glColor3f(data.color.R, data.color.G, data.color.B);
			glVertex3f(data.point_x, data.point_y, data.point_z);
		}
		glEnd(); // 그리기 종료

		glPopMatrix(); // 스택의 top에 있는 변환들을 꺼낸다.
		glPopMatrix(); // 스택의 top에 있는 변환들을 꺼낸다.
	}
}

void Object::Set_Random_Device()
{
	mt19937_64 seed(m_Random_Device());
	m_Random_SEED = seed;

	uniform_real_distribution<> rand_Color(0.0f, 1.0f);
	m_Random_Color = rand_Color;
}

void Object::Set_Path_Line()
{
	m_Vertexs.clear();

	Vertex3D temp_vertex;

	Init_Object();

	m_Drawing_Type = GL_LINE_LOOP;
	m_Object_Type = OBJECT_TYPE_PATH_LINE;

	temp_vertex.color.R = 0.5f;
	temp_vertex.color.G = 0.5f;
	temp_vertex.color.B = 1.0f;
	temp_vertex.point_x = 0.0f;
	temp_vertex.point_y = OBJECT_SIZE;
	m_Vertexs.push_back(temp_vertex);
	temp_vertex.color.R = 1.0f;
	temp_vertex.color.G = 0.5f;
	temp_vertex.color.B = 0.5f;
	temp_vertex.point_x = -OBJECT_SIZE;
	temp_vertex.point_y = -OBJECT_SIZE;
	m_Vertexs.push_back(temp_vertex);
	temp_vertex.color.R = 0.5f;
	temp_vertex.color.G = 1.0f;
	temp_vertex.color.B = 0.5f;
	temp_vertex.point_x = OBJECT_SIZE;
	temp_vertex.point_y = -OBJECT_SIZE;
	m_Vertexs.push_back(temp_vertex);
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

	m_is_Auto_Moving = true;
}

void Object::Set_Moving_Object(Object* path_line)
{
	m_Path_Line_Object = path_line;
	m_Curr_Vertex_iter = m_Path_Line_Object->m_Vertexs.begin();
	m_Drawing_Type = GL_TRIANGLES;
	m_Object_Type = OBJECT_TYPE_MOVING_OBJECT;

	Init_Object();

	Vertex3D temp_vertex;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);

	temp_vertex.point_x = 0.0f;
	temp_vertex.point_y = OBJECT_SIZE;
	m_Vertexs.push_back(temp_vertex);
	temp_vertex.point_x = -OBJECT_SIZE;
	temp_vertex.point_y = -OBJECT_SIZE;
	m_Vertexs.push_back(temp_vertex);
	temp_vertex.point_x = OBJECT_SIZE;
	temp_vertex.point_y = -OBJECT_SIZE;
	m_Vertexs.push_back(temp_vertex);
}

void Object::Auto_ColorScale()
{
	if (m_is_Auto_Scaling)
	{
		m_Vertexs.front().color.R += COLOR_SCALE_VALUE_R * m_Color_Scale_Sign_R;
		if (m_Vertexs.front().color.R >= 1.0f || m_Vertexs.front().color.R <= 0.0f) { m_Color_Scale_Sign_R *= -1; }
		m_Vertexs.front().color.G += COLOR_SCALE_VALUE_G * m_Color_Scale_Sign_G;
		if (m_Vertexs.front().color.G >= 1.0f || m_Vertexs.front().color.G <= 0.0f) { m_Color_Scale_Sign_G *= -1; }
		m_Vertexs.front().color.B += COLOR_SCALE_VALUE_B * m_Color_Scale_Sign_B;
		if (m_Vertexs.front().color.B >= 1.0f || m_Vertexs.front().color.B <= 0.0f) { m_Color_Scale_Sign_B *= -1; }

		for (auto& Data : m_Vertexs)
			Data.color = m_Vertexs.front().color;

	}
}

void Object::Auto_Translate()
{
	if (m_is_Auto_Moving)
	{
		if (s_is_Scaled)
		{
			m_Vertexs_Distance_X = m_Curr_Vertex_iter->point_x * m_Path_Line_Object->m_Local_Scale_X - m_Local_Translate_X;
			m_Vertexs_Distance_Y = m_Curr_Vertex_iter->point_y * m_Path_Line_Object->m_Local_Scale_Y - m_Local_Translate_Y;
			m_Distance_Length = sqrtf(m_Vertexs_Distance_X * m_Vertexs_Distance_X + m_Vertexs_Distance_Y * m_Vertexs_Distance_Y);
			m_Auto_Translate_Value_X = m_Vertexs_Distance_X / m_Distance_Length * OBJECT_AUTO_MOVE_SPEED;
			m_Auto_Translate_Value_Y = m_Vertexs_Distance_Y / m_Distance_Length * OBJECT_AUTO_MOVE_SPEED;
			++s_Correction_Processed_Count;
			if (s_Correction_Processed_Count >= 2)
			{
				s_is_Scaled = true;
				s_Correction_Processed_Count = 0;
			}
		}

		if (roundf(m_Curr_Vertex_iter->point_x * m_Path_Line_Object->m_Local_Scale_X) - 5.0f <= roundf(m_Local_Translate_X) &&
			roundf(m_Curr_Vertex_iter->point_x * m_Path_Line_Object->m_Local_Scale_X) + 5.0f >= roundf(m_Local_Translate_X) &&
			roundf(m_Curr_Vertex_iter->point_y * m_Path_Line_Object->m_Local_Scale_Y) - 5.0f <= roundf(m_Local_Translate_Y) &&
			roundf(m_Curr_Vertex_iter->point_y * m_Path_Line_Object->m_Local_Scale_Y) + 5.0f >= roundf(m_Local_Translate_Y))
		{
			++m_Curr_Vertex_iter;

			if (m_Curr_Vertex_iter == m_Path_Line_Object->m_Vertexs.end())
			{
				m_Curr_Vertex_iter = m_Path_Line_Object->m_Vertexs.begin();
			}

			m_Vertexs_Distance_X = m_Curr_Vertex_iter->point_x * m_Path_Line_Object->m_Local_Scale_X - m_Local_Translate_X;
			m_Vertexs_Distance_Y = m_Curr_Vertex_iter->point_y * m_Path_Line_Object->m_Local_Scale_Y - m_Local_Translate_Y;
			m_Distance_Length = sqrtf(m_Vertexs_Distance_X * m_Vertexs_Distance_X + m_Vertexs_Distance_Y * m_Vertexs_Distance_Y);
			m_Auto_Translate_Value_X = m_Vertexs_Distance_X / m_Distance_Length * OBJECT_AUTO_MOVE_SPEED;
			m_Auto_Translate_Value_Y = m_Vertexs_Distance_Y / m_Distance_Length * OBJECT_AUTO_MOVE_SPEED;
		}

		else
		{
			m_Local_Translate_X += m_Auto_Translate_Value_X;
			m_Local_Translate_Y += m_Auto_Translate_Value_Y;
		}
	}
}

void Object::Go_To_Start_Point()
{
	if (m_Path_Line_Object != nullptr)
	{
		m_Curr_Vertex_iter = m_Path_Line_Object->m_Vertexs.begin();
		m_Local_Translate_X = m_Curr_Vertex_iter->point_x;
		m_Local_Translate_Y = m_Curr_Vertex_iter->point_y;
	}
}


void Object::World_Rotate(const int& sign)
{
	s_World_Rotate_Angle = s_World_Rotate_Angle + OBJECT_ROTATE_VALUE * sign < 360.0f && s_World_Rotate_Angle + OBJECT_ROTATE_VALUE * sign > -360.0f ? s_World_Rotate_Angle + OBJECT_ROTATE_VALUE * sign : 0.0f;
	s_World_Rotate_Y = 1.0f;
}

void Object::World_Scale(const int& sign)
{
	s_World_Scale_X = s_World_Scale_X + SCALE_VALUE * sign > -0.8f && s_World_Scale_X + SCALE_VALUE * sign < 1.0f ? s_World_Scale_X + SCALE_VALUE * sign : s_World_Scale_X;
	s_World_Scale_Y = s_World_Scale_Y + SCALE_VALUE * sign > -0.8f && s_World_Scale_Y + SCALE_VALUE * sign < 1.0f ? s_World_Scale_Y + SCALE_VALUE * sign : s_World_Scale_Y;
	s_World_Scale_Z = s_World_Scale_Z + SCALE_VALUE * sign > -0.8f && s_World_Scale_Z + SCALE_VALUE * sign < 1.0f ? s_World_Scale_Z + SCALE_VALUE * sign : s_World_Scale_Z;
}

void Object::Local_Scale(const int& sign)
{
	m_Local_Scale_X = m_Local_Scale_X + SCALE_VALUE * sign > 0.2f && m_Local_Scale_X + SCALE_VALUE * sign < 1.8f ? m_Local_Scale_X + SCALE_VALUE * sign : m_Local_Scale_X;
	m_Local_Scale_Y = m_Local_Scale_Y + SCALE_VALUE * sign > 0.2f && m_Local_Scale_Y + SCALE_VALUE * sign < 1.8f ? m_Local_Scale_Y + SCALE_VALUE * sign : m_Local_Scale_Y;
	m_Local_Scale_Z = m_Local_Scale_Z + SCALE_VALUE * sign > 0.2f && m_Local_Scale_Z + SCALE_VALUE * sign < 1.8f ? m_Local_Scale_Z + SCALE_VALUE * sign : m_Local_Scale_Z;
	s_is_Scaled = true;
}

void Object::Update()
{
	Auto_Translate();
	Auto_ColorScale();
}