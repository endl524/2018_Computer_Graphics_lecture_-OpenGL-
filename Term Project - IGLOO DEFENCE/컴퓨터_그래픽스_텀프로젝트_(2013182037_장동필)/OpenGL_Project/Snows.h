#pragma once
#include "Object.h"

class Snows : Object
{
private:

public:

public:
	Snows() : Object() { Snows_Init(); }
	~Snows() {}

	void Snows_Init()
	{
		m_Scale_X = 1.0f; m_Scale_Y = 0.3f; m_Scale_Z = 1.0f;
		m_Position.y = -OBJECT_SIZE + OBJECT_SIZE * m_Scale_Y * 0.5f;

		m_Material.Ambient[0] = 1.0f; m_Material.Ambient[1] = 1.0f; m_Material.Ambient[2] = 1.0f; m_Material.Ambient[3] = 1.0f;
		m_Material.Diffuse[0] = 1.0f; m_Material.Diffuse[1] = 1.0f; m_Material.Diffuse[2] = 1.0f; m_Material.Diffuse[3] = 1.0f;
		m_Material.Specular[0] = 1.0f; m_Material.Specular[1] = 1.0f; m_Material.Specular[2] = 1.0f; m_Material.Specular[3] = 1.0f;

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
		glMateriali(GL_FRONT, GL_SHININESS, 30);

		glutSolidSphere(OBJECT_SIZE, 5, 5);
		glPopMatrix();
	}
};