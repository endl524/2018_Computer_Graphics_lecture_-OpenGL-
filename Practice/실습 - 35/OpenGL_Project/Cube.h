#pragma once
#include "Object.h"
#include "Spring.h"
#include "Ball.h"

class Cube : Object
{
private:
	vector<vector<Vertex3D>> m_Model_Planes;

	Spring* m_Spring = nullptr;
	Ball* m_Ball = nullptr;
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

	vector<Texture*> m_Textures;

public:
	Cube() : Object(true) { Cube_Initialize(); };
	~Cube()
	{
		if (m_Spring != nullptr)
		{
			delete m_Spring;
			m_Spring = nullptr;
		}

		if (m_Ball != nullptr)
		{
			delete m_Ball;
			m_Ball = nullptr;
		}

		if (!m_Textures.empty())
		{
			for (auto& texture : m_Textures)
			{
				delete texture; texture = nullptr;
			}
		}
	};

	void Cube_Initialize()
	{
		m_Object_Type = CUBE_OBJECT;
		m_is_Static_Object = true;

		Pitch(-30.0f);
		Yaw(45.0f);

		Make_Model_Vertice();

		m_Spring = new Spring(m_Position);
		m_Ball = new Ball(m_Position);

		Texture* temp_Texture;
		temp_Texture = new Texture();
		temp_Texture->Set_Texture("test_texture_0.bmp", 256, 256);
		m_Textures.push_back(temp_Texture);
		temp_Texture = new Texture();
		temp_Texture->Set_Texture("test_texture_1.bmp", 256, 256);
		m_Textures.push_back(temp_Texture);
		temp_Texture = new Texture();
		temp_Texture->Set_Texture("test_texture_2.bmp", 256, 256);
		m_Textures.push_back(temp_Texture);
		temp_Texture = new Texture();
		temp_Texture->Set_Texture("test_texture_3.bmp", 256, 256);
		m_Textures.push_back(temp_Texture);
		temp_Texture = new Texture();
		temp_Texture->Set_Texture("test_texture_5.bmp", 256, 256);
		m_Textures.push_back(temp_Texture);
		temp_Texture = new Texture();
		temp_Texture->Set_Texture("test_texture_4.bmp", 256, 256);
		m_Textures.push_back(temp_Texture);
	}

	virtual void Make_Model_Vertice()
	{
		Vertex3D vertex;
		vector<Vertex3D> vertice;

		// 1번 면
		vertex.x = -OBJECT_SIZE;
		vertex.y = OBJECT_SIZE * 2.0f;
		vertex.z = 0.0f;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE;
		vertex.y = 0.0f;
		vertex.z = 0.0f;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE;
		vertex.y = 0.0f;
		vertex.z = 0.0f;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE;
		vertex.y = OBJECT_SIZE * 2.0f;
		vertex.z = 0.0f;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		
		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 2번 면
		vertex.x = OBJECT_SIZE;
		vertex.y = OBJECT_SIZE;
		vertex.z = OBJECT_SIZE;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE;
		vertex.y = -OBJECT_SIZE;
		vertex.z = OBJECT_SIZE;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE;
		vertex.y = -OBJECT_SIZE;
		vertex.z = -OBJECT_SIZE;
		vertex.color.R = 0.3f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE;
		vertex.y = OBJECT_SIZE;
		vertex.z = -OBJECT_SIZE;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 3번 면
		vertex.x = OBJECT_SIZE;
		vertex.y = OBJECT_SIZE;
		vertex.z = -OBJECT_SIZE;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE;
		vertex.y = -OBJECT_SIZE;
		vertex.z = -OBJECT_SIZE;
		vertex.color.R = 0.3f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE;
		vertex.y = -OBJECT_SIZE;
		vertex.z = -OBJECT_SIZE;
		vertex.color.R = 0.1f;
		vertex.color.G = 0.1f;
		vertex.color.B = 0.1f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE;
		vertex.y = OBJECT_SIZE;
		vertex.z = -OBJECT_SIZE;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		// 4번 면
		vertex.x = -OBJECT_SIZE;
		vertex.y = OBJECT_SIZE;
		vertex.z = -OBJECT_SIZE;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE;
		vertex.y = -OBJECT_SIZE;
		vertex.z = -OBJECT_SIZE;
		vertex.color.R = 0.1f;
		vertex.color.G = 0.1f;
		vertex.color.B = 0.1f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE;
		vertex.y = -OBJECT_SIZE;
		vertex.z = OBJECT_SIZE;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE;
		vertex.y = OBJECT_SIZE;
		vertex.z = OBJECT_SIZE;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);

