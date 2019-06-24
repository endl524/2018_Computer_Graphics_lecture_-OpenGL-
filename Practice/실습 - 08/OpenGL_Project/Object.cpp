#include "Object.h"

Object::Object(const GLenum type, vector<Vertex2D>& v_vertexs, const int index, const float pos_x, const float pos_y, const float speed, const int round_num, const bool is_Clockwise)
{
	Set_Drawing_Type(type);
	Set_Vertexs_vector(v_vertexs);
	Set_Index(index);
	Set_Position(pos_x, pos_y);
	Set_Speed(speed);
	Set_Round_Number(round_num);
	Set_is_Clockwise(is_Clockwise);

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

void Object::Auto_Drawing()
{
	if (m_New_Vertex_Time >= DRAW_NEW_VERTEX_TIME && !m_is_Draw_Over[1])
	{
		Set_New_Vertex();
		m_New_Vertex_Time = 0.0f;
	}
	else
	{
		m_New_Vertex_Time += m_Speed;
		m_Forward_Move_Distance += m_is_Draw_Over[0] ? -m_Speed : m_Speed;
	}
}

void Object::Set_New_Vertex()
{
	Vertex2D temp_Vertex;
	auto last_Vertex = m_Vertexs.rbegin();

	temp_Vertex.angle = m_is_Clockwise ? last_Vertex->angle + DRAW_NEW_VERTEX_ANGLE : last_Vertex->angle - DRAW_NEW_VERTEX_ANGLE;
	
	if (!m_is_Draw_Over[0])
	{
		if ((m_is_Clockwise && temp_Vertex.angle > 180.0f * m_Round_Number) || (!m_is_Clockwise && temp_Vertex.angle < -180.0f * m_Round_Number))
		{
			if (m_is_Clockwise) m_is_Clockwise = false;
			else m_is_Clockwise = true;
			temp_Vertex.angle = 0.0f;
			m_is_Draw_Over[0] = true;
		}
	}

	else if (m_is_Draw_Over[0] && !m_is_Draw_Over[1])
	{
		if ((m_is_Clockwise && temp_Vertex.angle > 180.0f * m_Round_Number) || (!m_is_Clockwise && temp_Vertex.angle < -180.0f * m_Round_Number))
		{
			m_is_Draw_Over[1] = true;
		}
	}
	temp_Vertex.point_x = last_Vertex->point_x + sin(temp_Vertex.angle / 0.0175f) * m_Forward_Move_Distance;
	temp_Vertex.point_y = last_Vertex->point_y + cos(temp_Vertex.angle / 0.0175f) * m_Forward_Move_Distance; 

	m_Vertexs.push_back(temp_Vertex);
}