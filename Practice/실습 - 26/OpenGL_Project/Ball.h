#pragma once
#include "Object.h"

class Ball : Object
{
private:
	Vector3 m_Direction;
	float m_Radius;

public:
	Ball(const float& pos_x, const float& pos_y, const float& pos_z, const float& dir_x, const float& dir_y, const float& dir_z) : Object(true) { Ball_Initialize(pos_x, pos_y, pos_z, dir_x, dir_y, dir_z); };
	~Ball() {};

	void Set_Position_Y(const float& value) { m_Position.y = value; }
	float const Get_Position_Y() { return m_Position.y; }
	void Set_Position_Z(const float& value) { m_Position.z = value; }
	float const Get_Position_Z() { return m_Position.z; }
	float const Get_Radius() { return m_Radius; }

	void Ball_Initialize(const float& pos_x, const float& pos_y, const float& pos_z, const float& dir_x, const float& dir_y, const float& dir_z)
	{
		m_Object_Type = BALL_OBJECT;
		m_is_Static_Object = false;

		m_Position.x = pos_x;
		m_Position.y = pos_y;
		m_Position.z = pos_z;

		m_Direction.x = dir_x;
		m_Direction.y = dir_y;
		m_Direction.z = dir_z;

		m_Scale_X = 0.5f;
		m_Scale_Y = 0.5f;
		m_Scale_Z = 0.5f;

		m_OBB->OBB_Init(m_Position, m_Scale_X, m_Scale_Y, m_Scale_Z);

		m_Radius = OBJECT_SIZE * 0.5f * m_Scale_X;

		m_Color.R = 1.0f; m_Color.G = 1.0f; m_Color.B = 1.0f; m_Color.A = 1.0f;
	}

	virtual void Draw()
	{
		glPushMatrix();
		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);
		glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
		glutSolidSphere(OBJECT_SIZE * 0.5f, 15, 15);
		glPopMatrix();
	}

	void Auto_Move()
	{
		m_Position.x += m_Direction.x * 1.5f;
		m_Position.y += m_Direction.y * 1.5f;
		m_Position.z += m_Direction.z * 1.5f;

		m_OBB->Set_Center_Position(m_Position);
	}

	virtual void Collision_Check(Object* other)
	{
		if (!m_is_Static_Object)
		{
			if (other->Get_Object_Type() == CUBE_OBJECT)
			{
				for (int i = 0; i < 6; ++i)
				{
					if (m_OBB->OBB_Intersection(other->Get_Cube_Collider(i)))
						Collision_Procedure(other->Get_Collider_Normal_Vector(i));
				}
			}
		}
	}

	void Collision_Procedure(const Vector4& collider_Normal_Vector)
	{
		if (!m_is_Static_Object)
		{
			Vector3 temp_Direction = m_Direction;
			temp_Direction.x *= -1; temp_Direction.y *= -1; temp_Direction.z *= -1;
			Vector3 result_Direction;
			float k = Vector_DotProduct(temp_Direction, collider_Normal_Vector) * 2.0f;
			result_Direction.x = collider_Normal_Vector.x * k;
			result_Direction.y = collider_Normal_Vector.y * k;
			result_Direction.z = collider_Normal_Vector.z * k;

			m_Direction.x += result_Direction.x; m_Direction.y += result_Direction.y; m_Direction.z += result_Direction.z;
			
			m_Position.x += collider_Normal_Vector.x;
			m_Position.y += collider_Normal_Vector.y;
			m_Position.z += collider_Normal_Vector.z;
			m_OBB->Set_Center_Position(m_Position);
		}
	}

	float Vector_DotProduct(const Vector3& v0, const Vector4& v1)
	{
		return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	}

	virtual void Update()
	{
		Auto_Move();
	}
};

