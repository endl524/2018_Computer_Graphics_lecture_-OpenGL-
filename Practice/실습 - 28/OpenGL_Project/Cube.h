#pragma once
#include "Object.h"

class Cube : Object
{
private:
	vector<vector<Vertex3D>> m_Model_Planes;
	vector<Vertex3D> m_Door_Plane;

	OBB* m_Cube_Collider[4]{ nullptr };

	float m_Door_Rotation_Value = 0.0f;
	bool m_is_Door_Open = false;
	bool m_is_Door_Production = false;

public:
	Cube() : Object(true) { Cube_Initialize(); };
	~Cube()
	{
		if (m_Cube_Collider[0] != nullptr)
		{
			delete[] m_Cube_Collider;
			for (int i = 0; i < 4; ++i) m_Cube_Collider[i] = nullptr;
		}
	};

	virtual OBB* Get_Cube_Collider(const int& index) { return m_Cube_Collider[index]; };
	
	void Cube_Initialize()
	{
		m_Object_Type = CUBE_OBJECT;
		m_is_Static_Object = true;

		m_Scale_X = 20.0f; m_Scale_Y = 20.0f; m_Scale_Z = 20.0f;

		m_Position.y = OBJECT_SIZE * 0.5f * m_Scale_Y;

		Vector3 temp_Pos;

		m_Cube_Collider[0] = new OBB();
		temp_Pos = m_Position;
		temp_Pos.z += OBJECT_SIZE * 0.65f * m_Scale_Z;
		m_Cube_Collider[0]->OBB_Init(temp_Pos, m_Scale_X, m_Scale_Y, m_Scale_Z * 0.3f);
		
		m_Cube_Collider[1] = new OBB();
		temp_Pos = m_Position;
		temp_Pos.x += OBJECT_SIZE * 0.65f * m_Scale_X;
		m_Cube_Collider[1]->OBB_Init(temp_Pos, m_Scale_X * 0.3f, m_Scale_Y, m_Scale_Z);
		
		m_Cube_Collider[2] = new OBB();
		temp_Pos = m_Position;
		temp_Pos.z -= OBJECT_SIZE * 0.65f * m_Scale_Z;
		m_Cube_Collider[2]->OBB_Init(temp_Pos, m_Scale_X, m_Scale_Y, m_Scale_Z * 0.3f);
		
		m_Cube_Collider[3] = new OBB();
		temp_Pos = m_Position;
		temp_Pos.x -= OBJECT_SIZE * 0.65f * m_Scale_X;
		m_Cube_Collider[3]->OBB_Init(temp_Pos, m_Scale_X * 0.3f, m_Scale_Y, m_Scale_Z);
		
		Make_Model_Vertice();
	}

	virtual void Make_Model_Vertice()
	{
		Vertex3D vertex;
		vector<Vertex3D> vertice;

		// 1번 면
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = 0.0f;
		vertex.color.R = 0.6f;
		vertex.color.G = 0.6f;
		vertex.color.B = 0.6f;
		m_Door_Plane.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = 0.0f;
		m_Door_Plane.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = 0.0f;
		m_Door_Plane.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = 0.0f;
		m_Door_Plane.push_back(vertex);

		// 2번 면
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.5f;
		vertex.color.G = 0.5f;
		vertex.color.B = 0.5f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 3번 면
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.6f;
		vertex.color.G = 0.6f;
		vertex.color.B = 0.6f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 4번 면
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.5f;
		vertex.color.G = 0.5f;
		vertex.color.B = 0.5f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 5번 면
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.7f;
		vertex.color.G = 0.7f;
		vertex.color.B = 0.7f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 6번 면
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertex.color.R = 0.7f;
		vertex.color.G = 0.7f;
		vertex.color.B = 0.7f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = -OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();
	}


	virtual void Draw()
	{
		glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);

		

		glFrontFace(GL_CW);
		glPushMatrix();
		glRotatef(m_Door_Rotation_Value, -1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, OBJECT_SIZE * 0.5f * m_Scale_Z);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glBegin(GL_QUADS);
		for (auto& vertex : m_Door_Plane)
		{
			glColor4f(vertex.color.R, vertex.color.G, vertex.color.B, vertex.color.A);
			glVertex3f(vertex.x, vertex.y, vertex.z);
		}
		glEnd();
		glPopMatrix();

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
		glFrontFace(GL_CCW);

		glPopMatrix();

		 // 충돌체 그리기
		for (int i = 0; i < 4; ++i)
		{
			glPushMatrix();
			glTranslatef(m_Cube_Collider[i]->Get_Center_Position()[0], m_Cube_Collider[i]->Get_Center_Position()[1], m_Cube_Collider[i]->Get_Center_Position()[2]);
			glMultMatrixf(m_Rotate_Transform_Matrix);
			glScalef(m_Cube_Collider[i]->Get_OBB_Axis_Distance()[0] / (OBJECT_SIZE * 0.5f), m_Cube_Collider[i]->Get_OBB_Axis_Distance()[1] / (OBJECT_SIZE * 0.5f), m_Cube_Collider[i]->Get_OBB_Axis_Distance()[2] / (OBJECT_SIZE * 0.5f));
			glutWireCube(OBJECT_SIZE);
			glPopMatrix();
		}
		
	}


	void Door_Manage()
	{
		m_is_Door_Production = true;
		if (!m_is_Door_Open) m_is_Door_Open = true;
		else m_is_Door_Open = false;
	}

	void Door_Production()
	{
		if (m_is_Door_Production)
		{
			if (m_is_Door_Open)
			{
				if (m_Door_Rotation_Value >= 90.0f)
				{
					m_Door_Rotation_Value = 90.0f;
					m_is_Door_Production = false;
				}
				else
				{
					m_Door_Rotation_Value += 3.0f;
				}
			}
			else
			{
				if (m_Door_Rotation_Value <= 0.0f)
				{
					m_Door_Rotation_Value = 0.0f;
					m_is_Door_Production = false;
				}
				else
				{
					m_Door_Rotation_Value -= 3.0f;
				}
			}
		}
	}

	virtual void Update()
	{
		Door_Production();
	}
};

