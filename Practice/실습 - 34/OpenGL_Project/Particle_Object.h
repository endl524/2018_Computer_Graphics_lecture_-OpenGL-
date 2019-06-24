#pragma once
#include "Object.h"

class Particle_Object : Object
{
private:
	Vector3 m_Particle_Positions[15];
	Vector3 m_Particle_Directions[15];
	float m_Particle_Speeds[15];
	Vector3 m_Particle_Stacked_Distance[15];
	bool m_is_Particle_End[15];
	bool m_is_Particle_Over = true;

public:
	void Set_is_Particle_Over(const bool& b) { m_is_Particle_Over = b; }
	bool const Get_is_Particle_Over() { return m_is_Particle_Over; }

	void Set_Random_Speeds(uniform_real_distribution<>& urd, mt19937_64& seed)
	{
		for (int i = 0; i < 15; ++i)
			m_Particle_Speeds[i] = urd(seed);
	}

	void Set_Random_Directions(uniform_real_distribution<>& urd, mt19937_64& seed)
	{
		for (int i = 0; i < 15; ++i)
		{
			m_Particle_Directions[i].x = urd(seed);
			m_Particle_Directions[i].y = urd(seed);
			m_Particle_Directions[i].z = urd(seed);
		}
	}

public:
	Particle_Object() : Object() { Init_Particle(); };
	~Particle_Object() {};

	void Init_Particle()
	{
		for (int i = 0; i < 15; ++i)
		{
			m_Particle_Positions[i].x = 0.0f;
			m_Particle_Positions[i].y = 0.0f;
			m_Particle_Positions[i].z = 0.0f;
		}
		Roll(-45.0f);
	}

	virtual void Draw()
	{
		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glDisable(GL_LIGHTING);
		for (int i = 0; i < 15; ++i)
		{
			glPushMatrix();
			glTranslatef(m_Particle_Positions[i].x, m_Particle_Positions[i].y, m_Particle_Positions[i].z);
			glColor3f(0.5f, 1.0f, 0.7f);
			glutSolidSphere(OBJECT_SIZE * 0.05f, 5, 5);
			glPopMatrix();
		}
		glEnable(GL_LIGHTING);

		glPopMatrix();
	}

	virtual void Update()
	{
		Auto_Move();
	}

	virtual void Auto_Move()
	{
		if (!m_is_Particle_Over)
		{
			bool is_Particle_Over = true;
			for (int i = 0; i < 15; ++i)
			{
				if (!m_is_Particle_End[i])
				{
					m_Particle_Stacked_Distance[i].x += m_Particle_Directions[i].x * m_Particle_Speeds[i];
					m_Particle_Stacked_Distance[i].y += m_Particle_Directions[i].y * m_Particle_Speeds[i];
					m_Particle_Stacked_Distance[i].z += m_Particle_Directions[i].z * m_Particle_Speeds[i];
					m_Particle_Positions[i].x += m_Particle_Directions[i].x * m_Particle_Speeds[i];
					m_Particle_Positions[i].y += m_Particle_Directions[i].y * m_Particle_Speeds[i];
					m_Particle_Positions[i].z += m_Particle_Directions[i].z * m_Particle_Speeds[i];

					if (m_Particle_Stacked_Distance[i].x >= 60.0f || m_Particle_Stacked_Distance[i].y >= 60.0f
						|| m_Particle_Stacked_Distance[i].z >= 60.0f)
					{
						m_is_Particle_End[i] = true;
						m_Particle_Positions[i].x = 0.0f; m_Particle_Positions[i].y = 0.0f; m_Particle_Positions[i].z = 0.0f;
					}

					is_Particle_Over = false;
				}
			}

			if (is_Particle_Over)
			{
				m_is_Particle_Over = true;

				for (int i = 0; i < 15; ++i)
				{
					m_is_Particle_End[i] = false;
					m_Particle_Stacked_Distance[i].x = 0.0f;
					m_Particle_Stacked_Distance[i].y = 0.0f;
					m_Particle_Stacked_Distance[i].z = 0.0f;
				}
			}
		}
	}

};