#pragma once
#include "Object.h"
#include "Pyramid.h"

class Obstacle : Object
{
private:
	Pyramid* m_Tree_Leaves = nullptr;

public:

public:
	Obstacle(const int& type, const float& x, const float& y, const float& z) : Object() { Obstacle_Init(type, x, y, z); }
	~Obstacle() 
	{
		if (m_Tree_Leaves != nullptr)
		{
			delete m_Tree_Leaves;
			m_Tree_Leaves = nullptr;
		}
	}

	void Obstacle_Init(const int& type, const float& x, const float& y, const float& z)
	{
		m_Object_Type = type;

		m_Position.x = x; m_Position.y = y; m_Position.z = z;

		switch (m_Object_Type)
		{
		case ROCK_OBSTACLE:
			m_Scale_X = 0.8f; m_Scale_Y = 0.8f; m_Scale_Z = 0.8f;
			m_Position.y = -OBJECT_SIZE + OBJECT_SIZE * m_Scale_Y * 0.5f;
			break;

		case TREE_OBSTACLE:
			m_Scale_X = 0.5f; m_Scale_Y = 1.5f; m_Scale_Z = 0.5f;
			m_Position.y = -OBJECT_SIZE + OBJECT_SIZE * m_Scale_Y * 0.5f;

			m_Tree_Leaves = new Pyramid();
			reinterpret_cast<Object*>(m_Tree_Leaves)->Set_Material_Ambient_Color(0.2f, 0.2f, 0.2f, 1.0f);
			reinterpret_cast<Object*>(m_Tree_Leaves)->Set_Material_Diffuse_Color(0.2f, 0.8f, 0.2f, 1.0f);
			reinterpret_cast<Object*>(m_Tree_Leaves)->Set_Material_Specular_Color(0.7f, 0.7f, 0.7f, 1.0f);
			reinterpret_cast<Object*>(m_Tree_Leaves)->Set_Translate(m_Position.x, m_Position.y + OBJECT_SIZE * m_Scale_Y, m_Position.z);


			break;
		}

		Make_OBB();
	}

	virtual void Draw()
	{
		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		switch (m_Object_Type)
		{
		case ROCK_OBSTACLE:
			glDisable(GL_LIGHTING);
			glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
			glutSolidSphere(OBJECT_SIZE, 5, 5);
			glEnable(GL_LIGHTING);
			break;

		case TREE_OBSTACLE:
			glDisable(GL_LIGHTING);
			glColor4f(0.5f, 0.3f, 0.2f, 1.0f);
			glutSolidCube(OBJECT_SIZE);
			glEnable(GL_LIGHTING);
			glPopMatrix();
			glPushMatrix();
			m_Tree_Leaves->Draw();
			break;
		}

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