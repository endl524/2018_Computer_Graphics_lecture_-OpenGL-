#pragma once
#include "Object.h"

class Spring : Object
{
private:
	vector<Vector3> m_Model_Vertice;

	bool m_is_Fire_Over = false;

public:
	Spring(const Vector3& pos) { Spring_Initialize(pos); };
	~Spring() {};

	float const Get_Last_Vertex_Position_Y() { return m_Model_Vertice.back().y; }
	void Set_Scale_Y(const float& value) { m_Scale_Y = value; }
	float const Get_Scale_Y() { return m_Scale_Y; }

	void Spring_Initialize(const Vector3& pos)
	{
		m_Object_Type = SPRING_OBJECT;

		m_Position.x = pos.x;
		m_Position.y = pos.y - OBJECT_SIZE;
		m_Position.z = pos.z;

		m_Color.R = 0.0f; m_Color.G = 0.0f; m_Color.B = 0.0f; m_Color.A = 1.0f;

		Make_Model_Vertice();
	}

	virtual void Make_Model_Vertice()
	{
		Vector3 vertex;

		for (int i = 0; i <= 3600; i += 10)
		{
			vertex.x = cos(i * 3.1416f / 180.0f) * SPRING_RADIUS;
			vertex.y = i * 0.01f;
			vertex.z = sin(i * 3.1416f / 180.0f) * SPRING_RADIUS;
			m_Model_Vertice.push_back(vertex);
		}
	}

	virtual void Draw()
	{
		glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);

		glPushMatrix();
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		
		glBegin(GL_LINE_STRIP);
		for (auto& Vertex : m_Model_Vertice)
			glVertex3f(Vertex.x, Vertex.y, Vertex.z);
		glEnd();

		glPopMatrix();
	}
};

