#pragma once
#include "Object.h"
#include "Camera.h"

class Robot : Object
{
private:
	Vector3 m_Prev_Position;
	Vector3 m_Head_Prev_Position;
	float m_Speed;

	float m_Upper_Arm_Rot_Value[2]{ -60.0f, 40.0f };
	int m_Upper_Arm_Rot_Sign[2]{ 1, -1 };

	float m_Upper_Leg_Rot_Value[2]{ -80.0f, 40.0f };
	int m_Upper_Leg_Rot_Sign[2]{ 1, -1 };
	float m_Lower_Leg_Rot_Value[2]{ 80.0f, 10.0f };

	Camera* m_Camera = nullptr;

	bool m_is_Shootable = true;
	bool m_is_Running_Shooting_Cool_Time = false;
	clock_t m_Start_Shooting_Cool_Time;
	clock_t m_Curr_Shooting_Cool_Time;

	Quaternion* m_Up_Quat = nullptr;
	float m_Pitch_Stack = 0.0f;

	bool m_Have_Snow_Ball = false;
	bool m_is_Reloading = false;

	bool m_is_Dash = false;

	Object* m_Snow_Ball = nullptr;

public:
	
	void Set_Pitch_Stack(const float& v) { m_Pitch_Stack = v; }
	float const Get_Pitch_Stack() { return m_Pitch_Stack; }

	virtual void Yaw(const float& value) 
	{
		m_Rotate_Yaw_Value = value;
		m_Quaternion_temp_2->Set_By_Angle_Axis(value, 0.0f, 1.0f, 0.0f);
		m_Quaternion_Result->Multiple(m_Quaternion_temp_2);

		float temp_Transform[16] = { 0.0f, };
		m_Quaternion_temp_2->Get_Transform_Matrix(temp_Transform);
		Multiple_Matrix_And_Vector(temp_Transform, m_Forward_Vector);
		Multiple_Matrix_And_Vector(temp_Transform, m_Right_Vector);

		if (m_OBB != nullptr)
		{
			for (int i = 0; i < 16; ++i) temp_Transform[i] = 0.0f;
			m_Quaternion_temp_2->Get_Transform_Matrix(temp_Transform);
			m_OBB->Multiply_Rotate_Matrix_to_OBB_Axis(temp_Transform);
			Set_Pos_to_Collider_Pos();
		}

		m_Quaternion_Result->Get_Transform_Matrix(m_Rotate_Transform_Matrix);
	}
	
	virtual void Pitch(const float& value)
	{
		if (m_Camera != nullptr) 
		{
			if (!m_Camera->Get_is_FPS())
			{
				if (m_Pitch_Stack + value < 90.0f && m_Pitch_Stack + value > 0.0f)
				{
					m_Pitch_Stack += value;
					m_Up_Quat->Set_By_Angle_Axis(value, 1.0f, 0.0f, 0.0f);

					float temp_Transform[16] = { 0.0f, };
					m_Up_Quat->Get_Transform_Matrix(temp_Transform);
					Multiple_Matrix_And_Vector(temp_Transform, m_Up_Vector);
				}
			}
			else
			{
				if (m_Pitch_Stack + value < 120.0f && m_Pitch_Stack + value > 0.0f)
				{
					m_Pitch_Stack += value;
					m_Up_Quat->Set_By_Angle_Axis(value, 1.0f, 0.0f, 0.0f);

					float temp_Transform[16] = { 0.0f, };
					m_Up_Quat->Get_Transform_Matrix(temp_Transform);
					Multiple_Matrix_And_Vector(temp_Transform, m_Up_Vector);
				}
			}
		}
	}

	void Pitch_Correction()
	{
		if (m_Camera != nullptr)
		{
			if (!m_Camera->Get_is_FPS())
			{
				if (m_Pitch_Stack > 90.0f) Pitch(89.0f - m_Pitch_Stack);
				else if (m_Pitch_Stack < 0.0f) Pitch(1.0f -m_Pitch_Stack);
			}

			else
			{
				if (m_Pitch_Stack > 120.0f) Pitch(119.0f - m_Pitch_Stack);
				else if (m_Pitch_Stack < 0.0f) Pitch(1.0f -m_Pitch_Stack);
			}
		}
	}

	void Set_Camera(Camera* c)
	{
		m_Camera = c;
		m_Camera->Set_Target(this);
	}

