#pragma once
#include "Object.h"

class Ball : Object
{
private:

public:
	Ball(const Vector3& pos) { Ball_Initialize(pos); };
	~Ball() {};

	void Set_Position_Y(const float& value) { m_Position.y = value; }
	float const Get_Position_Y() { return m_Position.y; }
	void Set_Position_Z(const float& value) { m_Position.z = value; }
	float const Get_Position_Z() { return m_Position.z; }

	void Ball_Initialize(const Vector3& pos)
	{
		m_Object_Type = BALL_OBJECT;

		m_Position.x = pos.x;
		m_Position.y = pos.y;
		m_Position.z = pos.z;

		m_Color.R = 1.0f; m_Color.G = 1.0f; m_Color.B = 1.0f; m_Color.A = 1.0f;
	}

	virtual void Draw()
	{
		glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);

		glPushMatrix();
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidSphere(OBJECT_SIZE * 0.8f, 15, 15);
		glPopMatrix();

		glPopMatrix();
	}

	void Ball_Pitch(const float& value)
	{
		Pitch(value);
	}
};

