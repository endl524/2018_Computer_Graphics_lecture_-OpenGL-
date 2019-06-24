#pragma once
#include "Object.h"

class Box : Object
{
private:
	vector<vector<Vertex3D>> m_Model_Planes;
	Vector4 m_Direction;
	float m_Transform[16];
	bool m_is_Collide = false;
	bool m_is_Hold = false;

public:
	Box() : Object(true) { Box_Initialize(); };
	~Box() {};

	virtual void Set_Position_Y(const float& value) { m_Position.y = value; }
	virtual float const Get_Position_Y() { return m_Position.y; }
	virtual void Set_Position_Z(const float& value) { m_Position.z = value; }
	virtual float const Get_Position_Z() { return m_Position.z; }
	virtual void Set_Scale(const float& x, const float& y, const float& z) { m_Scale_X = x; m_Scale_Y = y; m_Scale_Z = z; }

	virtual void Set_OBB() { m_OBB->OBB_Init(m_Position, m_Scale_X, m_Scale_Y, m_Scale_Z); }

	void Set_is_Hold(const bool& b) { m_is_Hold = b; }
	void Set_Pos_to_Collider_Pos() 
	{
		m_Position.x = m_OBB->Get_Center_Position()[0];
		m_Position.y = m_OBB->Get_Center_Position()[1];
		m_Position.z = m_OBB->Get_Center_Position()[2];
	}

	void Box_Initialize()
	{
		m_Object_Type = BOX_OBJECT;
		m_is_Static_Object = false;
		m_is_Collide = false;

		m_Direction.x = 0.0f; m_Direction.y = -1.0f; m_Direction.z = 0.0f; m_Direction.w = 1.0f;

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

		/* // 충돌체 그리기
		glPushMatrix();
		glTranslatef(m_OBB->Get_Center_Position()[0], m_OBB->Get_Center_Position()[1], m_OBB->Get_Center_Position()[2]);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_OBB->Get_OBB_Axis_Distance()[0] / (OBJECT_SIZE * 0.5f), m_OBB->Get_OBB_Axis_Distance()[1] / (OBJECT_SIZE * 0.5f), m_OBB->Get_OBB_Axis_Distance()[2] / (OBJECT_SIZE * 0.5f));
		glutWireCube(OBJECT_SIZE);
		glPopMatrix();
		*/
	}

	void Auto_Move()
	{
		m_Position.x += m_Direction.x * 2.0f;
		m_Position.y += m_Direction.y * 2.0f;
		m_Position.z += m_Direction.z * 2.0f;

		m_OBB->Set_Center_Position(m_Position);
	}

	virtual void Collision_Check(Object* other)
	{
		if (!m_is_Static_Object)
		{
			if (other->Get_Object_Type() == CUBE_OBJECT && !m_is_Hold)
			{
				int collide_Count = 0;

				for (int i = 0; i < 6; ++i)
				{
					if (m_OBB->OBB_Intersection(other->Get_Cube_Collider(i)))
					{
						Collision_Procedure(other->Get_Collider_Normal_Vector(i));
						++collide_Count;
					}
				}

				if (collide_Count == 0) m_is_Collide = false;
			}
		}
	}

	void Collision_Procedure(const Vector4& collider_Normal_Vector)
	{
		if (!m_is_Static_Object)
		{
			m_Position.x += collider_Normal_Vector.x;
			m_Position.y += collider_Normal_Vector.y;
			m_Position.z += collider_Normal_Vector.z;

			m_OBB->Set_Center_Position(m_Position);
			
			m_is_Collide = true;
		}
	}

	virtual void Update()
	{
		if (!m_is_Collide && !m_is_Hold) Auto_Move();
	}

	void Box_Roll(const int& relative_mouse_x)
	{
		Roll(relative_mouse_x * 0.2f);

		glPushMatrix();
		glLoadIdentity();
		glRotatef(relative_mouse_x * 0.2f, 0.0f, 0.0f, -1.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX, m_Transform);
		glPopMatrix();
		m_OBB->Multiply_Rotate_Matrix_to_OBB_Position(m_Transform);

		Set_Pos_to_Collider_Pos();
	}
};

