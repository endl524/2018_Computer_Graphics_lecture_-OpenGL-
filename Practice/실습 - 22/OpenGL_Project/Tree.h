#pragma once
#include "Object.h"

class Tree : Object
{
private:
	Object* m_Leaves = nullptr;
	Object* m_Shadow = nullptr;
	int m_Auto_Scale_Sign = 1;

public:
	Tree() { Tree_Initialize(); };
	~Tree() 
	{
		if (m_Leaves != nullptr)
		{
			delete m_Leaves;
			m_Leaves = nullptr;
		}

		if (m_Shadow != nullptr)
		{
			delete m_Shadow;
			m_Shadow = nullptr;
		}
	};

	void Tree_Initialize()
	{
		m_Object_Type = TREE_OBJECT;
		m_is_Static_Object = true;

		// 기둥 속성
		m_Scale_X = 0.7f; m_Scale_Y = 5.0f; m_Scale_Z = 0.7f;

		m_Position.x = OBJECT_SIZE * 0.5f * 16.0f;
		m_Position.y = OBJECT_SIZE * 0.5f * m_Scale_Y ;
		m_Position.z = OBJECT_SIZE * 0.5f * 16.0f;
		
		Collider_Update();

		m_Color.R = 1.0f; m_Color.G = 0.6f; m_Color.B = 0.2f; m_Color.A = 1.0f;

		// 잎파리들 속성
		m_Leaves = new Object(false);
		m_Leaves->Init_Object();
		m_Leaves->Set_Scale(3.0f, 3.0f, 3.0f);
		m_Leaves->Set_Translate(0.0f, m_Position.y, 0.0f);
		m_Leaves->Set_Color(0.3f, 1.0f, 0.3f, 0.7f);

		// 그림자 속성
		m_Shadow = new Object(false);
		m_Shadow->Init_Object();
		m_Shadow->Set_Scale(4.0f, 1.0f, 4.0f);
		m_Shadow->Set_Translate(0.0f, -m_Position.y+1.0f, 0.0f);
		m_Shadow->Set_Color(0.2f, 0.2f, 0.2f, 1.0f);
	}

	virtual void Draw()
	{
		// 나무 기둥 그리기

		glPushMatrix(); // 나무 기둥 Translate & Rotate Push

		// 변환 적용.
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		Rotate_Check();
		glMultMatrixf(m_Rotate_Transform_Matrix);

		glPushMatrix(); // 기둥 Scale Push
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidCube(OBJECT_SIZE);
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glutWireCube(OBJECT_SIZE * 1.01f);
		glPopMatrix(); // 기둥 Scale Pop


		// 잎파리들 그리기
		if (m_Leaves)
		{
			glPushMatrix(); // 잎파리들 Translate & Rotate Push
			glTranslatef(m_Leaves->Get_Position_X(), m_Leaves->Get_Position_Y(), m_Leaves->Get_Position_Z());
			m_Leaves->Rotate_Check();
			glMultMatrixf(m_Leaves->Get_Rotate_Transform_Matrix());

			glPushMatrix(); // 잎파리들 Scale Push
			glScalef(m_Leaves->Get_Scale_X(), m_Leaves->Get_Scale_Y(), m_Leaves->Get_Scale_Z());

			glColor4f(m_Leaves->Get_Color_R(), m_Leaves->Get_Color_G(), m_Leaves->Get_Color_B(), m_Leaves->Get_Color_A());
			glutWireSphere(OBJECT_SIZE * 0.5f, 30, 30);
			glPopMatrix(); // 잎파리들 Scale Pop
			glPopMatrix(); // 잎파리들 Translate & Rotate Pop
		}

		// 그림자 그리기
		if (m_Shadow)
		{
			glPushMatrix(); // 그림자 Translate & Rotate Push
			glTranslatef(m_Shadow->Get_Position_X(), m_Shadow->Get_Position_Y(), m_Shadow->Get_Position_Z());
			m_Shadow->Rotate_Check();
			glMultMatrixf(m_Shadow->Get_Rotate_Transform_Matrix());

			glPushMatrix(); // 그림자 Scale Push
			glScalef(m_Shadow->Get_Scale_X(), m_Shadow->Get_Scale_Y(), m_Shadow->Get_Scale_Z());

			glColor4f(m_Shadow->Get_Color_R(), m_Shadow->Get_Color_G(), m_Shadow->Get_Color_B(), m_Shadow->Get_Color_A());
			glBegin(GL_TRIANGLES);
			glVertex3f(0.0f, 0.0f, -OBJECT_SIZE * 0.5f);
			glVertex3f(-OBJECT_SIZE * 0.5f, 0.0f, OBJECT_SIZE * 0.5f);
			glVertex3f(OBJECT_SIZE * 0.5f, 0.0f, OBJECT_SIZE * 0.5f);
			glEnd();

			glPopMatrix(); // 그림자 Scale Pop
			glPopMatrix(); // 그림자 Translate & Rotate Pop
		}
		glPopMatrix(); // 나무 기둥 Translate & Rotate Pop
	}

	void Auto_Scale()
	{
		m_Leaves->Set_Scale(m_Leaves->Get_Scale_X() + OBJECT_AUTO_SCALE_VALUE * m_Auto_Scale_Sign
			, m_Leaves->Get_Scale_Y() + OBJECT_AUTO_SCALE_VALUE * m_Auto_Scale_Sign
			, m_Leaves->Get_Scale_Z() + OBJECT_AUTO_SCALE_VALUE * m_Auto_Scale_Sign);
		if (m_Leaves->Get_Scale_X() >= 4.0f || m_Leaves->Get_Scale_X() <= 2.0f) m_Auto_Scale_Sign *= -1;
	}

	virtual void Update()
	{
		Auto_Scale();
	}
};