		m_Model_Planes.push_back(vertice);
		vertice.clear();

		///*
		// 5번 면
		vertex.x = -OBJECT_SIZE;
		vertex.y = 0.0f;
		vertex.z = 0.0f;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE;
		vertex.y = 0.0f;
		vertex.z = OBJECT_SIZE * 2.0f;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE;
		vertex.y = 0.0f;
		vertex.z = OBJECT_SIZE * 2.0f;
		vertex.color.R = 1.0f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE;
		vertex.y = 0.0f;
		vertex.z = 0.0f;
		vertex.color.R = 0.3f;
		vertex.color.G = 1.0f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		//*/
		m_Model_Planes.push_back(vertice);
		vertice.clear();
		
		// 6번 면
		vertex.x = -OBJECT_SIZE;
		vertex.y = -OBJECT_SIZE;
		vertex.z = OBJECT_SIZE;
		vertex.color.R = 1.0f;
		vertex.color.G = 0.3f;
		vertex.color.B = 0.3f;
		vertice.push_back(vertex);
		vertex.x = -OBJECT_SIZE;
		vertex.y = -OBJECT_SIZE;
		vertex.z = -OBJECT_SIZE;
		vertex.color.R = 0.1f;
		vertex.color.G = 0.1f;
		vertex.color.B = 0.1f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE;
		vertex.y = -OBJECT_SIZE;
		vertex.z = -OBJECT_SIZE;
		vertex.color.R = 0.3f;
		vertex.color.G = 0.3f;
		vertex.color.B = 1.0f;
		vertice.push_back(vertex);
		vertex.x = OBJECT_SIZE;
		vertex.y = -OBJECT_SIZE;
		vertex.z = OBJECT_SIZE;
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
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);

		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		m_Plane_Count = 0;

		for (auto& plane : m_Model_Planes)
		{
			if (m_Plane_Count == 0)
			{
				glPushMatrix();
				glTranslatef(0.0f, -OBJECT_SIZE, OBJECT_SIZE);
				glRotatef(m_F_Cover_Rotate_Value, 1.0f, 0.0f, 0.0f);
			}

			else if (m_Plane_Count == 4)
			{
				glPushMatrix();
				glTranslatef(0.0f, OBJECT_SIZE, -OBJECT_SIZE);
				glRotatef(m_U_Cover_Rotate_Value, 1.0f, 0.0f, 0.0f);
			}

			int vertex_count = 0;
			m_Textures[m_Plane_Count]->Mapping_Texture(false);
			glBegin(GL_QUADS);
			for (auto& vertex : plane)
			{
				glColor4f(vertex.color.R, vertex.color.G, vertex.color.B, vertex.color.A);
				if (vertex_count == 0) glTexCoord2f(1.0f, 1.0f);
				else if (vertex_count == 1) glTexCoord2f(1.0f, 0.0f);
				else if (vertex_count == 2) glTexCoord2f(0.0f, 0.0f);
				else if (vertex_count == 3) glTexCoord2f(0.0f, 1.0f);
				glVertex3f(vertex.x, vertex.y, vertex.z);

				++vertex_count;
			}
			glEnd();
			m_Textures[m_Plane_Count]->Mapping_Off(false);

			if (m_Plane_Count == 0 || m_Plane_Count == 4) glPopMatrix();
			++m_Plane_Count;
		}
		
		m_Spring->Draw();
		m_Ball->Draw();

		glPopMatrix();
	}

	void Upper_Cover_Switch(const bool& is_Open)
	{
		if (m_is_F_Cover_Closed)
		{
			if (is_Open) { m_is_U_Cover_Closed = false; m_is_U_Cover_Opening_End = false; m_is_U_Cover_Closing_End = true; }
			else { m_is_U_Cover_Opening_End = true; m_is_U_Cover_Closing_End = false; }
		}
	}

	void Front_Cover_Switch(const bool& is_Open)
	{
		if (m_is_U_Cover_Closed)
		{
			if (is_Open) { m_is_F_Cover_Closed = false; m_is_F_Cover_Opening_End = false; m_is_F_Cover_Closing_End = true; }
			else { m_is_F_Cover_Opening_End = true; m_is_F_Cover_Closing_End = false; }
		}
	}

	void Upper_Cover_Open()
	{
		if (!m_is_U_Cover_Opening_End)
		{
			if (m_Spring->Get_Scale_Y() >= 5.0f) { m_Spring->Set_Scale_Y(5.0f); m_is_U_Cover_Opening_End = true; }
			else if (m_Spring->Get_Scale_Y() >= 3.0f) m_Spring->Set_Scale_Y(m_Spring->Get_Scale_Y() + 0.5f);
			else m_Spring->Set_Scale_Y(m_Spring->Get_Scale_Y() + 0.2f);

			if (m_U_Cover_Rotate_Value <= -120.0f) m_U_Cover_Rotate_Value = -120.0f;
			else m_U_Cover_Rotate_Value -= 10.0f;

			m_Ball->Set_Position_Y(m_Spring->Get_Last_Vertex_Position_Y() * (m_Spring->Get_Scale_Y() - 1));
		}
	}

	void Upper_Cover_Close()
	{
		if (!m_is_U_Cover_Closing_End)
		{
			if (m_Spring->Get_Scale_Y() <= 1.0f) m_Spring->Set_Scale_Y(1.0f);
			else if (m_Spring->Get_Scale_Y() >= 3.0f) m_Spring->Set_Scale_Y(m_Spring->Get_Scale_Y() - 0.2f);
			else if (m_Spring->Get_Scale_Y() >= 2.0f) m_Spring->Set_Scale_Y(m_Spring->Get_Scale_Y() - 0.1f);
			else m_Spring->Set_Scale_Y(m_Spring->Get_Scale_Y() - 0.05f);

			if (m_U_Cover_Rotate_Value >= 0.0f) { m_U_Cover_Rotate_Value = 0.0f; m_is_U_Cover_Closing_End = true; m_is_U_Cover_Closed = true; }
			else m_U_Cover_Rotate_Value += 2.0f;

			m_Ball->Set_Position_Y(m_Spring->Get_Last_Vertex_Position_Y() * (m_Spring->Get_Scale_Y() - 1));
		}
	}

	void Front_Cover_Open()
	{
		if (!m_is_F_Cover_Opening_End)
		{
			if (m_Ball->Get_Position_Z() >= 160.0f) { m_Ball->Set_Position_Z(160.0f); m_is_F_Cover_Opening_End = true; }
			else if (m_Ball->Get_Position_Z() >= 40.0f)
			{
				m_Ball->Set_Position_Z(m_Ball->Get_Position_Z() + 5.0f);
				m_Ball->Ball_Pitch(-25.0f);
			}
			else
			{
				m_Ball->Set_Position_Z(m_Ball->Get_Position_Z() + 2.0f);
				m_Ball->Ball_Pitch(-10.0f);
			}

			if (m_F_Cover_Rotate_Value >= 90.0f) m_F_Cover_Rotate_Value = 90.0f;
			else m_F_Cover_Rotate_Value += 10.0f;
		}
	}

	void Front_Cover_Close()
	{
		if (!m_is_F_Cover_Closing_End)
		{
			if (m_Ball->Get_Position_Z() <= 0.0f) { m_Ball->Set_Position_Z(0.0f); m_is_F_Cover_Opening_End = true; }
			else if (m_Ball->Get_Position_Z() <= 40.0f)
			{
				m_Ball->Set_Position_Z(m_Ball->Get_Position_Z() - 2.0f);
				m_Ball->Ball_Pitch(10.0f);
			}
			else
			{
				m_Ball->Set_Position_Z(m_Ball->Get_Position_Z() - 5.0f);
				m_Ball->Ball_Pitch(25.0f);
			}

			if (m_F_Cover_Rotate_Value <= 0.0f) { m_F_Cover_Rotate_Value = 0.0f; m_is_F_Cover_Closing_End = true; m_is_F_Cover_Closed = true;	}
			else m_F_Cover_Rotate_Value -= 2.0f;
		}
	}

	virtual void Update()
	{
		Upper_Cover_Open();
		Upper_Cover_Close();
		Front_Cover_Open();
		Front_Cover_Close();
	}
};

