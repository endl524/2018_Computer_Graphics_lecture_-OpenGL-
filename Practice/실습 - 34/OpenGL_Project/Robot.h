#pragma once
#include "Object.h"
#include "Particle_Object.h"

#define EDGE_COLLIDE_PROCEDURE 0
#define OBJECT_COLLIDE_PROCEDURE 1

class Robot : Object
{
private:
	Vector3 m_Prev_Position;
	float m_Forward_Dir_Value = 0.0f;
	float m_Backward_Dir_Value = 0.0f;
	float m_Left_Dir_Value = 0.0f;
	float m_Right_Dir_Value = 0.0f;
	float m_Speed;

	float m_Upper_Arm_Rot_Value[2]{ -60.0f, 40.0f };
	int m_Upper_Arm_Rot_Sign[2]{ 1, -1 };

	float m_Upper_Leg_Rot_Value[2]{ -80.0f, 40.0f };
	int m_Upper_Leg_Rot_Sign[2]{ 1, -1 };
	float m_Lower_Leg_Rot_Value[2]{ -80.0f, 10.0f };


	float m_Yaw_Rotate_Stack = 0.0f;

	bool m_is_Jumped = false;
	bool m_is_Jumped_Max_Point = false;

	Light_Object* m_Spot_Light = nullptr;

public:
	virtual void Yaw(const float& value) { m_Rotate_Yaw_Value = value; m_Yaw_Rotate_Stack += value; Rotate_Check(); }

public:
	Robot() : Object() { Robot_Initialize(); };
	~Robot() 
	{ 
		if (m_Spot_Light != nullptr) { delete m_Spot_Light; m_Spot_Light = nullptr; }
	};

	void Robot_Initialize()
	{
		m_Object_Type = ROBOT_OBJECT;
		Set_is_Static(false);

		m_Speed = 3.0f;

		m_Scale_X = 0.3f; m_Scale_Y = 0.5f; m_Scale_Z = 0.3f;

		m_Position.x = 0.0f;
		m_Position.y = -OBJECT_SIZE * m_Scale_Y * 0.5f;
		m_Position.z = OBJECT_SIZE * 5.0f;

		m_Color.R = 1.0f; m_Color.G = 0.0f; m_Color.B = 0.0f; m_Color.A = 1.0f;
		m_Material.Ambient[0] = 0.1f; m_Material.Ambient[1] = 0.2f; m_Material.Ambient[2] = 0.1f; m_Material.Ambient[3] = 1.0f;
		m_Material.Diffuse[0] = 0.5f; m_Material.Diffuse[1] = 1.0f; m_Material.Diffuse[2] = 0.5f; m_Material.Diffuse[3] = 1.0f;
		m_Material.Specular[0] = 1.0f; m_Material.Specular[1] = 1.0f; m_Material.Specular[2] = 1.0f; m_Material.Specular[3] = 1.0f;
		
		Make_OBB();
		m_OBB->OBB_Init(m_Position, m_Scale_X, m_Scale_Y * 10.0f, m_Scale_Z);

		m_Spot_Light = new Light_Object(2);
		reinterpret_cast<Object*>(m_Spot_Light)->Set_Translate(0.0f, OBJECT_SIZE * 3.0f, 0.0f);
		reinterpret_cast<Object*>(m_Spot_Light)->Pitch(90.0f);
		m_Spot_Light->Set_Material_Ambient_Color(0.15f, 0.15f, 0.15f, 1.0f);
		m_Spot_Light->Set_Material_Diffuse_Color(0.7f, 0.7f, 0.7f, 1.0f);
		m_Spot_Light->Set_Material_Specular_Color(1.0f, 1.0f, 1.0f, 1.0f);
		m_Spot_Light->Set_Light_Position(0.0f, 0.0f, 0.0f, 1.0f);
		m_Spot_Light->Set_Light_Spot_CutOff(15.0f);
		m_Spot_Light->Set_Light_Spot_Exponent(1.0f);
		reinterpret_cast<Object*>(m_Spot_Light)->Set_is_Static(true);

		m_Spot_Light->Light_On();

	}

	virtual void Draw()
	{
		glPushMatrix(); // ·Îº¿ ±×¸®±â ½ÃÀÛ

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);

		m_Spot_Light->Draw();

		glShadeModel(GL_SMOOTH);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
		glMateriali(GL_FRONT, GL_SHININESS, 20);

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

