#pragma once
#include "Object.h"
#include "Camera.h"
#include "Roller_Coaster.h"

#define EDGE_COLLIDE_PROCEDURE 0
#define OBJECT_COLLIDE_PROCEDURE 1

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
	float m_Lower_Leg_Rot_Value[2]{ -80.0f, 10.0f };

	bool m_is_Jumped = false;
	bool m_is_Jumped_Max_Point = false;

	Light_Object* m_Spot_Light = nullptr;

	float m_Cloak_Plane_CPs[5][5][3]{ 0.0f, };
	float m_Cloak_Flapping_Speed[5];

	Camera* m_Camera = nullptr;

	bool m_is_Playable = false;
	bool m_have_Friend_Robot = false;

	bool m_is_Shootable = true;
	bool m_is_Running_Shooting_Cool_Time = false;
	clock_t m_Start_Shooting_Cool_Time;
	clock_t m_Curr_Shooting_Cool_Time;

	

public:
	virtual void Yaw(const float& value) 
	{
		m_Rotate_Yaw_Value = value; Rotate_Check(); 
		m_Yaw_Rotate_Stack += value;
		if (m_Camera != nullptr) m_Camera->Yaw(value);
	}
	float const Get_Yaw_Stack() { return m_Yaw_Rotate_Stack ; }
	
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
		m_Yaw_Rotate_Stack = 180.0f;

		m_Head_Position[0] = m_Position.x;
		m_Head_Position[1] = m_Position.y;
		m_Head_Position[2] = m_Position.z;
		m_Head_Position[3] = 1.0f;
		m_Head_Position[1] += OBJECT_SIZE * m_Scale_Y * 0.7f;
	}
	
	void Set_is_Playable(const bool& b) { m_is_Playable = b; }
	bool const Get_is_Playable() { return m_is_Playable; }

	void Set_have_Friend(const bool& b) { m_have_Friend_Robot = b; }
	bool const Get_have_Friend() { return m_have_Friend_Robot; }

	bool const Get_is_Shootable() { return m_is_Shootable; }

	virtual void HP_Reduce() 
	{
		m_HP -= 1;
		if (m_HP <= 0) m_is_Destroyed = true;
	}

