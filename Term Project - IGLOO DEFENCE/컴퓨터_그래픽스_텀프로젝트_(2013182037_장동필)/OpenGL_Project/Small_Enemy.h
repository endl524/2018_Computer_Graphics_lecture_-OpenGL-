#pragma once
#include "Object.h"

#define SNOW_BALL_HIT 999

class Small_Enemy : Object
{
private:
	float m_Speed = 3.0f;
	float m_Upper_Arm_Rot_Value[2]{ -60.0f, 40.0f };
	int m_Upper_Arm_Rot_Sign[2]{ 1, -1 };

	float m_Upper_Leg_Rot_Value[2]{ -80.0f, 40.0f };
	int m_Upper_Leg_Rot_Sign[2]{ 1, -1 };
	float m_Lower_Leg_Rot_Value[2]{ 80.0f, 10.0f };

	queue<Small_Enemy*>* m_Pool_pointer = nullptr;

	Object* m_Target = nullptr;

	OBB* m_Attack_OBB = nullptr;
	OBB* m_Detect_OBB = nullptr;
	bool m_is_Attack = false;
	bool m_Attack_OBB_Activate = false;

	bool m_is_Alive = false;
	float m_Rot_Val = 0.0f;
	float m_Rot_Axis[3];

	float m_Through_Vector[3];

public:

	virtual void HP_Reduce() 
	{ 
		m_HP -= 1;
		if (m_HP <= 0)
		{
			m_Target = nullptr;
			m_Position.x = 10000.0f;
			m_Position.z = 10000.0f;
			Small_Enemy_Initialize();
			Set_is_Alive(false);
			m_Pool_pointer->push(this);
			m_is_Move = false;
		}
	}
	virtual int Get_HP() { return m_HP; }

	void Set_Pool_pointer(queue<Small_Enemy*>* pool) { m_Pool_pointer = pool; }

	void Set_Target(Object* t) { m_Target = t; }

	void Set_is_Alive(const bool& b) { m_is_Alive = b; }

public:
	Small_Enemy() : Object() {}
	~Small_Enemy() 
	{
		if (m_Attack_OBB != nullptr) delete m_Attack_OBB;
		if (m_Detect_OBB != nullptr) delete m_Detect_OBB;
	}

	void Small_Enemy_Initialize()
	{
		Set_is_Static(false);

		m_Scale_X = 0.3f; m_Scale_Y = 0.5f; m_Scale_Z = 0.3f;

		m_Position.y = -OBJECT_SIZE * m_Scale_Y * 0.5f;
		
		if (m_Object_Type == SMALL_ENEMY_1)
		{
			m_HP = 2;
			m_Speed = 1.5f;
			m_Material.Ambient[0] = 0.3f; m_Material.Ambient[1] = 0.2f; m_Material.Ambient[2] = 0.2f; m_Material.Ambient[3] = 1.0f;
			m_Material.Diffuse[0] = 0.9f; m_Material.Diffuse[1] = 0.3f; m_Material.Diffuse[2] = 0.3f; m_Material.Diffuse[3] = 1.0f;
			m_Material.Specular[0] = 0.8f; m_Material.Specular[1] = 0.8f; m_Material.Specular[2] = 0.8f; m_Material.Specular[3] = 1.0f;
		}
		else if (m_Object_Type == SMALL_ENEMY_2)
		{
			m_HP = 1;
			m_Speed = 1.0f;
			m_Material.Ambient[0] = 0.3f; m_Material.Ambient[1] = 0.3f; m_Material.Ambient[2] = 0.2f; m_Material.Ambient[3] = 1.0f;
			m_Material.Diffuse[0] = 0.7f; m_Material.Diffuse[1] = 0.9f; m_Material.Diffuse[2] = 0.3f; m_Material.Diffuse[3] = 1.0f;
			m_Material.Specular[0] = 0.8f; m_Material.Specular[1] = 0.8f; m_Material.Specular[2] = 0.8f; m_Material.Specular[3] = 1.0f;
		}

		if (m_OBB == nullptr) Make_OBB();
		m_OBB->OBB_Init(m_Position, m_Scale_X, m_Scale_Y, m_Scale_Z);		

		m_Through_Vector[0] = 0.0f; m_Through_Vector[1] = 0.0f; m_Through_Vector[2] = 1.0f;

		if (m_Attack_OBB == nullptr) m_Attack_OBB = new OBB();
		Vector3 temp_v = m_Position;
		temp_v.x += m_Through_Vector[0] * OBJECT_SIZE * m_Scale_X * 2.0f;
		temp_v.y += m_Through_Vector[1] * OBJECT_SIZE * m_Scale_Y * 0.5f;
		temp_v.z += m_Through_Vector[2] * OBJECT_SIZE * m_Scale_Z * 2.0f;
		m_Attack_OBB->OBB_Init(temp_v, m_Scale_X * 1.2f, m_Scale_Y * 0.5f, m_Scale_Z * 1.2f);

		if (m_Detect_OBB == nullptr) m_Detect_OBB = new OBB();
		m_Detect_OBB->OBB_Init(m_Position, m_Scale_X * 2.5f, m_Scale_Y, m_Scale_Z * 2.5f);
	}