	void Release_Camera()
	{
		if (m_Camera != nullptr)
		{
			m_Camera->Release_Target();
			m_Camera = nullptr;
		}
	}

	virtual void Init_Head()
	{
		m_Head_Position[0] = m_Position.x;
		m_Head_Position[1] = m_Position.y;
		m_Head_Position[2] = m_Position.z;
		m_Head_Position[3] = 1.0f;
		m_Head_Position[1] += OBJECT_SIZE * m_Scale_Y * 0.7f;
	}

	bool const Get_is_Shootable() { return m_is_Shootable; }

	virtual void HP_Reduce() { m_HP -= 1; }
	virtual int Get_HP() { return m_HP; }

	void Init_Prev_Pos() { m_Prev_Position = m_Position; }

	bool const Get_Have_Snow_Ball() { return m_Have_Snow_Ball; }
	bool const Get_is_Reloading() { return m_is_Reloading; }

	void Dash_Mode_Activate()
	{
		if (!m_Have_Snow_Ball && !m_is_Reloading && m_is_Forward_Move  && !m_is_Backward_Move) m_is_Dash = true;
		else m_is_Dash = false;
	}

	void Dash_Mode_Off() { m_is_Dash = false; }

	void Set_Snow_Ball(Object* sb) { m_Snow_Ball = sb; }
	Object* Get_Snow_Ball() { return m_Snow_Ball; }

public:
	Robot() : Object() {};
	~Robot() 
	{ 
		if (m_Camera != nullptr) { m_Camera = nullptr; }
		if (m_Up_Quat != nullptr) { delete m_Up_Quat; }
	};

	void Robot_Initialize()
	{
		m_Object_Type = ROBOT_OBJECT;
		Set_is_Static(false);

		m_HP = PLAYER_MAX_HP;
		m_Speed = 3.0f;

		m_Scale_X = 0.3f; m_Scale_Y = 0.5f; m_Scale_Z = 0.3f;

		m_Position.y = -OBJECT_SIZE * m_Scale_Y * 0.5f;
		Init_Head();

		m_Color.R = 1.0f; m_Color.G = 0.0f; m_Color.B = 0.0f; m_Color.A = 1.0f;
		m_Material.Ambient[0] = 0.3f; m_Material.Ambient[1] = 0.2f; m_Material.Ambient[2] = 0.2f; m_Material.Ambient[3] = 1.0f;
		m_Material.Diffuse[0] = 0.3f; m_Material.Diffuse[1] = 0.3f; m_Material.Diffuse[2] = 0.8f; m_Material.Diffuse[3] = 1.0f;
		m_Material.Specular[0] = 0.8f; m_Material.Specular[1] = 0.8f; m_Material.Specular[2] = 0.8f; m_Material.Specular[3] = 1.0f;
		
		Make_OBB();
		m_OBB->OBB_Init(m_Position, m_Scale_X, m_Scale_Y, m_Scale_Z);

		m_Up_Quat = new Quaternion();
	}

	virtual void Draw()
	{
		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
		glMateriali(GL_FRONT, GL_SHININESS, 40);

		Draw_Robot();
		
		glPopMatrix();
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
		float speed = m_Speed;
		if (m_is_Dash) speed *= DASH_MODE_SPEED_RELATIVE_VALUE;

		for (int i = 0; i < 2; ++i)
		{
			if (!m_Have_Snow_Ball)
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
				m_Upper_Arm_Rot_Value[i] += 6.0f * m_Upper_Arm_Rot_Sign[i] * speed;
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
			m_Upper_Leg_Rot_Value[i] += 6.0f * m_Upper_Leg_Rot_Sign[i] * speed;


			if (m_Upper_Leg_Rot_Value[i] <= 0.0f) m_Lower_Leg_Rot_Value[i] = -m_Upper_Leg_Rot_Value[i];
			else m_Lower_Leg_Rot_Value[i] = 10.0f;
		}

		
	}

	virtual void Update()
	{
		Move();
	}
	
