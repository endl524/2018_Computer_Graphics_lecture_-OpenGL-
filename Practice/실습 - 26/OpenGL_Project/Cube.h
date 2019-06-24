#pragma once
#include "Object.h"

class Cube : Object
{
private:
	vector<vector<Vertex3D>> m_Model_Planes;

	int m_Auto_Scale_Sign = 1;

	int m_Plane_Count = 0;
	
	float m_U_Cover_Rotate_Value = 0.0f;
	float m_F_Cover_Rotate_Value = 0.0f;

	bool m_is_U_Cover_Opening_End = true;
	bool m_is_U_Cover_Closing_End = true;
	bool m_is_U_Cover_Closed = true;
	bool m_is_F_Cover_Opening_End = true;
	bool m_is_F_Cover_Closing_End = true;
	bool m_is_F_Cover_Closed = true;

	OBB* m_Cube_Collider[6]{ nullptr };
	Vector4 m_Collider_Normal_Vector[6];
	float m_Transform[16];

public:
	Cube() : Object(true) { Cube_Initialize(); };
	~Cube() 
	{
		if (m_Cube_Collider[0] != nullptr)
		{
			delete[] m_Cube_Collider;
			for (int i = 0; i < 6; ++i) m_Cube_Collider[i] = nullptr;
		}
	};

	virtual OBB* Get_Cube_Collider(const int& index) { return m_Cube_Collider[index]; };
	virtual Vector4 const Get_Collider_Normal_Vector(const int& index) { return m_Collider_Normal_Vector[index]; }

	void Cube_Initialize()
	{
		m_Object_Type = CUBE_OBJECT;
		m_is_Static_Object = true;

		m_Scale_X = 4.0f; m_Scale_Y = 4.0f; m_Scale_Z = 4.0f;


		Vector3 temp_Pos;

		m_Cube_Collider[0] = new OBB();
		temp_Pos = m_Position;
		temp_Pos.z += OBJECT_SIZE * 0.65f * m_Scale_Z;
		m_Cube_Collider[0]->OBB_Init(temp_Pos, m_Scale_X, m_Scale_Y, m_Scale_Z * 0.3f);
		m_Collider_Normal_Vector[0].x = 0.0f; m_Collider_Normal_Vector[0].y = 0.0f; m_Collider_Normal_Vector[0].z = -1.0f;

		m_Cube_Collider[1] = new OBB();
		temp_Pos = m_Position;
		temp_Pos.x += OBJECT_SIZE * 0.65f * m_Scale_X;
		m_Cube_Collider[1]->OBB_Init(temp_Pos, m_Scale_X * 0.3f, m_Scale_Y, m_Scale_Z);
		m_Collider_Normal_Vector[1].x = -1.0f; m_Collider_Normal_Vector[1].y = 0.0f; m_Collider_Normal_Vector[1].z = 0.0f;

		m_Cube_Collider[2] = new OBB();
		temp_Pos = m_Position;
		temp_Pos.z -= OBJECT_SIZE * 0.65f * m_Scale_Z;
		m_Cube_Collider[2]->OBB_Init(temp_Pos, m_Scale_X, m_Scale_Y, m_Scale_Z * 0.3f);
		m_Collider_Normal_Vector[2].x = 0.0f; m_Collider_Normal_Vector[2].y = 0.0f; m_Collider_Normal_Vector[2].z = 1.0f;

		m_Cube_Collider[3] = new OBB();
		temp_Pos = m_Position;
		temp_Pos.x -= OBJECT_SIZE * 0.65f * m_Scale_X;
		m_Cube_Collider[3]->OBB_Init(temp_Pos, m_Scale_X * 0.3f, m_Scale_Y, m_Scale_Z);
		m_Collider_Normal_Vector[3].x = 1.0f; m_Collider_Normal_Vector[3].y = 0.0f; m_Collider_Normal_Vector[3].z = 0.0f;

		m_Cube_Collider[4] = new OBB();
		temp_Pos = m_Position;
		temp_Pos.y += OBJECT_SIZE * 0.65f * m_Scale_Y;
		m_Cube_Collider[4]->OBB_Init(temp_Pos, m_Scale_X, m_Scale_Y * 0.3f, m_Scale_Z);
		m_Collider_Normal_Vector[4].x = 0.0f; m_Collider_Normal_Vector[4].y = -1.0f; m_Collider_Normal_Vector[4].z = 0.0f;

		m_Cube_Collider[5] = new OBB();
		temp_Pos = m_Position;
		temp_Pos.y -= OBJECT_SIZE * 0.65f * m_Scale_Y;
		m_Cube_Collider[5]->OBB_Init(temp_Pos, m_Scale_X, m_Scale_Y * 0.3f, m_Scale_Z);
		m_Collider_Normal_Vector[5].x = 0.0f; m_Collider_Normal_Vector[5].y = 1.0f; m_Collider_Normal_Vector[5].z = 0.0f;

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
		vertex.color.R = 0.6f;
		vertex.color.G = 0.6f;
		vertex.color.B = 0.6f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = -OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE * 0.5f;
		vertex.y = OBJECT_SIZE * 0.5f;
		vertex.z = OBJECT_SIZE * 0.5f;
		vertice.push_back(vertex);
		
		m_Model_Planes.push_back(vertice);
		vertice.clear();

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

		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glFrontFace(GL_CW);
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

		/* // 충돌체 그리기
		for (int i = 0; i < 6; ++i)
		{
			glPushMatrix();
			glTranslatef(m_Cube_Collider[i]->Get_Center_Position()[0], m_Cube_Collider[i]->Get_Center_Position()[1], m_Cube_Collider[i]->Get_Center_Position()[2]);
			glMultMatrixf(m_Rotate_Transform_Matrix);
			glScalef(m_Cube_Collider[i]->Get_OBB_Axis_Distance()[0] / (OBJECT_SIZE * 0.5f), m_Cube_Collider[i]->Get_OBB_Axis_Distance()[1] / (OBJECT_SIZE * 0.5f), m_Cube_Collider[i]->Get_OBB_Axis_Distance()[2] / (OBJECT_SIZE * 0.5f));
			glutWireCube(OBJECT_SIZE);
			glPopMatrix();
		}
		*/
	}

