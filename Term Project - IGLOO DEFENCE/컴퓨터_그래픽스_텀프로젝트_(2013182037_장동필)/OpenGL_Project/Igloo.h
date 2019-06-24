#pragma once
#include "Object.h"

class Igloo : Object
{
private:
	bool m_is_Hurt = false;
	float m_Curr_Shake_time = 0.0f;
	float m_Shake_time = 0.5f;
	bool m_Shake_Sign = true;
	Vector3 m_Origin_Pos;
	int m_Max_HP;

public:
	void Igloo_OBB_Size_Rebuild() { Vector3 temp_v = m_Position; temp_v.y += OBJECT_SIZE * m_Scale_Y * 0.5f; m_OBB->OBB_Init(temp_v, m_Scale_X * 0.9f, m_Scale_Y * 0.5f, m_Scale_Z * 0.9f); }
	virtual void HP_Reduce() 
	{
		m_HP -= 1;
		m_is_Hurt = true;
		m_Curr_Shake_time = 0.0f;
	}
	virtual int Get_HP() { return m_HP; }
	float const Get_Max_HP() { return m_Max_HP; }

	void Shake()
	{
		if (m_is_Hurt)
		{
			if (m_Curr_Shake_time <= m_Shake_time)
			{
				m_Curr_Shake_time += 0.01;
				if (m_Shake_Sign)
				{
					m_Position.x = m_Origin_Pos.x + 3.0f;
					m_Shake_Sign = false;
				}
				else
				{
					m_Position.x = m_Origin_Pos.x - 3.0f;
					m_Shake_Sign = true;
				}
			}
			else
			{
				m_is_Hurt = false;
				m_Curr_Shake_time = 0.0f;
				m_Shake_Sign = true;
				m_Position = m_Origin_Pos;
			}
		}
	}

public:
	Igloo() : Object() { Igloo_Init(); }
	~Igloo() {}

	void Igloo_Init()
	{
		m_Scale_X = 5.0f; m_Scale_Y = 5.0f; m_Scale_Z = 5.0f;
		m_Position.y = -OBJECT_SIZE * m_Scale_Y * 0.2f;
		
		m_Origin_Pos = m_Position;

		m_HP = 30;
		m_Max_HP = m_HP;

		m_Material.Ambient[0] = 0.6f; m_Material.Ambient[1] = 0.6f; m_Material.Ambient[2] = 0.6f; m_Material.Ambient[3] = 1.0f;
		m_Material.Diffuse[0] = 0.8f; m_Material.Diffuse[1] = 0.6f; m_Material.Diffuse[2] = 0.6f; m_Material.Diffuse[3] = 1.0f;
		m_Material.Specular[0] = 1.0f; m_Material.Specular[1] = 1.0f; m_Material.Specular[2] = 1.0f; m_Material.Specular[3] = 1.0f;

		Set_Texture("Resources/Texture/Obstacle/Igloo_Texture.bmp", 1024, 512);
	}

	virtual void Draw()
	{
		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
		glMateriali(GL_FRONT, GL_SHININESS, 20);

		m_Texture->Mapping_Texture(true);
		glutSolidSphere(OBJECT_SIZE, 10, 10);
		m_Texture->Mapping_Off(true);

		glPushMatrix();
		{
			glTranslatef(0.0f, 0.0f, OBJECT_SIZE);
			glScalef(0.5f, 0.5f, 2.0f);
			m_Texture->Mapping_Texture(true);
			glutSolidTorus(OBJECT_SIZE * 0.1f, OBJECT_SIZE * 0.5f, 12, 12);
			m_Texture->Mapping_Off(true);

			glDisable(GL_LIGHTING);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glPushMatrix();
			{
				glBegin(GL_POLYGON);
				glVertex2f(-OBJECT_SIZE * 0.35f, OBJECT_SIZE * 0.4f);
				glVertex2f(-OBJECT_SIZE * 0.35f, -OBJECT_SIZE * 0.4f);
				glVertex2f(OBJECT_SIZE * 0.35f, -OBJECT_SIZE * 0.4f);
				glVertex2f(OBJECT_SIZE * 0.35f, OBJECT_SIZE * 0.4f);
				glEnd();
			}
			glPopMatrix();

			glPushMatrix();
			{
				glTranslatef(0.0f, 0.0f, OBJECT_SIZE * 0.05f);
				glRotatef(-15.0f, 1.0f, 0.0f, 0.0f);
				glScalef(0.45f, 0.5f, 0.5f);
				glBegin(GL_POLYGON);
				glVertex2f(-OBJECT_SIZE, OBJECT_SIZE);
				glVertex2f(-OBJECT_SIZE, -OBJECT_SIZE);
				glVertex2f(OBJECT_SIZE, -OBJECT_SIZE);
				glVertex2f(OBJECT_SIZE, OBJECT_SIZE);
				glEnd();
			}
			glEnable(GL_LIGHTING);
			glPopMatrix();
		}
		glPopMatrix();

		glPopMatrix();
	}

	virtual void Update()
	{
		Shake();
	}
};