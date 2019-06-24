#pragma once
#include "Object.h"

class Running_Machine : Object
{
private:
	Object* m_Running_Man = nullptr;
	float m_Left_Leg_Rotate = 30.0f;
	float m_Right_Leg_Rotate = -30.0f;
	float m_Leg_Rotate_Sign = 1;
	float m_Belt_Rotate_Roll = 0.0f;

public:
	Running_Machine() { Running_Machine_Initialize(); };
	~Running_Machine()
	{
		if (m_Running_Man != nullptr)
		{
			delete m_Running_Man;
			m_Running_Man = nullptr;
		}
	};

	void Running_Machine_Initialize()
	{
		m_Object_Type = RUNNING_MACHINE_OBJECT;
		m_is_Static_Object = true;

		// ·±´×¸Ó½Å ¼Ó¼º
		m_Position.x = OBJECT_SIZE * 0.5f * -16.0f;
		m_Position.y = 2.0f;
		m_Position.z = OBJECT_SIZE * 0.5f * 16.0f;

		Collider_Update();

		m_Rotate_Yaw_Value = 45.0f;
		
		m_Color.R = 0.3f; m_Color.G = 0.3f; m_Color.B = 1.0f; m_Color.A = 1.0f;

		// ¶Ù´Â »ç¶÷ ¼Ó¼º
		m_Running_Man = new Object(false);
		m_Running_Man->Set_Translate(0.0f, OBJECT_SIZE, 0.0f);
		m_Running_Man->Set_Color(0.6f, 0.3f, 1.0f, 1.0f);
	}

	virtual void Draw()
	{
		// ·±´×¸Ó½Å ±×¸®±â

		glPushMatrix(); // ·±´×¸Ó½Å Translate & Rotate Push

		// º¯È¯ Àû¿ë.
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);