	virtual void Move()
	{
		if (m_is_Move)
		{
			float speed = m_Speed;
			if (m_is_Dash) speed *= DASH_MODE_SPEED_RELATIVE_VALUE;

			m_Prev_Position = m_Position;
			m_Head_Prev_Position.x = m_Head_Position[0]; m_Head_Prev_Position.y = m_Head_Position[1]; m_Head_Prev_Position.z = m_Head_Position[2];

			if (m_is_Forward_Move)
			{
				m_Position.x += m_Forward_Vector[0] * speed; m_Position.y += m_Forward_Vector[1] * speed; m_Position.z += m_Forward_Vector[2] * speed;
			}
			if (m_is_Backward_Move)
			{
				m_Position.x -= m_Forward_Vector[0] * speed; m_Position.y -= m_Forward_Vector[1] * speed; m_Position.z -= m_Forward_Vector[2] * speed;
			}
			if (m_is_Left_Move)
			{
				m_Position.x -= m_Right_Vector[0] * speed; m_Position.y -= m_Right_Vector[1] * speed; m_Position.z -= m_Right_Vector[2] * speed;
			}
			if (m_is_Right_Move)
			{
				m_Position.x += m_Right_Vector[0] * speed; m_Position.y += m_Right_Vector[1] * speed; m_Position.z += m_Right_Vector[2] * speed;
			}

			Init_Head();

			Animation();
			if (m_OBB != nullptr) m_OBB->Set_Center_Position(m_Position);
		}
	}

	virtual bool Collision_Check(Object* other)
	{
		if (m_Snow_Ball != other && 
			(other->Get_Object_Type() != SNOW_BALL_OBJECT && other->Get_Object_Type() != SMALL_ENEMY_1
				&& other->Get_Object_Type() != SMALL_ENEMY_2))
		{
			if (!m_is_Static_Object && !other->Get_is_Static())
			{
				if (other->Get_OBB() != nullptr)
				{
					if (m_OBB->OBB_Intersection(other->Get_OBB()))
					{
						Collision_Procedure(OBJECT_COLLIDE_PROCEDURE, other);
						return true;
					}
				}
				else if (other->Get_Object_Type() == FENCE_OBJECT)
				{
					for (int i = 0; i < 4; ++i)
					{
						if (other->Get_OBBs()[i] != nullptr)
						{
							if (m_OBB->OBB_Intersection(other->Get_OBBs()[i]))
							{
								Collision_Procedure(OBJECT_COLLIDE_PROCEDURE, other);
								return true;
							}
						}
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
			m_Position.x = m_Prev_Position.x; m_Position.z = m_Prev_Position.z;
			m_Head_Position[0] = m_Head_Prev_Position.x; m_Head_Position[1] = m_Head_Prev_Position.y; m_Head_Position[2] = m_Head_Prev_Position.z;
			if (m_OBB != nullptr) m_OBB->Set_Center_Position(m_Position);

			break;
		}
	}

	void Shoot_Process()
	{
		if (m_Have_Snow_Ball)
		{
			m_Have_Snow_Ball = false;
			m_Snow_Ball = nullptr;
			m_Upper_Arm_Rot_Value[0] = -60.0f; m_Upper_Arm_Rot_Value[1] = 40.0f;
			m_Upper_Arm_Rot_Sign[0] = 1; m_Upper_Arm_Rot_Sign[1] = -1;
		};
	}

	void Shooting_Cool_Time_Check()
	{
		if (Cool_Time_Check(m_Curr_Shooting_Cool_Time, m_Start_Shooting_Cool_Time, 200))
		{
			m_is_Running_Shooting_Cool_Time = false;
			m_is_Shootable = true;
		}
	}

	void Start_Cool_Time(clock_t& start_cool_time) { start_cool_time = clock(); }

	bool const Cool_Time_Check(clock_t& curr_cool_time, const clock_t& start_cool_time, const long& msec)
	{
		if (m_is_Running_Shooting_Cool_Time)
		{
			curr_cool_time = clock();
			if (curr_cool_time - start_cool_time >= msec) return true;
			return false;
		}
		else return false;
	}

	void Reload()
	{
		if (!m_is_Reloading && !m_Have_Snow_Ball)
		{
			m_is_Forward_Move = false;
			m_is_Backward_Move = false;
			m_is_Left_Move = false;
			m_is_Right_Move = false;
			m_is_Reloading = true;
		}
	}

	void Make_Snow_Ball()
	{
		if (m_is_Reloading)
		{
			m_is_Reloading = false;
			m_Have_Snow_Ball = true;
			m_Upper_Arm_Rot_Value[0] = -60.0f; m_Upper_Arm_Rot_Value[1] = -60.0f;
		}
	}

	

};