	virtual void Draw()
	{
		if (m_is_Alive)
		{
			glPushMatrix(); // ·Îº¿ ±×¸®±â ½ÃÀÛ

			glTranslatef(m_Position.x, m_Position.y, m_Position.z);
			//glMultMatrixf(m_Rotate_Transform_Matrix);
			glRotatef(m_Rot_Val, m_Rot_Axis[0], m_Rot_Axis[1], m_Rot_Axis[2]);
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
			glMateriali(GL_FRONT, GL_SHININESS, 40);

			Draw_Robot();

			glPopMatrix(); // ·Îº¿ ±×¸®±â ³¡

			/*
			// OBB ±×¸®±â
			glPushMatrix();
			glTranslatef(m_OBB->Get_Center_Position()[0], m_OBB->Get_Center_Position()[1], m_OBB->Get_Center_Position()[2]);
			glMultMatrixf(m_Rotate_Transform_Matrix);
			glScalef(m_OBB->Get_OBB_Axis_Distance()[0] / (OBJECT_SIZE * 0.5f), m_OBB->Get_OBB_Axis_Distance()[1] / (OBJECT_SIZE * 0.5f), m_OBB->Get_OBB_Axis_Distance()[2] / (OBJECT_SIZE * 0.5f));
			glutWireCube(OBJECT_SIZE);
			glPopMatrix();

			// ATTACK OBB ±×¸®±â
			glPushMatrix();
			glTranslatef(m_Attack_OBB->Get_Center_Position()[0], m_Attack_OBB->Get_Center_Position()[1], m_Attack_OBB->Get_Center_Position()[2]);
			glMultMatrixf(m_Rotate_Transform_Matrix);
			glScalef(m_Attack_OBB->Get_OBB_Axis_Distance()[0] / (OBJECT_SIZE * 0.5f), m_Attack_OBB->Get_OBB_Axis_Distance()[1] / (OBJECT_SIZE * 0.5f), m_Attack_OBB->Get_OBB_Axis_Distance()[2] / (OBJECT_SIZE * 0.5f));
			glutWireCube(OBJECT_SIZE);
			glPopMatrix();

			// DETECT OBB ±×¸®±â
			glPushMatrix();
			glTranslatef(m_Detect_OBB->Get_Center_Position()[0], m_Detect_OBB->Get_Center_Position()[1], m_Detect_OBB->Get_Center_Position()[2]);
			glMultMatrixf(m_Rotate_Transform_Matrix);
			glScalef(m_Detect_OBB->Get_OBB_Axis_Distance()[0] / (OBJECT_SIZE * 0.5f), m_Detect_OBB->Get_OBB_Axis_Distance()[1] / (OBJECT_SIZE * 0.5f), m_Detect_OBB->Get_OBB_Axis_Distance()[2] / (OBJECT_SIZE * 0.5f));
			glutWireCube(OBJECT_SIZE);
			glPopMatrix();
			*/
		}
	}

