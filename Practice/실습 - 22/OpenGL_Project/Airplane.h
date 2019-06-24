#pragma once
#include "Object.h"

class Airplane : Object
{
private:
	float m_Propeller_Rotate_Value = 0.0f;
	float m_Airplane_Roll_Value = 0.0f;
	float m_Airplane_Yaw_Stack_Value = 0.0f;
	Object* m_Fogs[4]{ nullptr };

public:
	Object* Get_Fog_Object(const int& index) { return m_Fogs[index]; }

public:
	Airplane() { Airplane_Initialize(); }
	~Airplane()
	{
		if (m_Fogs[0] != nullptr)
		{
			delete[] m_Fogs;
			m_Fogs[0] = nullptr;
			m_Fogs[1] = nullptr;
			m_Fogs[2] = nullptr;
			m_Fogs[3] = nullptr;
		}
	}

	void Airplane_Initialize()
	{
		m_Object_Type = AIRPLANE_OBJECT;
		m_is_Static_Object = true;

		// 비행기 속성
		m_Position.x = 300.0f;
		m_Position.y = 300.0f;
		m_Position.z = 0.0f;

		Collider_Update();

		for (int i = 0; i < 4; ++i)
		{
			m_Fogs[i] = new Object();
			m_Fogs[i]->Set_Object_Type(FOG_OBJECT);
			m_Fogs[i]->Set_Color(0.7f, 0.7f, 0.7f, 1.0f);
			m_Fogs[i]->Set_Translate(m_Position.x, m_Position.y, m_Position.z);
			m_Fogs[i]->Set_Scale(0.0f, 0.0f, 0.0f);
			m_Fogs[i]->Collider_Update();
			m_Fogs[i]->Set_is_Static(true);
		}
	}

	virtual void Draw()
	{
		// 비행기 그리기
		glPushMatrix(); // 비행기 Translate & Rotate Push

		// 변환 적용.
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glRotatef(m_Airplane_Roll_Value, 0.0f, 0.0f, 1.0f);

		glPushMatrix(); // 비행기 몸통 그리기
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z * 3.0f);
		glColor4f(1.0f, 1.0f, 0.4f, 1.0f);
		glutSolidSphere(OBJECT_SIZE * 0.5f, 20, 20);
		glPopMatrix(); // 비행기 몸통 Pop

		glPushMatrix(); // 비행기 날개 그리기
		glScalef(m_Scale_X * 3.0f, m_Scale_Y * 0.01f, m_Scale_Z);
		glColor4f(1.0f, 0.5f, 0.2f, 1.0f);
		glutSolidSphere(OBJECT_SIZE * 0.5f, 20, 20);
		glPopMatrix(); // 비행기 날개 Pop

		glPushMatrix(); // 비행기 꼬리 그리기
		glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.5f, OBJECT_SIZE * m_Scale_Z * 1.5f);
		glPushMatrix();
		glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
		glScalef(m_Scale_X * 0.01f, m_Scale_Y, m_Scale_Z);
		glColor4f(1.0f, 0.5f, 0.2f, 1.0f);
		glutSolidCone(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 20, 20);
		glPopMatrix();
		glPopMatrix(); // 비행기 꼬리 Pop

		glPushMatrix(); // 비행기 프로펠러 그리기

		glTranslatef(0.0f, 0.0f, -OBJECT_SIZE * m_Scale_Z * 1.5f);
		glPushMatrix();
		glScalef(m_Scale_X * 0.3f, m_Scale_Y * 0.3f, m_Scale_Z * 0.5f);
		glColor4f(1.0f, 0.5f, 0.2f, 1.0f);
		glutSolidSphere(OBJECT_SIZE * 0.5f, 20, 20); // 중앙
		glPopMatrix();

		glPushMatrix();
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(m_Propeller_Rotate_Value, 0.0f, 1.0f, 0.0f);

		glPushMatrix(); // 프로펠러 1
		glTranslatef(0.0f, 0.0f, OBJECT_SIZE * m_Scale_Z * 0.4f);
		glScalef(m_Scale_X * 0.3f, m_Scale_Y * 0.1f, m_Scale_Z * 0.9f);
		glColor4f(0.8f, 1.0f, 0.6f, 1.0f);
		glutSolidSphere(OBJECT_SIZE * 0.5f, 20, 20);
		glPopMatrix();

		glPushMatrix(); // 프로펠러 2
		glRotatef(120.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, OBJECT_SIZE * m_Scale_Z * 0.4f);
		glScalef(m_Scale_X * 0.3f, m_Scale_Y * 0.1f, m_Scale_Z * 0.9f);
		glColor4f(0.6f, 1.0f, 0.4f, 1.0f);
		glutSolidSphere(OBJECT_SIZE * 0.5f, 20, 20);
		glPopMatrix();

		glPushMatrix(); // 프로펠러 3
		glRotatef(240.0f, 0.0f, 1.0f, 0.0f);
		glTranslatef(0.0f, 0.0f, OBJECT_SIZE * m_Scale_Z * 0.4f);
		glScalef(m_Scale_X * 0.3f, m_Scale_Y * 0.1f, m_Scale_Z * 0.9f);
		glColor4f(0.4f, 1.0f, 0.2f, 1.0f);
		glutSolidSphere(OBJECT_SIZE * 0.5f, 20, 20);
		glPopMatrix();

		glPopMatrix();

		glPopMatrix(); // 비행기 프로펠러 Pop

		glPopMatrix(); // 비행기 Translate & Rotate Pop
	}

	virtual void Update()
	{
		m_Propeller_Rotate_Value += AIRPLANE_PROPELLER_ROTATE_VALUE;
		if (m_Propeller_Rotate_Value >= 360.0f) m_Propeller_Rotate_Value = m_Propeller_Rotate_Value - 360.0f;

		Yaw(-3.0f);
		m_Airplane_Yaw_Stack_Value += 3.0f;
		if (m_Airplane_Yaw_Stack_Value >= 360.0f) m_Airplane_Yaw_Stack_Value = m_Airplane_Yaw_Stack_Value - 360.0f;
	}

	void Set_Fog_Objects()
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_Fogs[i]->Get_Scale_X() <= 0.0f)
			{
				m_Fogs[i]->Yaw(m_Airplane_Yaw_Stack_Value);
				m_Fogs[i]->Set_Scale(2.0f, 2.0f, 2.0f);
				break;
			}
		}
		
	}

	virtual void Roll(const float& value) 
	{
		m_Airplane_Roll_Value += value;

		if (m_Airplane_Roll_Value >= 360.0f) m_Airplane_Roll_Value = m_Airplane_Roll_Value - 360.0f;
		else if (m_Airplane_Roll_Value <= -360.0f) m_Airplane_Roll_Value = m_Airplane_Roll_Value + 360.0f;
	}
};

