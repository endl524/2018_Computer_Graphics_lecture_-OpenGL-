#pragma once
#include "Object.h"

class Rock : Object
{
private:

public:
	void Rock_OBB_Size_Rebuild() { m_OBB->OBB_Init(m_Position, m_Scale_X * 0.8f, m_Scale_Y * 0.8f, m_Scale_Z * 0.8f); }


public:
	Rock() : Object() { Rock_Init(); }
	~Rock() {}

	void Rock_Init()
	{
		m_Scale_X = 0.8f; m_Scale_Y = 0.8f; m_Scale_Z = 0.8f;
		m_Position.y = -OBJECT_SIZE + OBJECT_SIZE * m_Scale_Y * 0.5f;
		
		m_Material.Ambient[0] = 0.5f; m_Material.Ambient[1] = 0.5f; m_Material.Ambient[2] = 0.5f; m_Material.Ambient[3] = 1.0f;
		m_Material.Diffuse[0] = 0.5f; m_Material.Diffuse[1] = 0.5f; m_Material.Diffuse[2] = 0.5f; m_Material.Diffuse[3] = 1.0f;
		m_Material.Specular[0] = 0.6f; m_Material.Specular[1] = 0.6f; m_Material.Specular[2] = 0.6f; m_Material.Specular[3] = 1.0f;

		Set_Texture("Resources/Texture/Obstacle/Snow_Rock.bmp", 1024, 1024);
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
		glMateriali(GL_FRONT, GL_SHININESS, 60);

		m_Texture->Mapping_Texture(true);
		glutSolidSphere(OBJECT_SIZE, 5, 5);
		m_Texture->Mapping_Off(true);
		glPopMatrix();

		/* // OBB ±×¸®±â
		glPushMatrix();
		glTranslatef(m_OBB->Get_Center_Position()[0], m_OBB->Get_Center_Position()[1], m_OBB->Get_Center_Position()[2]);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_OBB->Get_OBB_Axis_Distance()[0] / (OBJECT_SIZE * 0.5f), m_OBB->Get_OBB_Axis_Distance()[1] / (OBJECT_SIZE * 0.5f), m_OBB->Get_OBB_Axis_Distance()[2] / (OBJECT_SIZE * 0.5f));
		glutWireCube(OBJECT_SIZE);
		glPopMatrix();
		*/
	}
};