		glPopMatrix(); // ·Îº¿ ±×¸®±â ³¡
	}

	void Animation()
	{
		for (int i = 0; i < 2; ++i)
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

	/*
	void Jump() { if (!m_is_Jumped) { m_is_Jumped = true; m_Direction.y = 3.0f; } }

	void Jump_Check()
	{
		if (!m_is_Jumped_Max_Point)
		{
			if (m_Position.y >= OBJECT_SIZE * m_Scale_Y * 3.0f)
			{ 
				m_Position.y = OBJECT_SIZE * 3.0f;
				m_Direction.y = -1.5f; 
				m_is_Jumped_Max_Point = true; 
			}
			else if (m_Position.y >= OBJECT_SIZE * 1.0f) m_Direction.y = 1.5f; 
		}
		else
		{
			if (m_Position.y <= OBJECT_SIZE * m_Scale_Y * 1.5f)
			{
				m_Position.y = OBJECT_SIZE * m_Scale_Y * 1.5f;
				m_Direction.y = 0.0f;
				m_is_Jumped_Max_Point = false;
				m_is_Jumped = false;
			}
			else if (m_Position.y <= OBJECT_SIZE * 1.0f) m_Direction.y = -3.0f;
		}
	}
	*/

	virtual void Set_Move_to_Direction(const int& dir) 
	{ 
		switch (dir)
		{
		case 0: m_Forward_Dir_Value = -1.0f; break;
		case 1: m_Backward_Dir_Value = 1.0f; break;
		case 2: m_Left_Dir_Value = -1.0f; break;
		case 3: m_Right_Dir_Value = 1.0f; break;
		case 4: m_Forward_Dir_Value = 0.0f; break;
		case 5: m_Backward_Dir_Value = 0.0f; break;
		case 6: m_Left_Dir_Value = 0.0f; break;
		case 7: m_Right_Dir_Value = 0.0f; break;
		}
	}

	virtual void Auto_Move()
	{
		if (m_Forward_Dir_Value != 0.0f || m_Backward_Dir_Value != 0.0f ||
			m_Left_Dir_Value != 0.0f || m_Right_Dir_Value != 0.0f)
		{

			m_Prev_Position = m_Position;
			m_Position.z += m_Position.z + m_Forward_Dir_Value * m_Speed <= 200.0f && m_Position.z + m_Forward_Dir_Value * m_Speed >= -200.0f ? m_Forward_Dir_Value * m_Speed : 0.0f;
			m_Position.z += m_Position.z + m_Backward_Dir_Value * m_Speed <= 200.0f && m_Position.z + m_Backward_Dir_Value * m_Speed >= -200.0f ? m_Backward_Dir_Value * m_Speed : 0.0f;
			m_Position.x += m_Position.x + m_Left_Dir_Value * m_Speed <= 200.0f && m_Position.x + m_Left_Dir_Value * m_Speed >= -200.0f ? m_Left_Dir_Value * m_Speed : 0.0f;
			m_Position.x += m_Position.x + m_Right_Dir_Value * m_Speed <= 200.0f && m_Position.x + m_Right_Dir_Value * m_Speed >= -200.0f ? m_Right_Dir_Value * m_Speed : 0.0f;

			if (m_Forward_Dir_Value + m_Backward_Dir_Value <= -1.0f)
			{
				Yaw(-m_Yaw_Rotate_Stack);
				if (m_Left_Dir_Value + m_Right_Dir_Value <= -1.0f) Yaw(135.0f);
				else if (m_Left_Dir_Value + m_Right_Dir_Value >= 1.0f) Yaw(225.0f);
				else Yaw(180.0f);
			}
			else if (m_Forward_Dir_Value + m_Backward_Dir_Value >= 1.0f)
			{
				Yaw(-m_Yaw_Rotate_Stack);
				if (m_Left_Dir_Value + m_Right_Dir_Value <= -1.0f) Yaw(45.0f);
				else if (m_Left_Dir_Value + m_Right_Dir_Value >= 1.0f) Yaw(315.0f);
			}
			else if (m_Left_Dir_Value + m_Right_Dir_Value <= -1.0f)
			{
				Yaw(-m_Yaw_Rotate_Stack); Yaw(90.0f);
			}
			else if (m_Left_Dir_Value + m_Right_Dir_Value >= 1.0f)
			{
				Yaw(-m_Yaw_Rotate_Stack); Yaw(270.0f);
			}
			
			Animation();
			m_OBB->Set_Center_Position(m_Position);
		}
	}

	virtual bool Collision_Check(Object* other)
	{
		if (!m_is_Static_Object && !other->Get_is_Static() && other->Get_OBB() != nullptr)
		{
			if (m_OBB->OBB_Intersection(other->Get_OBB()))
			{
				Collision_Procedure(OBJECT_COLLIDE_PROCEDURE, other);
				return true;
			}
		}
		return false;
	}

	virtual void Collision_Procedure(const int& collision_type, Object* other)
	{
		switch (collision_type)
		{
		case OBJECT_COLLIDE_PROCEDURE:
			m_Position.x = m_Prev_Position.x; m_Position.z = m_Prev_Position.z;
			break;
		}
	}

	virtual void Update()
	{
		Auto_Move();
	}
};

