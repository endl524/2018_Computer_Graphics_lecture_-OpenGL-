#pragma once
#include "Object.h"

class Bar : Object
{
private:
	Object* m_Bar_Man = nullptr;
	float m_Bar_Man_Rotate_Value = 0.0f;
	int m_Bar_Man_Speed_Value = 1;

public:
	Bar() { Bar_Initialize(); };
	~Bar()
	{
		if (m_Bar_Man != nullptr)
		{
			delete m_Bar_Man;
			m_Bar_Man = nullptr;
		}
	};

	void Bar_Initialize()
	{
		m_Object_Type = BAR_OBJECT;

		// ö�� �Ӽ�
		m_Position.x = OBJECT_SIZE * 0.5f * 16.0f;
		m_Position.y = 0.0f;
		m_Position.z = OBJECT_SIZE * 0.5f * -16.0f;

		Collider_Update();

		m_Rotate_Yaw_Value = 45.0f;

		m_Color.R = 1.0f; m_Color.G = 0.2f; m_Color.B = 0.2f; m_Color.A = 1.0f;

		// �Ŵ޸� ��� �Ӽ�
		m_Bar_Man = new Object(false);
		m_Bar_Man->Set_Translate(0.0f, -OBJECT_SIZE * 1.2f, 0.0f);
		m_Bar_Man->Set_Color(1.0f, 0.2f, 0.5f, 1.0f);
	}

	virtual void Draw()
	{
		// ö�� �׸���

		glPushMatrix(); // ö�� Translate & Rotate Push

		// ��ȯ ����.
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);

		glPushMatrix(); // ö�� ���1 �׸���
		glTranslatef(-OBJECT_SIZE * m_Scale_X, OBJECT_SIZE * 0.5f * m_Scale_Y * 4.0f, 0.0f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 4.0f, m_Scale_X * 0.1f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ö�� ���1 Pop

		glPushMatrix(); // ö�� ���2 �׸���
		glTranslatef(OBJECT_SIZE * m_Scale_X, OBJECT_SIZE * 0.5f * m_Scale_Y * 4.0f, 0.0f);
		glScalef(m_Scale_X * 0.1f, m_Scale_Y * 4.0f, m_Scale_X * 0.1f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ö�� ���2 Pop

		glPushMatrix(); // ö�� Bar �׸���
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 4.0f, 0.0f);
		glScalef(m_Scale_X * 3.0f, m_Scale_Y * 0.1f, m_Scale_Z * 0.1f);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // ö�� Bar Pop


		// �Ŵ޸� ��� �׸���
		if (m_Bar_Man)
		{
			glPushMatrix(); // �Ŵ޸� ��� Translate & Rotate Push
			glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 4.0f, 0.0f);
			glRotatef(m_Bar_Man_Rotate_Value, 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, m_Bar_Man->Get_Position_Y(), 0.0f);

			glPushMatrix(); // �Ӹ� Push
			glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.7f, 0.0f);
			glScalef(m_Bar_Man->Get_Scale_X() * 0.4f, m_Bar_Man->Get_Scale_Y() * 0.3f, m_Bar_Man->Get_Scale_Z() * 0.4f);
			glColor4f(m_Bar_Man->Get_Color_R(), m_Bar_Man->Get_Color_G(), m_Bar_Man->Get_Color_B(), m_Bar_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // �Ӹ� Pop

			glPushMatrix(); // ���� Push
			glScalef(m_Bar_Man->Get_Scale_X() * 0.5f, m_Bar_Man->Get_Scale_Y(), m_Bar_Man->Get_Scale_Z() * 0.3f);
			glColor4f(m_Bar_Man->Get_Color_R(), m_Bar_Man->Get_Color_G(), m_Bar_Man->Get_Color_B(), m_Bar_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // ���� Pop

			glPushMatrix(); // ��1 Push
			glTranslatef(-OBJECT_SIZE * 0.3f, OBJECT_SIZE * 0.8f, 0.0f);
			glScalef(m_Bar_Man->Get_Scale_X() * 0.1f, m_Bar_Man->Get_Scale_Y() * 0.8f, m_Bar_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Bar_Man->Get_Color_R(), m_Bar_Man->Get_Color_G(), m_Bar_Man->Get_Color_B(), m_Bar_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // ��1 Pop

			glPushMatrix(); // ��2 Push
			glTranslatef(OBJECT_SIZE * 0.3f, OBJECT_SIZE * 0.8f, 0.0f);
			glScalef(m_Bar_Man->Get_Scale_X() * 0.1f, m_Bar_Man->Get_Scale_Y() * 0.8f, m_Bar_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Bar_Man->Get_Color_R(), m_Bar_Man->Get_Color_G(), m_Bar_Man->Get_Color_B(), m_Bar_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // ��2 Pop

			glPushMatrix(); // �ٸ�1 Push
			glTranslatef(-OBJECT_SIZE * 0.1f, -OBJECT_SIZE * 0.8f, 0.0f);
			glScalef(m_Bar_Man->Get_Scale_X() * 0.1f, m_Bar_Man->Get_Scale_Y() * 0.8f, m_Bar_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Bar_Man->Get_Color_R(), m_Bar_Man->Get_Color_G(), m_Bar_Man->Get_Color_B(), m_Bar_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // �ٸ�1 Pop

			glPushMatrix(); // �ٸ�2 Push
			glTranslatef(OBJECT_SIZE * 0.1f, -OBJECT_SIZE * 0.8f, 0.0f);
			glScalef(m_Bar_Man->Get_Scale_X() * 0.1f, m_Bar_Man->Get_Scale_Y() * 0.8f, m_Bar_Man->Get_Scale_Z() * 0.1f);
			glColor4f(m_Bar_Man->Get_Color_R(), m_Bar_Man->Get_Color_G(), m_Bar_Man->Get_Color_B(), m_Bar_Man->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // �ٸ�2 Pop

			glPopMatrix(); // �Ŵ޸� ��� Translate & Rotate Pop
		}

		glPopMatrix(); // ö�� Translate & Rotate Pop
	}

	virtual void Update()
	{
		m_Bar_Man_Rotate_Value += BAR_MAN_ROTATE_VALUE * m_Bar_Man_Speed_Value;
		if (m_Bar_Man_Rotate_Value >= 360.0f) m_Bar_Man_Rotate_Value = m_Bar_Man_Rotate_Value - 360.0f;
		m_Bar_Man_Speed_Value = m_Bar_Man_Rotate_Value >= 210.0f || m_Bar_Man_Rotate_Value <= 150.0f ? 2.0f : 1.0f;
	}
};

