#pragma once
#include "Object.h"

class Bullet : Object
{
private:
	bool m_is_Shot = false;
	bool m_is_Collide = false;
	bool m_is_Collide_Prod_End = false;
	float m_Speed = 10.0f;
	queue<Bullet*>* m_Pool_pointer = nullptr;
	Object* m_Whose_Bullet_is_it = nullptr;

public:
	void Set_is_Shot(const bool& b) { m_is_Shot = b; }
	bool const Get_is_Shot() { return m_is_Shot; }
	void Set_is_Collide_Prod_End(const bool& b) { m_is_Collide_Prod_End = b; }
	bool const Get_is_Collide_Prod_End() { return m_is_Collide_Prod_End; }

	void Set_Pool_pointer(queue<Bullet*>* pool) { m_Pool_pointer = pool; }

public:
	Bullet() : Object() { Bullet_Init(); Make_OBB(); }
	~Bullet() {}



	void Bullet_Init()
	{
		m_Object_Type = BULLET_OBJECT;
		m_Whose_Bullet_is_it = nullptr;
		m_is_Shot = false;
		m_is_Collide = false;
		m_is_Collide_Prod_End = true;
		m_Position.x = 1000.0f; m_Position.y = 1000.0f; m_Position.z = 1000.0f;
		m_Scale_X = 0.1f; m_Scale_Y = 0.1f; m_Scale_Z = 0.1f;
		m_Forward_Vector[0] = 0.0f; m_Forward_Vector[1] = 0.0f; m_Forward_Vector[2] = 0.0f;  m_Forward_Vector[3] = 1.0f;
		if (m_OBB != nullptr) m_OBB->Set_Center_Position(m_Position);
	}

	void Bullet_Load(Object* who)
	{
		m_Whose_Bullet_is_it = who;
		m_Position = who->Get_Position();
		m_Forward_Vector[0] = who->Get_Forward_Vector_X(); m_Forward_Vector[1] = who->Get_Forward_Vector_Y(); m_Forward_Vector[2] = who->Get_Forward_Vector_Z();
		m_is_Shot = true;
		m_is_Collide = false;
		m_is_Collide_Prod_End = false;
	}

	virtual void Draw()
	{
		glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);
		glutSolidSphere(OBJECT_SIZE * 0.3f, 8, 8);
		glPopMatrix();
	}

	virtual void Update()
	{
		Move();
		Border_Check();
		Particle_Animate();
		Reload();
	}

	virtual void Move()
	{
		if (m_is_Shot && !m_is_Collide)
		{
			m_Position.x += m_Forward_Vector[0] * m_Speed;
			m_Position.y += m_Forward_Vector[1] * m_Speed;
			m_Position.z += m_Forward_Vector[2] * m_Speed;
			if (m_OBB != nullptr) m_OBB->Set_Center_Position(m_Position);
		}
	}

	void Border_Check()
	{
		if (m_OBB->Get_Center_Position()[0] - m_OBB->Get_OBB_Axis_Distance()[0] < -FLOOR_PIECE_WIDTH_SIZE * FLOOR_WIDTH
			|| m_OBB->Get_Center_Position()[0] + m_OBB->Get_OBB_Axis_Distance()[0] > FLOOR_PIECE_WIDTH_SIZE * FLOOR_WIDTH
			|| m_OBB->Get_Center_Position()[2] - m_OBB->Get_OBB_Axis_Distance()[2] < -FLOOR_PIECE_HEIGHT_SIZE * FLOOR_HEIGHT
			|| m_OBB->Get_Center_Position()[2] + m_OBB->Get_OBB_Axis_Distance()[2] > FLOOR_PIECE_HEIGHT_SIZE * FLOOR_HEIGHT)
		{
			m_is_Collide = true;
			m_is_Collide_Prod_End = true;
		}
	}

	virtual bool Collision_Check(Object* other)
	{
		if (m_Whose_Bullet_is_it != nullptr)
		{
			if (m_Whose_Bullet_is_it != other && other->Get_Object_Type() != BULLET_OBJECT)
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
		}

		return false;
	}

	virtual void Collision_Procedure(const int& collision_type, Object* other)
	{
		switch (collision_type)
		{
		case OBJECT_COLLIDE_PROCEDURE:
			m_is_Collide = true;
			m_is_Collide_Prod_End = false;
			if (m_Whose_Bullet_is_it != other && other->Get_Object_Type() == ROBOT_OBJECT)
				other->HP_Reduce();
			break;
		}
	}

	void Particle_Animate()
	{
		if (m_is_Collide)
		{
			if (!m_is_Collide_Prod_End)
			{
				m_is_Collide_Prod_End = true;
			}
		}
	}

	void Reload()
	{
		if (m_is_Collide && m_is_Collide_Prod_End)
		{
			Bullet_Init();
			m_Pool_pointer->push(this);
		}
	}
};