#pragma once
#include "Object.h"

class Box : Object
{
private:
	vector<vector<Vertex3D>> m_Model_Planes;
	bool m_is_Draw = false;

public:
	Box() : Object(true) { Box_Initialize(); };
	~Box() {};
	
	void Draw_Switch() { if (m_is_Draw) m_is_Draw = false; else m_is_Draw = true; }

	void Box_Initialize()
	{
		m_Object_Type = BOX_OBJECT;
		m_is_Static_Object = true;

		Set_Scale(5.0f, 5.0f, 5.0f);
		Set_Translate(0.0f, OBJECT_SIZE * 0.5f * m_Scale_Y, 0.0f);

		Make_Model_Vertice();
	}

	virtual void Make_Model_Vertice()
	{
		Vertex3D vertex;
		vector<Vertex3D> vertice;

		// 1번 면
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 2번 면
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.3f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 3번 면
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.3f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.1f;
		vertex.color.G = 0.1f;
		vertex.color.B = 0.1f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 4번 면
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.1f;
		vertex.color.G = 0.1f;
		vertex.color.B = 0.1f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 5번 면
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 6번 면
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.1f;
		vertex.color.G = 0.1f;
		vertex.color.B = 0.1f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.3f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();
	}

	virtual void Draw()
	{
		if (m_is_Draw)
		{
			glPushMatrix();
			glTranslatef(m_Position.x, m_Position.y, m_Position.z);
			glMultMatrixf(m_Rotate_Transform_Matrix);
			glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

			glBegin(GL_QUADS);
			for (auto& plane : m_Model_Planes)
			{
				for (auto& vertex : plane)
				{
					glColor4f(vertex.color.R, vertex.color.G, vertex.color.B, vertex.color.A);
					glVertex3f(vertex.x, vertex.y, vertex.z);
				}
			}
			glEnd();
			glPopMatrix();
		}
	}
};