	void Draw_Robot()
	{
		{ // ·Îº¿ ¸öÅë ±×¸®±â
			glPushMatrix();
			glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

			glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
			glutSolidCube(OBJECT_SIZE);
			glColor4f(0.0f, 0.0f, 0.0f, m_Color.A);
			glutWireCube(OBJECT_SIZE * 1.01f);
			glPopMatrix();
		}

		{ // <<·Îº¿ ¸Ó¸® ºÎºÐ>>
			glPushMatrix();
			glTranslatef(0.0f, OBJECT_SIZE * 0.5f * m_Scale_Y * 1.3f, 0.0f);

			{ // ·Îº¿ ¸Ó¸®Åë ±×¸®±â
				glPushMatrix();
				glScalef(m_Scale_X, m_Scale_Y * 0.6f, m_Scale_Z);

				glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
				glutSolidSphere(OBJECT_SIZE * 0.5f, 20, 20);
				glPopMatrix();
			}

			{ // ·Îº¿ ÄÚ ±×¸®±â
				glPushMatrix();
				glTranslatef(0.0f, 0.0f, OBJECT_SIZE * 0.5f * m_Scale_Z * 0.6f);
				glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
				glScalef(m_Scale_X * 0.2f, m_Scale_Y * 0.5f, m_Scale_Z * 0.2f);

				glColor4f(0.0f, 0.0f, 1.0f, m_Color.A);
				glutSolidCone(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 20, 20);
				glPopMatrix();
			}

			glPopMatrix();
		}

		{ // <<·Îº¿ ¿ÞÆÈ ºÎºÐ>>
			glPushMatrix();
			glTranslatef(OBJECT_SIZE * 0.5f * m_Scale_X * 0.9f, OBJECT_SIZE * 0.5f * m_Scale_Y * 0.8f, 0.0f);
			glRotatef(35.0f, 0.0f, 0.0f, 1.0f);
			glRotatef(m_Upper_Arm_Rot_Value[0], 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.4f, 0.0f);

			{ // ·Îº¿ ÁÂ»ó¿Ï ±×¸®±â
				glPushMatrix();
				glScalef(m_Scale_X * 0.2f, m_Scale_Y * 0.4f, m_Scale_Z * 0.2f);

				glColor4f(0.0f, 0.5f, 1.0f, m_Color.A);
				glutSolidCube(OBJECT_SIZE);
				glColor4f(0.0f, 0.0f, 0.0f, m_Color.A);
				glutWireCube(OBJECT_SIZE * 1.01f);
				glPopMatrix();
			}

			{ // ·Îº¿ ÁÂÇÏ¿Ï ±×¸®±â
				glPushMatrix();
				glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.3f, OBJECT_SIZE * 0.5f * m_Scale_Z * 0.3f);
				glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
				glScalef(m_Scale_X * 0.15f, m_Scale_Y * 0.3f, m_Scale_Z * 0.15f);

				glColor4f(0.3f, 0.3f, 1.0f, m_Color.A);
				glutSolidCube(OBJECT_SIZE);
				glColor4f(0.0f, 0.0f, 0.0f, m_Color.A);
				glutWireCube(OBJECT_SIZE * 1.01f);
				glPopMatrix();
			}
			glPopMatrix();
		}

