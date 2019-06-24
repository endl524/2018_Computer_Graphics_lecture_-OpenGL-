#pragma once
#include "Object.h"

class Crane : Object
{
private:
	Object* m_Middle_Object = nullptr;
	Object* m_Upper_Object = nullptr;
	float m_Stacked_Mid_Objs_Pitch = 0.0f;
	float m_Stacked_Mid_Objs_Yaw = 0.0f;
	float m_Stacked_Upp_Objs_Pitch = 0.0f;
	float m_Stacked_Upp_Objs_Roll = 0.0f;

public:
	virtual void Yaw(const float& value) { m_Rotate_Yaw_Value = value; }

public:
	Crane() { Crane_Initialize(); };
	~Crane()
	{
		if (m_Middle_Object != nullptr)
		{
			delete m_Middle_Object;
			m_Middle_Object = nullptr;
		}

		if (m_Upper_Object != nullptr)
		{
			delete m_Upper_Object;
			m_Upper_Object = nullptr;
		}
	};

	void Crane_Initialize()
	{
		m_Object_Type = CRANE_OBJECT;
		m_Auto_Move_Dir = 0;
		m_is_Static_Object = false;

		// 기둥 속성
		m_Scale_X = 1.5f; m_Scale_Y = 1.0f; m_Scale_Z = 1.5f;

		m_Position.x = 0.0f;
		m_Position.y = OBJECT_SIZE * 0.5f * m_Scale_Y;
		m_Position.z = OBJECT_SIZE * 0.5f * 16.0f;

		m_Color.R = 1.0f; m_Color.G = 0.0f; m_Color.B = 0.0f; m_Color.A = 1.0f;

		// 크레인 중단부 생성/설정
		m_Middle_Object = new Object(false);
		m_Middle_Object->Init_Object();
		m_Middle_Object->Set_Color(0.0f, 1.0f, 0.0f, 1.0f);
		m_Middle_Object->Set_Scale(m_Scale_X * 0.2f, m_Scale_Y, m_Scale_Z * 0.2f);
		m_Middle_Object->Set_Translate(0.0f, OBJECT_SIZE * m_Scale_Y, 0.0f);

		// 크레인 상단부 생성/설정
		m_Upper_Object = new Object(false);
		m_Upper_Object->Init_Object();
		m_Upper_Object->Set_Color(0.0f, 0.0f, 1.0f, 1.0f);
		m_Upper_Object->Set_Scale(m_Scale_X * 0.1f, m_Scale_Y, m_Scale_Z * 0.1f);
		m_Upper_Object->Set_Translate(0.0f, OBJECT_SIZE * m_Middle_Object->Get_Scale_Y(), 0.0f);

		m_Stacked_Mid_Objs_Pitch = 0.0f;
		m_Stacked_Mid_Objs_Yaw = 0.0f;
		m_Stacked_Upp_Objs_Pitch = 0.0f;
		m_Stacked_Upp_Objs_Roll = 0.0f;
	}

	virtual void Draw()
	{
		// 크레인 하단부 그리기.

		glPushMatrix(); // 다음의 변환들을 스택에 넣는다.

		// 변환 적용.
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);

		glPushMatrix(); // Lower Object Scale Push
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // Lower Object Scale Pop


