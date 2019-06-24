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

		// ���׸ӽ� �Ӽ�
		m_Position.x = OBJECT_SIZE * 0.5f * -16.0f;
		m_Position.y = 2.0f;
		m_Position.z = OBJECT_SIZE * 0.5f * 16.0f;

		Collider_Update();

		m_Rotate_Yaw_Value = 45.0f;
		
		m_Color.R = 0.3f; m_Color.G = 0.3f; m_Color.B = 1.0f; m_Color.A = 1.0f;

		// �ٴ� ��� �Ӽ�
		m_Running_Man = new Object(false);
		m_Running_Man->Set_Translate(0.0f, OBJECT_SIZE, 0.0f);
		m_Running_Man->Set_Color(0.6f, 0.3f, 1.0f, 1.0f);
	}

	virtual void Draw()
	{
		// ���׸ӽ� �׸���

		glPushMatrix(); // ���׸ӽ� Translate & Rotate Push

		// ��ȯ ����.
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);

		glPushMatrix(); // ���׸ӽ� �ٴ� �׸��� push
		glScalef(m_Scale_X * 1.2f, m_Scale_Y * 0.1f, m_Scale_Z * 2.0f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ���׸ӽ� �ٴ� �׸��� pop
		 
		glPushMatrix(); // ���׸ӽ� �ٴں�Ʈ �׸��� push
		glScalef(m_Scale_X * 1.2f, m_Scale_Y * 0.1f, m_Scale_Z);
		glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
		glRotatef(m_Belt_Rotate_Roll, 0.0f, 0.0f, 1.0f);
		glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
		glutWireTorus(OBJECT_SIZE * 0.5f * m_Scale_Z * 0.95f, OBJECT_SIZE * 0.5f * m_Scale_Z, 30, 30);
		glPopMatrix(); // ���׸ӽ� �ٴں�Ʈ �׸��� pop

		glPushMatrix(); // ���׸ӽ� ����1 �׸��� push
		glTranslatef(OBJECT_SIZE * 0.5f * m_Scale_X * 1.1f, OBJECT_SIZE * 0.5f * m_Scale_Y * 1.5f, OBJECT_SIZE * 0.5f * m_Scale_Z * 2.0f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 1.5f, m_Scale_Z * 0.1f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ���׸ӽ� ����1 �׸��� pop

		glPushMatrix(); // ���׸ӽ� ����2 �׸��� push
		glTranslatef(-OBJECT_SIZE * 0.5f * m_Scale_X * 1.1f, OBJECT_SIZE * 0.5f * m_Scale_Y * 1.5f, OBJECT_SIZE * 0.5f * m_Scale_Z * 2.0f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 1.5f, m_Scale_Z * 0.1f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ���׸ӽ� ����2 �׸��� pop

		glPushMatrix(); // ���׸ӽ� ��Ȳ�� �׸��� push
		glTranslatef(0.0f, OBJECT_SIZE * 0.5f * m_Scale_Y * 3.0f, OBJECT_SIZE * 0.5f * m_Scale_Y * 2.0f);
		glRotatef(135.0f, 1.0f, 0.0f, 0.0f);
		glScalef(m_Scale_X * 1.2f, m_Scale_Y * 0.1f, m_Scale_Z * 0.5f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ���׸ӽ� ��Ȳ�� �׸��� pop
		
		// ���׸� �׸���
		if (m_Running_Man)
		{
			glPushMatrix(); // ���׸� ���� �׸��� push
			glTranslatef(m_Running_Man->Get_Position_X(), m_Running_Man->Get_Position_Y(), m_Running_Man->Get_Position_Z());
			glPushMatrix();
			glScalef(m_Running_Man->Get_Scale_X() * 0.5f, m_Running_Man->Get_Scale_Y(), m_Running_Man->Get_Scale_Z() * 0.3f);
			glColor4f(m_Running_Man->Get_Color_R(), m_Running_Man->Get_Color_G(), m_Running_Man->Get_Color_B(), m_Running_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();

			glPushMatrix(); // ���׸� �Ӹ� �׸��� push
			glTranslatef(0.0f, OBJECT_SIZE * m_Running_Man->Get_Scale_Y() * 0.7f, 0.0f);
			glPushMatrix();
			glScalef(m_Running_Man->Get_Scale_X() * 0.3f, m_Running_Man->Get_Scale_Y() * 0.3f, m_Running_Man->Get_Scale_Z() * 0.3f);
			glColor4f(m_Running_Man->Get_Color_R(), m_Running_Man->Get_Color_G(), m_Running_Man->Get_Color_B(), m_Running_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();
			glPopMatrix(); // ���׸� �Ӹ� �׸��� pop

			glPushMatrix(); // ���׸� ��1 �׸��� push
			glRotatef(30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, OBJECT_SIZE * m_Running_Man->Get_Scale_Y() * 0.5f, 0.0f);
			glPushMatrix();
			glScalef(m_Running_Man->Get_Scale_X() * 0.1f, m_Running_Man->Get_Scale_Y() * 0.7f, m_Running_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Running_Man->Get_Color_R(), m_Running_Man->Get_Color_G(), m_Running_Man->Get_Color_B(), m_Running_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();
			glPopMatrix(); // ���׸� ��1 �׸��� pop

			glPushMatrix(); // ���׸� ��2 �׸��� push
			glRotatef(-30.0f, 0.0f, 0.0f, 1.0f);
			glTranslatef(0.0f, OBJECT_SIZE * m_Running_Man->Get_Scale_Y() * 0.5f, 0.0f);
			glPushMatrix();
			glScalef(m_Running_Man->Get_Scale_X() * 0.1f, m_Running_Man->Get_Scale_Y() * 0.7f, m_Running_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Running_Man->Get_Color_R(), m_Running_Man->Get_Color_G(), m_Running_Man->Get_Color_B(), m_Running_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();
			glPopMatrix(); // ���׸� ��2 �׸��� pop

			glPushMatrix(); // ���׸� �ٸ�1 �׸��� push
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
			glPopMatrix(); // ���׸� �ٸ�1 �׸��� pop

			glPushMatrix(); // ���׸� �ٸ�2 �׸��� push
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
			glPopMatrix(); // ���׸� �ٸ�2 �׸��� pop

			glPopMatrix(); // ���׸� ���� �׸��� pop
		}

		glPopMatrix(); // ���׸ӽ� Translate & Rotate Pop
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