		{ // <<·Îº¿ ¿À¸¥ÆÈ ºÎºÐ>>
			glPushMatrix();
			glTranslatef(-OBJECT_SIZE * 0.5f * m_Scale_X * 0.9f, OBJECT_SIZE * 0.5f * m_Scale_Y * 0.8f, 0.0f);
			glRotatef(35.0f, 0.0f, 0.0f, -1.0f);
			glRotatef(m_Upper_Arm_Rot_Value[1], 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.4f, 0.0f);

			{ // ·Îº¿ ¿ì»ó¿Ï ±×¸®±â
				glPushMatrix();
				glScalef(m_Scale_X * 0.2f, m_Scale_Y * 0.4f, m_Scale_Z * 0.2f);

				glColor4f(0.0f, 0.5f, 1.0f, m_Color.A);
				glutSolidCube(OBJECT_SIZE);
				glColor4f(0.0f, 0.0f, 0.0f, m_Color.A);
				glutWireCube(OBJECT_SIZE * 1.01f);
				glPopMatrix();
			}

			{ // ·Îº¿ ¿ìÇÏ¿Ï ±×¸®±â
				glPushMatrix();
				glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.3f, OBJECT_SIZE * 0.5f * m_Scale_Z * 0.3f);
				glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
				glScalef(m_Scale_X * 0.15f, m_Scale_Y * 0.3f, m_Scale_Z * 0.15f);

				glColor4f(0.3f, 0.3f, 1.0f, m_Color.A);
				glutSolidCube(OBJECT_SIZE);
				glColor4f(0.0f, 0.0f, 0.0f, m_Color.A);
				glutWireCube(OBJECT_SIZE * 1.01f);
				glPopMatrix();
			}
			glPopMatrix();
		}

		{ // <<·Îº¿ ¿Þ´Ù¸® ºÎºÐ>>
			glPushMatrix();
			glTranslatef(OBJECT_SIZE * 0.5f * m_Scale_X * 0.6f, -OBJECT_SIZE * 0.5f * m_Scale_Y, 0.0f);
			glRotatef(m_Upper_Leg_Rot_Value[0], 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.5f, 0.0f);

			{ // ·Îº¿ ÁÂ»ó°¢ ±×¸®±â
				glPushMatrix();
				glScalef(m_Scale_X * 0.2f, m_Scale_Y * 0.6f, m_Scale_Z * 0.2f);

				glColor4f(0.0f, 0.5f, 1.0f, m_Color.A);
				glutSolidCube(OBJECT_SIZE);
				glColor4f(0.0f, 0.0f, 0.0f, m_Color.A);
				glutWireCube(OBJECT_SIZE * 1.01f);
				glPopMatrix();
			}

			{ // ·Îº¿ ÁÂÇÏ°¢ ±×¸®±â
				glPushMatrix();
				glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.5f, 0.0f);
				glRotatef(m_Lower_Leg_Rot_Value[0], 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.5f, 0.0f);
				glScalef(m_Scale_X * 0.15f, m_Scale_Y * 0.5f, m_Scale_Z * 0.15f);

				glColor4f(0.3f, 0.3f, 1.0f, m_Color.A);
				glutSolidCube(OBJECT_SIZE);
				glColor4f(0.0f, 0.0f, 0.0f, m_Color.A);
				glutWireCube(OBJECT_SIZE * 1.01f);
				glPopMatrix();
			}
			glPopMatrix();
		}

		{ // <<·Îº¿ ¿À¸¥´Ù¸® ºÎºÐ>>
			glPushMatrix();
			glTranslatef(-OBJECT_SIZE * 0.5f * m_Scale_X * 0.6f, -OBJECT_SIZE * 0.5f * m_Scale_Y, 0.0f);
			glRotatef(m_Upper_Leg_Rot_Value[1], 1.0f, 0.0f, 0.0f);
			glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.5f, 0.0f);

			{ // ·Îº¿ ¿ì»ó°¢ ±×¸®±â
				glPushMatrix();
				glScalef(m_Scale_X * 0.2f, m_Scale_Y * 0.6f, m_Scale_Z * 0.2f);

				glColor4f(0.0f, 0.5f, 1.0f, m_Color.A);
				glutSolidCube(OBJECT_SIZE);
				glColor4f(0.0f, 0.0f, 0.0f, m_Color.A);
				glutWireCube(OBJECT_SIZE * 1.01f);
				glPopMatrix();
			}

			{ // ·Îº¿ ¿ìÇÏ°¢ ±×¸®±â
				glPushMatrix();
				glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.5f, 0.0f);
				glRotatef(m_Lower_Leg_Rot_Value[1], 1.0f, 0.0f, 0.0f);
				glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.5f, 0.0f);
				glScalef(m_Scale_X * 0.15f, m_Scale_Y * 0.5f, m_Scale_Z * 0.15f);

				glColor4f(0.3f, 0.3f, 1.0f, m_Color.A);
				glutSolidCube(OBJECT_SIZE);
				glColor4f(0.0f, 0.0f, 0.0f, m_Color.A);
				glutWireCube(OBJECT_SIZE * 1.01f);
				glPopMatrix();
			}
			glPopMatrix();
		}

	}

	void Animation()
	{
		for (int i = 0; i < 2; ++i)
		{
			if (!m_is_Attack)
			{
				if (m_Upper_Arm_Rot_Value[i] <= -60.0f)
				{
					m_Upper_Arm_Rot_Value[i] = -60.0f;
					m_Upper_Arm_Rot_Sign[i] *= -1;
				}
				else if (m_Upper_Arm_Rot_Value[i] >= 40.0f)
				{
					m_Upper_Arm_Rot_Value[i] = 40.0f;
					m_Upper_Arm_Rot_Sign[i] *= -1;
				}
				m_Upper_Arm_Rot_Value[i] += 6.0f * m_Upper_Arm_Rot_Sign[i] * m_Speed;
			}
			else
			{
				if (m_Object_Type == SMALL_ENEMY_2) m_Upper_Arm_Rot_Value[i] += -m_Speed;
				if (m_Object_Type == SMALL_ENEMY_1) m_Upper_Arm_Rot_Value[i] += 3.0f * -m_Speed;
				if (m_Upper_Arm_Rot_Value[i] <= -100.0f)
				{
					m_Upper_Arm_Rot_Value[0] = -60.0f; m_Upper_Arm_Rot_Value[1] = 40.0f;
					m_Upper_Arm_Rot_Sign[0] = 1; m_Upper_Arm_Rot_Sign[1] = -1;
					m_Attack_OBB_Activate = false;
					m_is_Attack = false;
					m_is_Move = true;
				}
				else if (m_Upper_Arm_Rot_Value[i] <= -80.0f)
				{
					m_Attack_OBB_Activate = true;
				}
			}

			if (m_Upper_Leg_Rot_Value[i] <= -80.0f)
			{
				m_Upper_Leg_Rot_Value[i] = -80.0f;
				m_Upper_Leg_Rot_Sign[i] *= -1;
			}
			else if (m_Upper_Leg_Rot_Value[i] >= 40.0f)
			{
				m_Upper_Leg_Rot_Value[i] = 40.0f;
				m_Upper_Leg_Rot_Sign[i] *= -1;
			}
			m_Upper_Leg_Rot_Value[i] += 6.0f * m_Upper_Leg_Rot_Sign[i] * m_Speed;


			if (m_Upper_Leg_Rot_Value[i] <= 0.0f) m_Lower_Leg_Rot_Value[i] = -m_Upper_Leg_Rot_Value[i];
			else m_Lower_Leg_Rot_Value[i] = 10.0f;
		}
	}

	virtual void Update()
	{
		if (m_is_Alive)
		{
			Target_Chasing();
			Move();
			Animation();
		}
	}

	void Target_Chasing()
	{
		if (m_Target != nullptr)
		{
			float v = 0.0f;
			float p[3];
			p[0] = m_Target->Get_Position_X() - m_Position.x; p[1] = 0.0f; p[2] = m_Target->Get_Position_Z() - m_Position.z;
			float size = sqrtf(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
			p[0] /= size; p[1] /= size; p[2] /= size;

			Vector_DotProduct(v, m_Forward_Vector, p, false);
			m_Rot_Val = acosf(v) * (180.0f / 3.1416f);
			Vector_CrossProduct(m_Rot_Axis, m_Forward_Vector, p);

			m_Through_Vector[0] = p[0]; m_Through_Vector[1] = p[1]; m_Through_Vector[2] = p[2];
		}
	}

	virtual void Move()
	{
		if (m_is_Move)
		{
			m_Position.x += m_Through_Vector[0] * m_Speed; //m_Position.y += m_Through_Vector[1] * m_Speed; 
			m_Position.z += m_Through_Vector[2] * m_Speed;

			if (m_OBB != nullptr) m_OBB->Set_Center_Position(m_Position);
			if (m_Attack_OBB != nullptr)
			{
				Vector3 temp_v = m_Position;
				temp_v.x += m_Through_Vector[0] * OBJECT_SIZE * m_Scale_X * 2.0f;
				temp_v.y += m_Through_Vector[1] * OBJECT_SIZE * m_Scale_Y * 0.5f;
				temp_v.z += m_Through_Vector[2] * OBJECT_SIZE * m_Scale_Z * 2.0f;
				m_Attack_OBB->Set_Center_Position(temp_v);
			}
			if (m_Detect_OBB != nullptr) m_Detect_OBB->Set_Center_Position(m_Position);
		}
	}

	virtual bool Collision_Check(Object* other)
	{
		if (!m_is_Static_Object && !other->Get_is_Static())
		{
			if (other->Get_OBB() != nullptr)
			{
				if (other == m_Target && m_Attack_OBB->OBB_Intersection(other->Get_OBB()))
				{
					if (m_Attack_OBB_Activate)
					{
						other->HP_Reduce();
						m_Upper_Arm_Rot_Value[0] = -60.0f; m_Upper_Arm_Rot_Value[1] = 40.0f;
						m_Upper_Arm_Rot_Sign[0] = 1; m_Upper_Arm_Rot_Sign[1] = -1;
						m_Attack_OBB_Activate = false;
						m_is_Attack = false;
						m_is_Move = true;
					}
				}
				if (other == m_Target && m_Detect_OBB->OBB_Intersection(other->Get_OBB()))
				{
					if (!m_is_Attack)
					{
						m_Upper_Arm_Rot_Value[0] = 80.0f; m_Upper_Arm_Rot_Value[1] = 80.0f;
						m_is_Attack = true;
						m_Attack_OBB_Activate = false;
						m_is_Move = false;
					}
				}
			}
		}

		return false;
	}

	virtual void Collision_Procedure(const int& collision_type, Object* other)
	{
		switch (collision_type)
		{
		case OBJECT_COLLIDE_PROCEDURE:
			//m_Position.x = m_Prev_Position.x; m_Position.z = m_Prev_Position.z;
			if (m_OBB != nullptr) m_OBB->Set_Center_Position(m_Position);

			break;

		//case SNOW_BALL_HIT: HP_Reduce(); break;
		}
	}

	void Vector_CrossProduct(float* result, const float* p1, const float* p2)
	{
		result[0] = p1[1] * p2[2] - p1[2] * p2[1];
		result[1] = p1[2] * p2[0] - p1[0] * p2[2];
		result[2] = p1[0] * p2[1] - p1[1] * p2[0];
	}

	void Vector_DotProduct(float& result, const float* p1, const float* p2, const bool& is_it_Normal)
	{
		if (is_it_Normal)
		{
			result = (p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2]) /
				(sqrtf(p1[0] * p1[0] + p1[1] * p1[1] + p1[2] * p1[2]) * sqrtf(p2[0] * p2[0] + p2[1] * p2[1] + p2[2] * p2[2]));
		}
		else
		{
			result = (p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2]);
		}
	}
};