	void Cube_Roll(const int& relative_mouse_x)
	{
		Roll(relative_mouse_x * 0.2f);

		for (int i = 0; i < 6; ++i)
		{
			glPushMatrix();
			glLoadIdentity();
			glRotatef(relative_mouse_x * 0.2f, 0.0f, 0.0f, -1.0f);
			glGetFloatv(GL_MODELVIEW_MATRIX, m_Transform);
			m_Cube_Collider[i]->Multiply_Rotate_Matrix_to_OBB_Axis(m_Transform);
			m_Cube_Collider[i]->Multiply_Rotate_Matrix_to_OBB_Position(m_Transform);
			Multiply_Rotate_Matrix_to_Normal_Vector(i, m_Transform);
			glPopMatrix();
		}
	}

	void Multiply_Rotate_Matrix_to_Normal_Vector(const int& index, float* rot_Transform)
	{
		float temp_Float = 0.0f;
		float temp_Vector[4];
		float Vector[4];

		Vector[0] = m_Collider_Normal_Vector[index].x;
		Vector[1] = m_Collider_Normal_Vector[index].y;
		Vector[2] = m_Collider_Normal_Vector[index].z;
		Vector[3] = m_Collider_Normal_Vector[index].w;

		for (int i = 0; i < 4; ++i)
		{
			temp_Float = 0.0f;
			for (int j = 0; j < 4; ++j) 
				temp_Float += rot_Transform[i + j * 4] * Vector[j];
			temp_Vector[i] = temp_Float;
		}

		m_Collider_Normal_Vector[index].x = temp_Vector[0];
		m_Collider_Normal_Vector[index].y = temp_Vector[1];
		m_Collider_Normal_Vector[index].z = temp_Vector[2];
		m_Collider_Normal_Vector[index].w = temp_Vector[3];
	}
};