public:
	Robot() : Object() { Robot_Initialize(); };
	~Robot() 
	{ 
		if (m_Spot_Light != nullptr) { delete m_Spot_Light; m_Spot_Light = nullptr; }
		if (m_Camera != nullptr) { m_Camera = nullptr; }
	};

	void Robot_Initialize()
	{
		m_Object_Type = ROBOT_OBJECT;
		Set_is_Static(false);

		m_HP = 3;
		m_Speed = 3.0f;

		m_Scale_X = 0.3f; m_Scale_Y = 0.5f; m_Scale_Z = 0.3f;

		m_Position.x = 0.0f;
		m_Position.y = -OBJECT_SIZE * m_Scale_Y * 0.5f;
		m_Position.z = OBJECT_SIZE * 5.0f;
		Init_Head();

		m_Color.R = 1.0f; m_Color.G = 0.0f; m_Color.B = 0.0f; m_Color.A = 1.0f;
		m_Material.Ambient[0] = 0.1f; m_Material.Ambient[1] = 0.2f; m_Material.Ambient[2] = 0.1f; m_Material.Ambient[3] = 1.0f;
		m_Material.Diffuse[0] = 0.5f; m_Material.Diffuse[1] = 1.0f; m_Material.Diffuse[2] = 0.5f; m_Material.Diffuse[3] = 1.0f;
		m_Material.Specular[0] = 1.0f; m_Material.Specular[1] = 1.0f; m_Material.Specular[2] = 1.0f; m_Material.Specular[3] = 1.0f;
		
		Make_OBB();
		m_OBB->OBB_Init(m_Position, m_Scale_X, m_Scale_Y, m_Scale_Z);

		Init_Clock_CPs();
		random_device random_Device;
		mt19937_64 random_SEED(random_Device());
		uniform_real_distribution<> random_Speed(-3.0f, 3.0f);

		for (int i = 0; i < 5; ++i) m_Cloak_Flapping_Speed[i] = random_Speed(random_SEED);
	}

	void Spot_Light_Setting()
	{
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

		if (m_Spot_Light != nullptr) m_Spot_Light->Draw();

		glShadeModel(GL_SMOOTH);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
		glMateriali(GL_FRONT, GL_SHININESS, 20);

		Draw_Robot();
		if (m_have_Friend_Robot)
		{
			glTranslatef(OBJECT_SIZE * m_Scale_X * 2.0f, 0.0f, 0.0f);
			Draw_Robot();
		}
		glPopMatrix(); // ·Îº¿ ±×¸®±â ³¡
		
		/* // OBB ±×¸®±â
		glPushMatrix();
		glTranslatef(m_OBB->Get_Center_Position()[0], m_OBB->Get_Center_Position()[1], m_OBB->Get_Center_Position()[2]);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_OBB->Get_OBB_Axis_Distance()[0] / (OBJECT_SIZE * 0.5f), m_OBB->Get_OBB_Axis_Distance()[1] / (OBJECT_SIZE * 0.5f), m_OBB->Get_OBB_Axis_Distance()[2] / (OBJECT_SIZE * 0.5f));
		glutWireCube(OBJECT_SIZE);
		glPopMatrix();
		*/
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

			{ // ·Îº¿ ¸ÁÅä ±×¸®±â
				glPushMatrix();
				glTranslatef(0.0f, -OBJECT_SIZE * 0.5f * m_Scale_Y * 0.25f, 0.0f);

				glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, 3, 5, 0.0f, 1.0f, 15, 5, &m_Cloak_Plane_CPs[0][0][0]);
				glEnable(GL_MAP2_VERTEX_3);
				glMapGrid2f(10, 0.0f, 1.0f, 10, 0.0f, 1.0f);
				glEvalMesh2(GL_FILL, 0, 10, 0, 10);
				glDisable(GL_MAP2_VERTEX_3);
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

	
	virtual void Move()
	{
		if (m_is_Move)
		{
			m_Prev_Position = m_Position;
			m_Head_Prev_Position.x = m_Head_Position[0]; m_Head_Prev_Position.y = m_Head_Position[1]; m_Head_Prev_Position.z = m_Head_Position[2];

			if (m_is_Forward_Move)
			{
				m_Position.x += m_Forward_Vector[0] * m_Speed; m_Position.y += m_Forward_Vector[1] * m_Speed; m_Position.z += m_Forward_Vector[2] * m_Speed;
			}
			if (m_is_Backward_Move)
			{
				m_Position.x -= m_Forward_Vector[0] * m_Speed; m_Position.y -= m_Forward_Vector[1] * m_Speed; m_Position.z -= m_Forward_Vector[2] * m_Speed;
			}
			if (m_is_Left_Move)
			{
				m_Position.x -= m_Right_Vector[0] * m_Speed; m_Position.y -= m_Right_Vector[1] * m_Speed; m_Position.z -= m_Right_Vector[2] * m_Speed;
			}
			if (m_is_Right_Move)
			{
				m_Position.x += m_Right_Vector[0] * m_Speed; m_Position.y += m_Right_Vector[1] * m_Speed; m_Position.z += m_Right_Vector[2] * m_Speed;
			}

			Init_Head();

			Animation();
			if (m_OBB != nullptr) m_OBB->Set_Center_Position(m_Position);
		}
	}

	virtual bool Collision_Check(Object* other)
	{
		if (other->Get_Object_Type() != BULLET_OBJECT)
		{
			if (!m_is_Static_Object && !other->Get_is_Static() && other->Get_OBB() != nullptr)
			{
				if (m_OBB->OBB_Intersection(other->Get_OBB()))
				{
					Collision_Procedure(OBJECT_COLLIDE_PROCEDURE, other);
					return true;
				}
			}
			else if (!m_is_Static_Object && !other->Get_is_Static() && other->Get_Object_Type() == ROLLER_COASTER)
			{
				Roller_Coaster* temp_RC = reinterpret_cast<Roller_Coaster*>(other);
				if (temp_RC->Get_is_CP_Making_Over())
				{
					for (auto& obb : temp_RC->Get_OBBs())
					{
						if (m_OBB->OBB_Intersection(obb))
						{
							Collision_Procedure(OBJECT_COLLIDE_PROCEDURE, other);
							return true;
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
			
			if (other->Get_Object_Type() == ROBOT_OBJECT)
			{
				if (m_is_Playable)
				{
					m_have_Friend_Robot = true;
					other->Set_is_Destroyed(true);
				}
				else
				{
					reinterpret_cast<Robot*>(other)->Set_have_Friend(true);
					Set_is_Destroyed(true);
				}
			}

			if (!m_is_Playable) Yaw(90.0f);
			break;
		}
	}

	void Border_Check()
	{
		if (m_OBB->Get_Center_Position()[0] - m_OBB->Get_OBB_Axis_Distance()[0] < -FLOOR_PIECE_WIDTH_SIZE * FLOOR_WIDTH
			|| m_OBB->Get_Center_Position()[0] + m_OBB->Get_OBB_Axis_Distance()[0] > FLOOR_PIECE_WIDTH_SIZE * FLOOR_WIDTH
			|| m_OBB->Get_Center_Position()[2] - m_OBB->Get_OBB_Axis_Distance()[2] < -FLOOR_PIECE_HEIGHT_SIZE * FLOOR_HEIGHT
			|| m_OBB->Get_Center_Position()[2] + m_OBB->Get_OBB_Axis_Distance()[2] > FLOOR_PIECE_HEIGHT_SIZE * FLOOR_HEIGHT)
		{
			m_Position.x = m_Prev_Position.x; m_Position.z = m_Prev_Position.z;
			m_Head_Position[0] = m_Head_Prev_Position.x; m_Head_Position[1] = m_Head_Prev_Position.y; m_Head_Position[2] = m_Head_Prev_Position.z;
			if (m_OBB != nullptr) m_OBB->Set_Center_Position(m_Position);
			
			if (!m_is_Playable) Yaw(90.0f);
		}
	}

	virtual void Update()
	{
		Move();
		Border_Check();
		Clock_Flapping();
		Shooting_Cool_Time_Check();
	}

	void Init_Clock_CPs()
	{
		// ÃÊ±âÈ­
		for (int i = -2; i <= 2; ++i)
		{
			for (int j = 0; j <= 4; ++j)
			{
				m_Cloak_Plane_CPs[i+2][j][0] = i * OBJECT_SIZE * m_Scale_X*0.5f;
				m_Cloak_Plane_CPs[i+2][j][1] = 0.0f;
				m_Cloak_Plane_CPs[i+2][j][2] = -j * OBJECT_SIZE * m_Scale_Z*0.3f;
			}
		}

		// ¾î±ú Á¶Á¤
		m_Cloak_Plane_CPs[0][0][0] += 0.5f * OBJECT_SIZE * m_Scale_X;
		m_Cloak_Plane_CPs[0][0][1] += -0.05f * OBJECT_SIZE * m_Scale_Y;
		m_Cloak_Plane_CPs[4][0][0] += -0.5f * OBJECT_SIZE * m_Scale_X;
		m_Cloak_Plane_CPs[4][0][1] += -0.05f * OBJECT_SIZE * m_Scale_Y;

		// ¸ÁÅä Æø Á¶Á¤
		for (int i = -2; i <= 2; ++i)
		{
			for (int j = 1; j < 4; ++j) m_Cloak_Plane_CPs[i + 2][j][0] += -0.06f * i * j * OBJECT_SIZE * m_Scale_X;
		}

		// Áß¹ÝºÎ±îÁö ÈÖ¾îÁö°Ô
		for (int i = 0; i < 5; ++i)
		{
			m_Cloak_Plane_CPs[i][1][1] += 0.15f * OBJECT_SIZE * m_Scale_Y;
			m_Cloak_Plane_CPs[i][2][1] += 0.2f * OBJECT_SIZE * m_Scale_Y;
		}

		// ÈÄ¹ÝºÎ´Â ¶Ò ¶³¾îÁö°Ô
		for (int i = 0; i < 5; ++i)
		{
			m_Cloak_Plane_CPs[i][3][1] = -0.5f * OBJECT_SIZE * m_Scale_Y;
			m_Cloak_Plane_CPs[i][4][1] = -1.0f * OBJECT_SIZE * m_Scale_Y;
		}

	}

	void Clock_Flapping()
	{
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 3; j < 5; ++j)
			{
				m_Cloak_Plane_CPs[i][j][1] += OBJECT_SIZE * m_Scale_Y * 0.1f * m_Cloak_Flapping_Speed[j];
				m_Cloak_Plane_CPs[i][j][2] += OBJECT_SIZE * m_Scale_Z * 0.1f * -m_Cloak_Flapping_Speed[j];
				if (m_Cloak_Plane_CPs[i][j][1] >= OBJECT_SIZE * m_Scale_Y * 1.0f
					|| m_Cloak_Plane_CPs[i][j][1] <= -OBJECT_SIZE * m_Scale_Y * 1.0f)
				{
					m_Cloak_Flapping_Speed[j] *= -1; 
					m_Cloak_Plane_CPs[i][j][1] += OBJECT_SIZE * m_Scale_Y * 0.1f * m_Cloak_Flapping_Speed[j];
				}
			}
		}
	}

	void Shoot()
	{
		m_is_Shootable = false;
		Start_Cool_Time(m_Start_Shooting_Cool_Time);
		m_is_Running_Shooting_Cool_Time = true;
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
};

