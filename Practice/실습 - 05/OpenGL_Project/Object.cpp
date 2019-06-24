#include "Object.h"

float Object::s_Main_Speed = 1.0f;

Object::Object(const GLenum type, const int shape_Type, vector<Vertex2D>& v_vertexs, const int index, const float pos_x, const float pos_y, const float size_x, const float size_y, const float dir_x, const float dir_y, const float speed)
{
	Set_Drawing_Type(type);
	Set_Shape_Type(shape_Type);
	Set_Vertexs_vector(v_vertexs);
	Set_Index(index);
	Set_Position(pos_x, pos_y);
	Set_Size(size_x, size_y);
	Set_Direction(dir_x, dir_y);
	Set_Speed(speed);
}

Object::~Object()
{
}

void Object::Draw()
{
	glBegin(m_Drawing_Type);

	for (auto& data : m_Vertexs)
	{
		glColor3f(data.color.R, data.color.G, data.color.B);
		glVertex2f(data.point_x, data.point_y);
	}

	glEnd();
}

void Object::Auto_Move()
{
	if (m_Position_X - m_Size_X * 0.5f < 0.0f)
	{
		m_Position_X = m_Size_X * 0.5f;
		m_Direction_X *= -1;
	}
	else if (m_Position_X + m_Size_X * 0.5f > INIT_WINDOW_SIZE_X)
	{
		m_Position_X = INIT_WINDOW_SIZE_X - m_Size_X * 0.5f;
		m_Direction_X *= -1;
	}
	if (m_Position_Y - m_Size_Y * 0.5f <= 0.0f)
	{
		m_Position_Y = m_Size_Y * 0.5f;
		m_Direction_Y *= -1;
	}
	else if (m_Position_Y + m_Size_Y * 0.5f >= INIT_WINDOW_SIZE_Y)
	{
		m_Position_Y = INIT_WINDOW_SIZE_Y - m_Size_Y * 0.5f;
		m_Direction_Y *= -1;
	}

	m_Position_X += m_Direction_X * m_Speed * s_Main_Speed;
	m_Position_Y += m_Direction_Y * m_Speed * s_Main_Speed;
}

void Object::Auto_Scale()
{
	m_Vertexs[0].color.R += COLOR_SCALE_VALUE_R * m_Color_Scale_Sign_R;
	if (m_Vertexs[0].color.R >= 1.0f || m_Vertexs[0].color.R <= 0.0f) { m_Color_Scale_Sign_R *= -1; }
	m_Vertexs[0].color.G += COLOR_SCALE_VALUE_G * m_Color_Scale_Sign_G;
	if (m_Vertexs[0].color.G >= 1.0f || m_Vertexs[0].color.G <= 0.0f) { m_Color_Scale_Sign_G *= -1; }
	m_Vertexs[0].color.B += COLOR_SCALE_VALUE_B * m_Color_Scale_Sign_B;
	if (m_Vertexs[0].color.B >= 1.0f || m_Vertexs[0].color.B <= 0.0f) { m_Color_Scale_Sign_B *= -1; }

	m_Size_X += SCALE_VALUE_X * m_Scale_Sign_X;
	if (m_Size_X >= RECT_SIZE_MAX_X || m_Size_X <= RECT_SIZE_MIN_X) { m_Scale_Sign_X *= -1; }
	m_Size_Y += SCALE_VALUE_Y * m_Scale_Sign_Y;
	if (m_Size_Y >= RECT_SIZE_MAX_Y || m_Size_Y <= RECT_SIZE_MIN_Y) { m_Scale_Sign_Y *= -1; }

	int vertex_num;
	switch (m_Shape_Type)
	{
	case SHAPE_TYPE_TRIANGLE: 
		vertex_num = 3;
		m_Vertexs[0].point_x = m_Position_X;
		m_Vertexs[0].point_y = m_Position_Y + m_Size_Y * 0.5f;
		m_Vertexs[1].point_x = m_Position_X - m_Size_X * 0.5f;
		m_Vertexs[1].point_y = m_Position_Y - m_Size_Y * 0.5f;
		m_Vertexs[2].point_x = m_Position_X + m_Size_X * 0.5f;
		m_Vertexs[2].point_y = m_Position_Y - m_Size_Y * 0.5f;
		break;
	case SHAPE_TYPE_RECTANGLE: 
		vertex_num = 4;
		m_Vertexs[0].point_x = m_Position_X - m_Size_X * 0.5f;
		m_Vertexs[0].point_y = m_Position_Y + m_Size_Y * 0.5f;
		m_Vertexs[1].point_x = m_Position_X - m_Size_X * 0.5f;
		m_Vertexs[1].point_y = m_Position_Y - m_Size_Y * 0.5f;
		m_Vertexs[2].point_x = m_Position_X + m_Size_X * 0.5f;
		m_Vertexs[2].point_y = m_Position_Y - m_Size_Y * 0.5f;
		m_Vertexs[3].point_x = m_Position_X + m_Size_X * 0.5f;
		m_Vertexs[3].point_y = m_Position_Y + m_Size_Y * 0.5f;
		break;
	}

	for (int i = 1; i < vertex_num; ++i)
	{
		m_Vertexs[i].color.R = m_Vertexs[0].color.R;
		m_Vertexs[i].color.G = m_Vertexs[0].color.G;
		m_Vertexs[i].color.B = m_Vertexs[0].color.B;
	}

	

	
	
}

void Object::Change_Speed(const int sign)
{
	s_Main_Speed += MAIN_SPEED_RELATIVE_VALUE * sign;
}