#pragma once
#include "Object.h"

class Bench_Press : Object
{
private:
	Object* m_Bench_Man = nullptr;
	float m_Upper_Arm_Rotate_Value = 45.0f;
	float m_Lower_Arm_Rotate_Value = 45.0f;
	int m_Arm_Rotate_Sign = 1;

public:
	Bench_Press() { Bench_Press_Initialize(); };
	~Bench_Press()
	{
		if (m_Bench_Man != nullptr)
		{
			delete m_Bench_Man;
			m_Bench_Man = nullptr;
		}
	};

	void Bench_Press_Initialize()
	{
		m_Object_Type = BAR_OBJECT;
		m_is_Static_Object = true;

		// ��ġ �Ӽ�
		m_Position.x = OBJECT_SIZE * 0.5f * -16.0f;
		m_Position.y = 0.0f;
		m_Position.z = OBJECT_SIZE * 0.5f * -16.0f;

		Collider_Update();

		m_Rotate_Yaw_Value = -45.0f;

		m_Color.R = 0.5f; m_Color.G = 0.5f; m_Color.B = 0.5f; m_Color.A = 1.0f;

		// ��� ��� �Ӽ�
		m_Bench_Man = new Object(false);
		m_Bench_Man->Set_Translate(0.0f, -OBJECT_SIZE * 1.2f, 0.0f);
		m_Bench_Man->Set_Color(1.0f, 1.0f, 1.0f, 1.0f);
	}

	virtual void Draw()
	{
		// ��ġ ������ �׸���

		glPushMatrix(); // ö�� Translate & Rotate Push

		// ��ȯ ����.
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);