		glPushMatrix(); // ·±´×¸Ó½Å ¹Ù´Ú ±×¸®±â push
		glScalef(m_Scale_X * 1.2f, m_Scale_Y * 0.1f, m_Scale_Z * 2.0f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ·±´×¸Ó½Å ¹Ù´Ú ±×¸®±â pop
		 
		glPushMatrix(); // ·±´×¸Ó½Å ¹Ù´Úº§Æ® ±×¸®±â push
		glScalef(m_Scale_X * 1.2f, m_Scale_Y * 0.1f, m_Scale_Z);
		glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(m_Belt_Rotate_Roll, 0.0f, 0.0f, 1.0f);
		glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
		glutWireTorus(OBJECT_SIZE * 0.5f * m_Scale_Z * 0.95f, OBJECT_SIZE * 0.5f * m_Scale_Z, 30, 30);
		glPopMatrix(); // ·±´×¸Ó½Å ¹Ù´Úº§Æ® ±×¸®±â pop

		glPushMatrix(); // ·±´×¸Ó½Å »À´ë1 ±×¸®±â push
		glTranslatef(OBJECT_SIZE * 0.5f * m_Scale_X * 1.1f, OBJECT_SIZE * 0.5f * m_Scale_Y * 1.5f, OBJECT_SIZE * 0.5f * m_Scale_Z * 2.0f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 1.5f, m_Scale_Z * 0.1f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ·±´×¸Ó½Å »À´ë1 ±×¸®±â pop

		glPushMatrix(); // ·±´×¸Ó½Å »À´ë2 ±×¸®±â push
		glTranslatef(-OBJECT_SIZE * 0.5f * m_Scale_X * 1.1f, OBJECT_SIZE * 0.5f * m_Scale_Y * 1.5f, OBJECT_SIZE * 0.5f * m_Scale_Z * 2.0f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 1.5f, m_Scale_Z * 0.1f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ·±´×¸Ó½Å »À´ë2 ±×¸®±â pop

		glPushMatrix(); // ·±´×¸Ó½Å »óÈ²ÆÇ ±×¸®±â push
		glTranslatef(0.0f, OBJECT_SIZE * 0.5f * m_Scale_Y * 3.0f, OBJECT_SIZE * 0.5f * m_Scale_Y * 2.0f);
		glRotatef(135.0f, 1.0f, 0.0f, 0.0f);
		glScalef(m_Scale_X * 1.2f, m_Scale_Y * 0.1f, m_Scale_Z * 0.5f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ·±´×¸Ó½Å »óÈ²ÆÇ ±×¸®±â pop
		
		// ·±´×¸Ç ±×¸®±â
		if (m_Running_Man)
		{
			glPushMatrix(); // ·±´×¸Ç ¸öÅë ±×¸®±â push
			glTranslatef(m_Running_Man->Get_Position_X(), m_Running_Man->Get_Position_Y(), m_Running_Man->Get_Position_Z());
			glPushMatrix();
			glScalef(m_Running_Man->Get_Scale_X() * 0.5f, m_Running_Man->Get_Scale_Y(), m_Running_Man->Get_Scale_Z() * 0.3f);
			glColor4f(m_Running_Man->Get_Color_R(), m_Running_Man->Get_Color_G(), m_Running_Man->Get_Color_B(), m_Running_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();

			glPushMatrix(); // ·±´×¸Ç ¸Ó¸® ±×¸®±â push
			glTranslatef(0.0f, OBJECT_SIZE * m_Running_Man->Get_Scale_Y() * 0.7f, 0.0f);
			glPushMatrix();
			glScalef(m_Running_Man->Get_Scale_X() * 0.3f, m_Running_Man->Get_Scale_Y() * 0.3f, m_Running_Man->Get_Scale_Z() * 0.3f);
			glColor4f(m_Running_Man->Get_Color_R(), m_Running_Man->Get_Color_G(), m_Running_Man->Get_Color_B(), m_Running_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();
			glPopMatrix(); // ·±´×¸Ç ¸Ó¸® ±×¸®±â pop

			glPushMatrix(); // ·±´×¸Ç ÆÈ1 ±×¸®±â push
			glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, OBJECT_SIZE * m_Running_Man->Get_Scale_Y() * 0.5f, 0.0f);
			glPushMatrix();
			glScalef(m_Running_Man->Get_Scale_X() * 0.1f, m_Running_Man->Get_Scale_Y() * 0.7f, m_Running_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Running_Man->Get_Color_R(), m_Running_Man->Get_Color_G(), m_Running_Man->Get_Color_B(), m_Running_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();
			glPopMatrix(); // ·±´×¸Ç ÆÈ1 ±×¸®±â pop

			glPushMatrix(); // ·±´×¸Ç ÆÈ2 ±×¸®±â push
			glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, OBJECT_SIZE * m_Running_Man->Get_Scale_Y() * 0.5f, 0.0f);
			glPushMatrix();
			glScalef(m_Running_Man->Get_Scale_X() * 0.1f, m_Running_Man->Get_Scale_Y() * 0.7f, m_Running_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Running_Man->Get_Color_R(), m_Running_Man->Get_Color_G(), m_Running_Man->Get_Color_B(), m_Running_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();
			glPopMatrix(); // ·±´×¸Ç ÆÈ2 ±×¸®±â pop

			glPushMatrix(); // ·±´×¸Ç ´Ù¸®1 ±×¸®±â push
			glTranslatef(-OBJECT_SIZE * m_Running_Man->Get_Scale_X() * 0.1f, -OBJECT_SIZE * m_Running_Man->Get_Scale_X() * 0.3f, 0.0f);
			glRotatef(m_Left_Leg_Rotate, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -OBJECT_SIZE * m_Running_Man->Get_Scale_Y() * 0.5f, 0.0f);
			glPushMatrix();
			glScalef(m_Running_Man->Get_Scale_X() * 0.15f, m_Running_Man->Get_Scale_Y() * 0.5f, m_Running_Man->Get_Scale_Z() * 0.15f);
			glColor4f(m_Running_Man->Get_Color_R(), m_Running_Man->Get_Color_G(), m_Running_Man->Get_Color_B(), m_Running_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();
			glPopMatrix(); // ·±´×¸Ç ´Ù¸®1 ±×¸®±â pop

			glPushMatrix(); // ·±´×¸Ç ´Ù¸®2 ±×¸®±â push
			glTranslatef(OBJECT_SIZE * m_Running_Man->Get_Scale_X() * 0.1f, -OBJECT_SIZE * m_Running_Man->Get_Scale_X() * 0.3f, 0.0f);
			glRotatef(m_Right_Leg_Rotate, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -OBJECT_SIZE * m_Running_Man->Get_Scale_Y() * 0.5f, 0.0f);
			glPushMatrix();
			glScalef(m_Running_Man->Get_Scale_X() * 0.15f, m_Running_Man->Get_Scale_Y() * 0.5f, m_Running_Man->Get_Scale_Z() * 0.15f);
			glColor4f(m_Running_Man->Get_Color_R(), m_Running_Man->Get_Color_G(), m_Running_Man->Get_Color_B(), m_Running_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();
			glPopMatrix(); // ·±´×¸Ç ´Ù¸®2 ±×¸®±â pop

			glPopMatrix(); // ·±´×¸Ç ¸öÅë ±×¸®±â pop
		}

		glPopMatrix(); // ·±´×¸Ó½Å Translate & Rotate Pop
	}

	virtual void Update()
	{
		m_Belt_Rotate_Roll += RUNNING_MACHINE_BELT_ROTATE_VALUE;
		if (m_Belt_Rotate_Roll >= 360.0f) m_Belt_Rotate_Roll = m_Belt_Rotate_Roll - 360.0f;

		m_Left_Leg_Rotate += RUNNING_MACHINE_MAN_ROTATE_VALUE * m_Leg_Rotate_Sign;
		m_Right_Leg_Rotate -= RUNNING_MACHINE_MAN_ROTATE_VALUE * m_Leg_Rotate_Sign;

		m_Leg_Rotate_Sign *= m_Left_Leg_Rotate >= 40.0f || m_Left_Leg_Rotate <= -40.0f ? -1 : 1;
	}
};

