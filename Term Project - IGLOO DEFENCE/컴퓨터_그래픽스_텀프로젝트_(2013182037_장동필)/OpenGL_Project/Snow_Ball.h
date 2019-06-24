#pragma once
#include "Object.h"

class Snow_Ball : Object
{
private:
	bool m_is_Shot = false;
	bool m_is_Collide = false;
	bool m_is_Collide_Prod_End = false;
	float m_Speed = 20.0f;
	queue<Snow_Ball*>* m_Pool_pointer = nullptr;
	Object* m_Whose_Ball_is_it = nullptr;

	float m_Pitch_Value_Stack = 0.0f;

public:
	void Set_is_Shot(const bool& b) { m_is_Shot = b; }
	bool const Get_is_Shot() { return m_is_Shot; }
	void Set_is_Collide_Prod_End(const bool& b) { m_is_Collide_Prod_End = b; }
	bool const Get_is_Collide_Prod_End() { return m_is_Collide_Prod_End; }

	void Set_Pool_pointer(queue<Snow_Ball*>* pool) { m_Pool_pointer = pool; }

public:
	Snow_Ball() : Object() { Snow_Ball_Init(); Make_OBB(); }
	~Snow_Ball() {}



	void Snow_Ball_Init()
	{
		m_Object_Type = SNOW_BALL_OBJECT;
		m_Whose_Ball_is_it = nullptr;
		m_is_Shot = false;
		m_is_Collide = false;
		m_is_Collide_Prod_End = true;
		m_Position.x = 1000.0f; m_Position.y = 1000.0f; m_Position.z = 1000.0f;
		m_Scale_X = 1.0f; m_Scale_Y = 1.0f; m_Scale_Z = 1.0f;
		m_Forward_Vector[0] = 0.0f; m_Forward_Vector[1] = 0.0f; m_Forward_Vector[2] = 1.0f;  m_Forward_Vector[3] = 1.0f;
		
		m_Material.Ambient[0] = 1.0f; m_Material.Ambient[1] = 1.0f; m_Material.Ambient[2] = 1.0f; m_Material.Ambient[3] = 1.0f;
		m_Material.Diffuse[0] = 1.0f; m_Material.Diffuse[1] = 1.0f; m_Material.Diffuse[2] = 1.0f; m_Material.Diffuse[3] = 1.0f;
		m_Material.Specular[0] = 1.0f; m_Material.Specular[1] = 1.0f; m_Material.Specular[2] = 1.0f; m_Material.Specular[3] = 1.0f;

		m_Pitch_Value_Stack = 0.0f;

		for (int i = 0; i < 16; ++i) m_Rotate_Transform_Matrix[i] = 0;
		m_Rotate_Transform_Matrix[0] = 1.0f; m_Rotate_Transform_Matrix[5] = 1.0f; m_Rotate_Transform_Matrix[10] = 1.0f; m_Rotate_Transform_Matrix[15] = 1.0f;

		if (m_OBB != nullptr) m_OBB->OBB_Init(m_Position, m_Scale_X * 0.25f, m_Scale_Y * 0.25f, m_Scale_Z * 0.25f);
	}

	void Snow_Ball_Load(Object* who)
	{
		m_Whose_Ball_is_it = who;
		
		m_Position = who->Get_Position();
		
		m_Forward_Vector[0] = who->Get_Forward_Vector_X(); m_Forward_Vector[1] = who->Get_Forward_Vector_Y(); m_Forward_Vector[2] = who->Get_Forward_Vector_Z();
		
		m_Position.x += m_Forward_Vector[0] * m_Scale_X * OBJECT_SIZE * 0.5f;
		m_Position.y += 10.0f + m_Forward_Vector[1] * m_Scale_Y * OBJECT_SIZE * 0.5f;
		m_Position.z += m_Forward_Vector[2] * m_Scale_Z * OBJECT_SIZE * 0.5f;

		for (int i = 0; i < 16; ++i) m_Rotate_Transform_Matrix[i] = who->Get_Rotate_Transform_Matrix()[i];

		if (m_OBB != nullptr) m_OBB->Set_Center_Position(m_Position);

		m_is_Shot = false;
		m_is_Collide = false;
		m_is_Collide_Prod_End = false;
	}

	virtual void Draw()
	{
		glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glRotatef(m_Pitch_Value_Stack, m_Right_Vector[0], m_Right_Vector[1], m_Right_Vector[2]);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
		glMateriali(GL_FRONT, GL_SHININESS, 25);

		glutSolidSphere(OBJECT_SIZE * 0.3f, 7, 7);

		glPopMatrix();
	}

	virtual void Update()
	{
		Move();
		Auto_Rotate();
		Border_Check();
		Particle_Animate();
		Reload();
	}

	void Auto_Rotate()
	{
		if (m_is_Shot)
		{
			m_Pitch_Value_Stack += -5.0f;
			if (m_Pitch_Value_Stack <= -360.0f) m_Pitch_Value_Stack = 0.0f;
		}
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

		else if (!m_is_Shot && m_Whose_Ball_is_it != nullptr)
		{
			m_Position = m_Whose_Ball_is_it->Get_Position();

			m_Forward_Vector[0] = m_Whose_Ball_is_it->Get_Forward_Vector_X(); m_Forward_Vector[1] = m_Whose_Ball_is_it->Get_Forward_Vector_Y(); m_Forward_Vector[2] = m_Whose_Ball_is_it->Get_Forward_Vector_Z();

			m_Position.x += m_Forward_Vector[0] * m_Scale_X * OBJECT_SIZE * 0.5f;
			m_Position.y += 10.0f + m_Forward_Vector[1] * m_Scale_Y * OBJECT_SIZE * 0.5f;
			m_Position.z += m_Forward_Vector[2] * m_Scale_Z * OBJECT_SIZE * 0.5f;
			
			if (m_OBB != nullptr) m_OBB->Set_Center_Position(m_Position);

			for (int i = 0; i < 16; ++i) m_Rotate_Transform_Matrix[i] = m_Whose_Ball_is_it->Get_Rotate_Transform_Matrix()[i];
		}
	}

	virtual bool Collision_Check(Object* other)
	{
		if (m_Whose_Ball_is_it != nullptr && m_is_Shot)
		{
			if (m_Whose_Ball_is_it != other && other->Get_Object_Type() != SNOW_BALL_OBJECT)
			{
				if (!m_is_Static_Object && !other->Get_is_Static() && other->Get_OBB() != nullptr)
				{
					if (m_OBB->OBB_Intersection(other->Get_OBB()))
					{
						Collision_Procedure(OBJECT_COLLIDE_PROCEDURE, other);
						return true;
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
			if (m_Whose_Ball_is_it != other && 
				(other->Get_Object_Type() == ROBOT_OBJECT || other->Get_Object_Type() == SMALL_ENEMY_1 || other->Get_Object_Type() == SMALL_ENEMY_2))
				other->HP_Reduce();
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
			m_is_Collide = true;
			m_is_Collide_Prod_End = true;
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
			Snow_Ball_Init();
			m_Pool_pointer->push(this);
		}
	}
};