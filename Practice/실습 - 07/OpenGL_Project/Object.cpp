#include "Object.h"

Object::Object(const GLenum type, vector<Vertex2D>& v_vertexs, const int index, const float pos_x, const float pos_y, const float speed, const bool through_edge)
{
	Set_Drawing_Type(type);
	Set_Vertexs_vector(v_vertexs);
	Set_Index(index);
	Set_Position(pos_x, pos_y);
	Set_Speed(speed);
	Set_is_Through_Edge(through_edge);
}

Object::~Object()
{
}

void Object::Draw()
{
	glPointSize(3.0f);

	glBegin(m_Drawing_Type);

	for (auto& data : m_Vertexs)
	{
		glColor3f(data.color.R, data.color.G, data.color.B);
		glVertex2f(data.point_x, data.point_y);
	}

	glEnd();
}

void Object::Auto_ColorScale()
{
	m_Vertexs[0].color.R += COLOR_SCALE_VALUE_R * m_Color_Scale_Sign_R;
	if (m_Vertexs[0].color.R >= 1.0f || m_Vertexs[0].color.R <= 0.0f) { m_Color_Scale_Sign_R *= -1; }
	m_Vertexs[0].color.G += COLOR_SCALE_VALUE_G * m_Color_Scale_Sign_G;
	if (m_Vertexs[0].color.G >= 1.0f || m_Vertexs[0].color.G <= 0.0f) { m_Color_Scale_Sign_G *= -1; }
	m_Vertexs[0].color.B += COLOR_SCALE_VALUE_B * m_Color_Scale_Sign_B;
	if (m_Vertexs[0].color.B >= 1.0f || m_Vertexs[0].color.B <= 0.0f) { m_Color_Scale_Sign_B *= -1; }

	for (int i = 1; i < m_Vertexs.size(); ++i)
	{
		m_Vertexs[i].color.R = m_Vertexs[0].color.R;
		m_Vertexs[i].color.G = m_Vertexs[0].color.G;
		m_Vertexs[i].color.B = m_Vertexs[0].color.B;
	}
}

void Object::Auto_Scale()
{
	m_Radius += m_Speed;
	if (m_Radius >= RADIUS_MAX_VALUE && !m_is_Through_Edge) m_is_Max_Radius = true;
	else if (m_Radius >= THROUDH_EDGE_RADIUS_MAX_VALUE && m_is_Through_Edge) m_is_Destroyed = true;
	Scale_Refresh();
}

void Object::Scale_Refresh()
{
	int count = 0;
	for (auto& Data : m_Vertexs)
	{
		if (!m_is_Max_Radius)
		{
			Data.point_x += cos(count * 9) * m_Speed;
			Data.point_y += sin(count * 9) * m_Speed;
		}
		else
		{
			Data.point_x = m_Position_X;
			Data.point_y = m_Position_Y;
		}
		++count;
	}
	if (m_is_Max_Radius)
	{
		m_is_Max_Radius = false;
		m_Radius = 0.0f;
	}
}