#pragma once
#include "Object.h"

class Floor : Object
{
private:

public:
	Floor() : Object(true) { Floor_Initialize(); };
	~Floor() {};

	void Floor_Initialize()
	{
		m_Object_Type = FLOOR_OBJECT;
		Set_is_Static(true);

		Set_Color(1.0f, 1.0f, 0.5f, 1.0f);
		Set_Translate(0.0f, 0.0f, 0.0f);
		Set_Scale(20.0f, 0.02f, 20.0f);
		Set_Object_Type(FLOOR_OBJECT);
	}

	virtual void Draw()
	{
		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

		switch (m_Object_Type)
		{
		case FLOOR_OBJECT:
			glutSolidCube(OBJECT_SIZE);
			glPopMatrix();

			glScalef(1.0f, 1.0f, 1.0f);

			// NW ¹Ù´Ú
			glPushMatrix();
			glTranslatef(-OBJECT_SIZE * 0.3f * m_Scale_X, 0.5f, -OBJECT_SIZE * 0.3f * m_Scale_Z);
			glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
			glBegin(GL_TRIANGLES);
			glVertex3f(-FLOOR_TRIANGLE_SIZE, 0.0f, -FLOOR_TRIANGLE_SIZE);
			glVertex3f(-FLOOR_TRIANGLE_SIZE, 0.0f, FLOOR_TRIANGLE_SIZE);
			glVertex3f(FLOOR_TRIANGLE_SIZE, 0.0f, -FLOOR_TRIANGLE_SIZE);
			glEnd();
			glPopMatrix();

			// NE ¹Ù´Ú
			glPushMatrix();
			glTranslatef(OBJECT_SIZE * 0.3f * m_Scale_X, 0.5f, -OBJECT_SIZE * 0.3f * m_Scale_Z);
			glColor4f(0.5f, 0.2f, 0.2f, 1.0f);
			glBegin(GL_TRIANGLES);
			glVertex3f(-FLOOR_TRIANGLE_SIZE, 0.0f, -FLOOR_TRIANGLE_SIZE);
			glVertex3f(FLOOR_TRIANGLE_SIZE, 0.0f, FLOOR_TRIANGLE_SIZE);
			glVertex3f(FLOOR_TRIANGLE_SIZE, 0.0f, -FLOOR_TRIANGLE_SIZE);
			glEnd();
			glPopMatrix();

			// SW ¹Ù´Ú
			glPushMatrix();
			glTranslatef(-OBJECT_SIZE * 0.3f * m_Scale_X, 0.5f, OBJECT_SIZE * 0.3f * m_Scale_Z);
			glColor4f(0.2f, 0.2f, 0.5f, 1.0f);
			glBegin(GL_TRIANGLES);
			glVertex3f(-FLOOR_TRIANGLE_SIZE, 0.0f, -FLOOR_TRIANGLE_SIZE);
			glVertex3f(-FLOOR_TRIANGLE_SIZE, 0.0f, FLOOR_TRIANGLE_SIZE);
			glVertex3f(FLOOR_TRIANGLE_SIZE, 0.0f, FLOOR_TRIANGLE_SIZE);
			glEnd();
			glPopMatrix();

			// SE ¹Ù´Ú
			glPushMatrix();
			glTranslatef(OBJECT_SIZE * 0.3f * m_Scale_X, 0.5f, OBJECT_SIZE * 0.3f * m_Scale_Z);
			glColor4f(0.2f, 0.5f, 0.2f, 1.0f);
			glBegin(GL_TRIANGLES);
			glVertex3f(FLOOR_TRIANGLE_SIZE, 0.0f, -FLOOR_TRIANGLE_SIZE);
			glVertex3f(-FLOOR_TRIANGLE_SIZE, 0.0f, FLOOR_TRIANGLE_SIZE);
			glVertex3f(FLOOR_TRIANGLE_SIZE, 0.0f, FLOOR_TRIANGLE_SIZE);
			glEnd();
			glPopMatrix();

			break;
		}

		glPopMatrix();
		
	}
};