		// 크레인 중단부 그리기.
		if (m_Middle_Object)
		{
			glPushMatrix(); // Middle Object Translate & Rotate Push
			glTranslatef(0.0f, m_Middle_Object->Get_Position_Y() * 0.5f, 0.0f);
			glRotatef(m_Stacked_Mid_Objs_Pitch, 1.0f, 0.0f, 0.0f);
			glRotatef(m_Stacked_Mid_Objs_Yaw, 0.0f, 1.0f, 0.0f);
			glTranslatef(m_Middle_Object->Get_Position_X(), m_Middle_Object->Get_Position_Y() * 0.5f, m_Middle_Object->Get_Position_Z());

			glPushMatrix(); // Middle Object Scale Push
			glScalef(m_Middle_Object->Get_Scale_X(), m_Middle_Object->Get_Scale_Y(), m_Middle_Object->Get_Scale_Z());

			glColor4f(m_Middle_Object->Get_Color_R(), m_Middle_Object->Get_Color_G(), m_Middle_Object->Get_Color_B(), m_Middle_Object->Get_Color_A());
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix(); // Middle Object Scale Pop


			// 크레인 상단부 그리기.
			if (m_Upper_Object)
			{
				glTranslatef(0.0f, m_Upper_Object->Get_Position_Y() * 0.5f, 0.0f);
				glRotatef(m_Stacked_Upp_Objs_Pitch, 1.0f, 0.0f, 0.0f);
				glRotatef(m_Stacked_Upp_Objs_Roll, 0.0f, 0.0f, 1.0f);
				glTranslatef(m_Upper_Object->Get_Position_X(), m_Upper_Object->Get_Position_Y() * 0.5f, m_Upper_Object->Get_Position_Z());

				glScalef(m_Upper_Object->Get_Scale_X(), m_Upper_Object->Get_Scale_Y(), m_Upper_Object->Get_Scale_Z());

				glColor4f(m_Upper_Object->Get_Color_R(), m_Upper_Object->Get_Color_G(), m_Upper_Object->Get_Color_B(), m_Upper_Object->Get_Color_A());
				glutSolidCube(OBJECT_SIZE);
				glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
				glutWireCube(OBJECT_SIZE * 1.01f);
			}
			glPopMatrix(); // Middle Object Translate & Rotate Pop
		}
		glPopMatrix(); // 스택의 top에 있는 변환들을 꺼낸다.
	}




	void Middle_Rotate_X(const float& value)
	{
		if (m_Stacked_Mid_Objs_Pitch + value <= 90.0f && m_Stacked_Mid_Objs_Pitch + value >= -90.0f)
		{
			m_Stacked_Mid_Objs_Pitch += value;
		}
	}
	void Middle_Rotate_Y(const float& value)
	{
		if (m_Stacked_Mid_Objs_Yaw + value <= 90.0f && m_Stacked_Mid_Objs_Yaw + value >= -90.0f)
		{
			m_Stacked_Mid_Objs_Yaw += value;
		}
	}

	void Upper_Rotate_X(const float& value)
	{
		if (m_Stacked_Upp_Objs_Pitch + value <= 90.0f && m_Stacked_Upp_Objs_Pitch + value >= -90.0f)
		{
			m_Stacked_Upp_Objs_Pitch += value;
		}
	}
	void Upper_Rotate_Z(const float& value)
	{
		if (m_Stacked_Upp_Objs_Roll + value <= 90.0f && m_Stacked_Upp_Objs_Roll + value >= -90.0f)
		{
			m_Stacked_Upp_Objs_Roll += value;
		}
	}

	virtual void Set_Move_Direction(const int& dir) { m_Auto_Move_Dir = dir; }

	virtual void Auto_Move()
	{
		switch (m_Auto_Move_Dir)
		{
		case 0:
			if (m_Position.z - OBJECT_SIZE * 0.5f * m_Scale_Z + OBJECT_AUTO_MOVE_VALUE < -OBJECT_SIZE * 10.0f)
				m_Auto_Move_Dir = 1;
			m_Position.z -= OBJECT_AUTO_MOVE_VALUE;
			break;

		case 1:
			if (m_Position.z + OBJECT_SIZE * 0.5f * m_Scale_Z + OBJECT_AUTO_MOVE_VALUE > OBJECT_SIZE * 10.0f)
				m_Auto_Move_Dir = 0;
			m_Position.z += OBJECT_AUTO_MOVE_VALUE;
			break;

		case 2:
			if (m_Position.x - OBJECT_SIZE * 0.5f * m_Scale_X + OBJECT_AUTO_MOVE_VALUE < -OBJECT_SIZE * 10.0f)
				m_Auto_Move_Dir = 3;
			m_Position.x -= OBJECT_AUTO_MOVE_VALUE;
			break;

		case 3:
			if (m_Position.x + OBJECT_SIZE * 0.5f * m_Scale_X + OBJECT_AUTO_MOVE_VALUE > OBJECT_SIZE * 10.0f)
				m_Auto_Move_Dir = 2;
			m_Position.x += OBJECT_AUTO_MOVE_VALUE;
			break;
		}


		Collider_Update();
	}

	virtual void Update()
	{
		Auto_Move();
	}
};

