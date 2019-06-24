#include "Object.h"

int Object::s_Object_Count = 0;

Object::Object(const GLenum& type, vector<Vertex2D>& v_vertexs, const float& pos_x, const float& pos_y, const float& size_x, const float& size_y, const bool& is_active, const int& obj_type)
{
	Set_Drawing_Type(type);
	Set_Vertexs_vector(v_vertexs);
	Set_Index(s_Object_Count++);
	Set_Position(pos_x, pos_y);
	Set_Size(size_x, size_y);
	Set_Active(is_active);
	Set_Type(obj_type);
}

Object::~Object()
{
}

void Object::Draw()
{
	if (m_is_Activated)
	{
		glBegin(m_Drawing_Type);

		for (auto& data : m_Vertexs)
		{
			glColor3f(data.color.R, data.color.G, data.color.B);
			glVertex2f(data.point_x, data.point_y);
		}

		glEnd();
	}
}

void Object::Auto_ColorScale()
{
	m_Vertexs[0].color.R += COLOR_SCALE_VALUE_R * m_Color_Scale_Sign_R;
	if (m_Vertexs[0].color.R >= 1.0f || m_Vertexs[0].color.R <= 0.0f) { m_Color_Scale_Sign_R *= -1; }
	m_Vertexs[0].color.G += COLOR_SCALE_VALUE_G * m_Color_Scale_Sign_G;
	if (m_Vertexs[0].color.G >= 1.0f || m_Vertexs[0].color.G <= 0.0f) { m_Color_Scale_Sign_G *= -1; }
	m_Vertexs[0].color.B += COLOR_SCALE_VALUE_B * m_Color_Scale_Sign_B;
	if (m_Vertexs[0].color.B >= 1.0f || m_Vertexs[0].color.B <= 0.0f) { m_Color_Scale_Sign_B *= -1; }

	for (int i = 1; i < 4; ++i)
	{
		m_Vertexs[i].color.R = m_Vertexs[0].color.R;
		m_Vertexs[i].color.G = m_Vertexs[0].color.G;
		m_Vertexs[i].color.B = m_Vertexs[0].color.B;
	}
}

void Object::Position_Update()
{
	m_Vertexs[0].point_x = m_Position_X - m_Size_X * 0.5f;
	m_Vertexs[0].point_y = m_Position_Y + m_Size_Y * 0.5f;
	m_Vertexs[1].point_x = m_Position_X - m_Size_X * 0.5f;
	m_Vertexs[1].point_y = m_Position_Y - m_Size_Y * 0.5f;
	m_Vertexs[2].point_x = m_Position_X + m_Size_X * 0.5f;
	m_Vertexs[2].point_y = m_Position_Y - m_Size_Y * 0.5f;
	m_Vertexs[3].point_x = m_Position_X + m_Size_X * 0.5f;
	m_Vertexs[3].point_y = m_Position_Y + m_Size_Y * 0.5f;
}

void Object::Collision_Check(const map<int, Object*>& object_list)
{
	for (auto& object : object_list)
	{
		if (!object.second->Get_is_Destroyed())
		{
			if (m_Position_X - m_Size_X * 0.5f <= object.second->Get_Position_X() + object.second->Get_Size_X() * 0.5f
				&& m_Position_X + m_Size_X * 0.5f >= object.second->Get_Position_X() - object.second->Get_Size_X() * 0.5f
				&& m_Position_Y - m_Size_Y * 0.5f <= object.second->Get_Position_Y() + object.second->Get_Size_Y() * 0.5f
				&& m_Position_Y + m_Size_Y * 0.5f >= object.second->Get_Position_Y() - object.second->Get_Size_Y() * 0.5f)
			{
				object.second->Set_is_Destroyed(true);
			}
		}
	}
}