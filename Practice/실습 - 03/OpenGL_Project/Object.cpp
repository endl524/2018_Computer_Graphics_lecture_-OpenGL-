#include "Object.h"

Object::Object(const GLenum type, vector<Vertex2D>& v_vertexs, const int index)
{
	Set_Drawing_Type(type);
	Set_Vertexs_vector(v_vertexs);
	Set_Index(index);
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