		glPushMatrix(); // ��ġ ����� �׸���
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y, 0.0f);
		glScalef(m_Scale_X, m_Scale_Y * 0.1f, m_Scale_Z * 2.0f);
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ��ġ ����� Pop

		glPushMatrix(); // ��ġ ū ���1 �׸���
		glTranslatef(-OBJECT_SIZE * m_Scale_X * 0.5f, OBJECT_SIZE * m_Scale_Y, -OBJECT_SIZE * m_Scale_X * 0.8f);
		glPushMatrix();
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 2.0f, m_Scale_Z * 0.1f);
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix();

		glPushMatrix(); // ��ġ ū ���1 ��ġ��1 �׸���
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.8f, 0.0f);
		glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.3f, 0.0f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 0.4f, m_Scale_X * 0.1f);
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ��ġ ū ���1 ��ġ��1 Pop

		glPushMatrix(); // ��ġ ū ���1 ��ġ��2 �׸���
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.8f, 0.0f);
		glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.3f, 0.0f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 0.4f, m_Scale_X * 0.1f);
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ��ġ ū ���1 ��ġ��2 Pop

		glPopMatrix(); // ��ġ ū ���1 Pop

		

		glPushMatrix(); // ��ġ ū ���2 �׸���
		glTranslatef(OBJECT_SIZE * m_Scale_X * 0.5f, OBJECT_SIZE * m_Scale_Y, -OBJECT_SIZE * m_Scale_X * 0.8f);
		glPushMatrix();
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 2.0f, m_Scale_Z * 0.1f);
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix();

		glPushMatrix(); // ��ġ ū ���2 ��ġ��1 �׸���
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.8f, 0.0f);
		glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.3f, 0.0f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 0.4f, m_Scale_X * 0.1f);
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ��ġ ū ���2 ��ġ��1 Pop

		glPushMatrix(); // ��ġ ū ���2 ��ġ��2 �׸���
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.8f, 0.0f);
		glRotatef(-30.0f, 1.0f, 0.0f, 0.0f);
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.3f, 0.0f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 0.4f, m_Scale_X * 0.1f);
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ��ġ ū ���2 ��ġ��2 Pop

		glPopMatrix(); // ��ġ ū ���2 Pop


		glPushMatrix(); // ��ġ ���� ���1 �׸���
		glTranslatef(-OBJECT_SIZE * m_Scale_X * 0.5f, OBJECT_SIZE * 0.5f * m_Scale_Y, OBJECT_SIZE * m_Scale_Z * 0.9f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y, m_Scale_X * 0.1f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ��ġ ���� ���1 Pop


		glPushMatrix(); // ��ġ ���� ���2 �׸���
		glTranslatef(OBJECT_SIZE * m_Scale_X * 0.5f, OBJECT_SIZE * 0.5f * m_Scale_Y, OBJECT_SIZE * m_Scale_Z * 0.9f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y, m_Scale_X * 0.1f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ��ġ ���� ���2 Pop
		/*
		
		*/
		// ��� ��� �׸���
		if (m_Bench_Man)
		{
			glPushMatrix(); // ��� ��� Translate & Rotate Push
			glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 1.2f, -OBJECT_SIZE * m_Scale_Z * 1.2f);
			glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, m_Bench_Man->Get_Position_Y(), 0.0f);

			glPushMatrix(); // �Ӹ� Push
			glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.7f, 0.0f);
			glScalef(m_Bench_Man->Get_Scale_X() * 0.4f, m_Bench_Man->Get_Scale_Y() * 0.3f, m_Bench_Man->Get_Scale_Z() * 0.4f);
			glColor4f(m_Bench_Man->Get_Color_R(), m_Bench_Man->Get_Color_G(), m_Bench_Man->Get_Color_B(), m_Bench_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // �Ӹ� Pop

			glPushMatrix(); // ���� Push
			glScalef(m_Bench_Man->Get_Scale_X() * 0.5f, m_Bench_Man->Get_Scale_Y(), m_Bench_Man->Get_Scale_Z() * 0.3f);
			glColor4f(m_Bench_Man->Get_Color_R(), m_Bench_Man->Get_Color_G(), m_Bench_Man->Get_Color_B(), m_Bench_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // ���� Pop

			glPushMatrix(); // ���1 Push
			glTranslatef(-OBJECT_SIZE * 0.3f * m_Scale_X, OBJECT_SIZE * 0.3f * m_Scale_Y, 0.0f);
			glRotatef(-m_Upper_Arm_Rotate_Value, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0f, OBJECT_SIZE * 0.1f * m_Scale_Y, OBJECT_SIZE * 0.1f * m_Scale_Z);
			glPushMatrix();
			glScalef(m_Bench_Man->Get_Scale_X() * 0.1f, m_Bench_Man->Get_Scale_Y() * 0.1f, m_Bench_Man->Get_Scale_Z() * 0.4f);
			glColor4f(m_Bench_Man->Get_Color_R(), m_Bench_Man->Get_Color_G(), m_Bench_Man->Get_Color_B(), m_Bench_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();

			glPushMatrix(); // �Ͽ�1 Push
			glTranslatef(0.0f, 0.0f, OBJECT_SIZE * 0.2f);
			glRotatef(m_Upper_Arm_Rotate_Value, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, OBJECT_SIZE * 0.2f);

			glPushMatrix();
			glScalef(m_Bench_Man->Get_Scale_X() * 0.1f, m_Bench_Man->Get_Scale_Y() * 0.1f, m_Bench_Man->Get_Scale_Z() * 0.4f);
			glColor4f(m_Bench_Man->Get_Color_R(), m_Bench_Man->Get_Color_G(), m_Bench_Man->Get_Color_B(), m_Bench_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();

			glPushMatrix(); // Bar Push
			glTranslatef(OBJECT_SIZE * 0.5f * m_Scale_X, 0.0f, OBJECT_SIZE * 0.2f * m_Scale_Z);
			glPushMatrix();
			glScalef(m_Bench_Man->Get_Scale_X() * 3.0f, m_Bench_Man->Get_Scale_Y() * 0.1f, m_Bench_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();
			glPushMatrix(); // Bar Ring1 Push
			glTranslatef(-OBJECT_SIZE * m_Scale_X * 1.2f, 0.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glScalef(m_Bench_Man->Get_Scale_X() * 0.3f, m_Bench_Man->Get_Scale_Y() * 0.3f, m_Bench_Man->Get_Scale_Z() * 0.5f);
			glColor4f(1.0f, 0.3f, 0.7f, 1.0f);
			glutWireTorus(OBJECT_SIZE * 0.7f, OBJECT_SIZE, 20, 20);
			glPopMatrix(); // Bar Ring1 Pop
			glPushMatrix(); // Bar Ring2 Push
			glTranslatef(OBJECT_SIZE * m_Scale_X * 1.2f, 0.0f, 0.0f);
			glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
			glScalef(m_Bench_Man->Get_Scale_X() * 0.3f, m_Bench_Man->Get_Scale_Y() * 0.3f, m_Bench_Man->Get_Scale_Z() * 0.5f);
			glColor4f(1.0f, 0.3f, 0.7f, 1.0f);
			glutWireTorus(OBJECT_SIZE * 0.7f, OBJECT_SIZE, 20, 20);
			glPopMatrix(); // Bar Ring2 Pop
			glPopMatrix(); // Bar Pop
			glPopMatrix(); // �Ͽ�1 Pop
			glPopMatrix(); // ���1 Pop

			glPushMatrix(); // ���1 Push
			glTranslatef(OBJECT_SIZE * 0.3f * m_Scale_X, OBJECT_SIZE * 0.3f * m_Scale_Y, 0.0f);
			glRotatef(m_Upper_Arm_Rotate_Value, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0f, OBJECT_SIZE * 0.1f * m_Scale_Y, OBJECT_SIZE * 0.1f * m_Scale_Z);
			glPushMatrix();
			glScalef(m_Bench_Man->Get_Scale_X() * 0.1f, m_Bench_Man->Get_Scale_Y() * 0.1f, m_Bench_Man->Get_Scale_Z() * 0.4f);
			glColor4f(m_Bench_Man->Get_Color_R(), m_Bench_Man->Get_Color_G(), m_Bench_Man->Get_Color_B(), m_Bench_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();

			glPushMatrix(); // �Ͽ�1 Push
			glTranslatef(0.0f, 0.0f, OBJECT_SIZE * 0.2f);
			glRotatef(-m_Upper_Arm_Rotate_Value, 0.0f, 1.0f, 0.0f);
			glTranslatef(0.0f, 0.0f, OBJECT_SIZE * 0.2f);

			glScalef(m_Bench_Man->Get_Scale_X() * 0.1f, m_Bench_Man->Get_Scale_Y() * 0.1f, m_Bench_Man->Get_Scale_Z() * 0.4f);
			glColor4f(m_Bench_Man->Get_Color_R(), m_Bench_Man->Get_Color_G(), m_Bench_Man->Get_Color_B(), m_Bench_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // �Ͽ�2 Pop
			glPopMatrix(); // ���2 Pop

			glPushMatrix(); // �ٸ�1 Push
			glTranslatef(-OBJECT_SIZE * 0.1f, -OBJECT_SIZE * 0.8f, 0.0f);
			glScalef(m_Bench_Man->Get_Scale_X() * 0.1f, m_Bench_Man->Get_Scale_Y() * 0.8f, m_Bench_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Bench_Man->Get_Color_R(), m_Bench_Man->Get_Color_G(), m_Bench_Man->Get_Color_B(), m_Bench_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // �ٸ�1 Pop

			glPushMatrix(); // �ٸ�2 Push
			glTranslatef(OBJECT_SIZE * 0.1f, -OBJECT_SIZE * 0.8f, 0.0f);
			glScalef(m_Bench_Man->Get_Scale_X() * 0.1f, m_Bench_Man->Get_Scale_Y() * 0.8f, m_Bench_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Bench_Man->Get_Color_R(), m_Bench_Man->Get_Color_G(), m_Bench_Man->Get_Color_B(), m_Bench_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // �ٸ�2 Pop

			glPopMatrix(); // ��� ��� Translate & Rotate Pop
		}

		glPopMatrix(); // ��ġ ������ Translate & Rotate Pop
	}

	virtual void Update()
	{
		if (m_Upper_Arm_Rotate_Value > 90.0f || m_Upper_Arm_Rotate_Value < 0.0f) m_Arm_Rotate_Sign *= -1;
		m_Upper_Arm_Rotate_Value += BENCH_PRESS_SPEED * m_Arm_Rotate_Sign;
	}
};

