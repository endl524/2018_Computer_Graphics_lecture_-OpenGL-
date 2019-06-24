#pragma once
#include "Object.h"

class Light_Object : Object
{
private:
	static bool s_Light_is_Already_Used[8]; // 라이트가 이미 생성되었는가를 판별할 때 사용.
	int m_Light_Number = 0; // 0 ~ 7 까지.

	float m_Light_Pos[4]{ 0.0f, 0.0f, 0.0f, 0.0f }; // 라이트의 위치
	float m_Light_Rot_Value = 0.0f; // 라이트의 공전값

	// 스포트라이트 변수들
	float m_Light_Dir[3]{ 0.0f, 0.0f, -1.0f }; // 라이트가 비추는 방향
	float m_Light_Exponent;
	float m_Light_CutOff;
	float m_Light_Const_ATNU;
	float m_Light_Linear_ATNU;
	float m_Light_Quatratic_ATNU;

	bool m_is_invalid_Light_Object = true;

public:
	bool Set_Light_Number(const int& num)
	{
		if (num >= 0 && num <= 7)
		{
			if (s_Light_is_Already_Used[num] == false)
			{
				s_Light_is_Already_Used[num] = true;
				m_Light_Number = GL_LIGHT0 + num;
				return true;
			}
		}
		return false;
	}

	int const Get_Light_Number() { return m_Light_Number; }
	bool const Get_is_invalid_Light_Object() { return m_is_invalid_Light_Object; }

	void Set_Material_Ambient_Color(const float& r, const float& g, const float& b, const float& a)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Ambient_Color[0] = r; m_Ambient_Color[1] = g; m_Ambient_Color[2] = b; m_Ambient_Color[3] = a;
			glLightfv(m_Light_Number, GL_AMBIENT, m_Ambient_Color);
		}
	}

	void Set_Material_Diffuse_Color(const float& r, const float& g, const float& b, const float& a)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Diffuse_Color[0] = r; m_Diffuse_Color[1] = g; m_Diffuse_Color[2] = b; m_Diffuse_Color[3] = a;
			glLightfv(m_Light_Number, GL_DIFFUSE, m_Diffuse_Color);
		}
	}

	void Set_Material_Specular_Color(const float& r, const float& g, const float& b, const float& a)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Specular_Color[0] = r; m_Specular_Color[1] = g; m_Specular_Color[2] = b; m_Specular_Color[3] = a;
			glLightfv(m_Light_Number, GL_SPECULAR, m_Specular_Color);
		}
	}

	void Set_Light_Position(const float& x, const float& y, const float& z, const float& a)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Light_Pos[0] = x; m_Light_Pos[1] = y; m_Light_Pos[2] = z; m_Light_Pos[3] = a;
		}
	}

	void Set_Light_Spot_Direction(const float& x, const float& y, const float& z)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Light_Dir[0] = x; m_Light_Dir[1] = y; m_Light_Dir[2] = z;
		}
	}

	void Set_Light_Spot_CutOff(const float& v)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Light_CutOff = v;
			glLightf(m_Light_Number, GL_SPOT_CUTOFF, m_Light_CutOff);
		}
	}

	void Set_Light_Spot_Exponent(const float& v)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Light_Exponent = v;
			glLightf(m_Light_Number, GL_SPOT_EXPONENT, m_Light_Exponent);
		}
	}

	void Set_Light_Const_ATNU(const float& v)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Light_Const_ATNU = v;
			glLightf(m_Light_Number, GL_SPOT_EXPONENT, m_Light_Const_ATNU);
		}
	}

	void Set_Light_Linear_ATNU(const float& v)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Light_Linear_ATNU = v;
			glLightf(m_Light_Number, GL_SPOT_EXPONENT, m_Light_Linear_ATNU);
		}
	}

	void Set_Light_Quatratic_ATNU(const float& v)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Light_Quatratic_ATNU = v;
			glLightf(m_Light_Number, GL_SPOT_EXPONENT, m_Light_Quatratic_ATNU);
		}
	}

	void Rotate_Light(const float& v)
	{
		m_Light_Rot_Value += v;

		if (m_Light_Rot_Value >= 360.0f || m_Light_Rot_Value <= -360.0f)
		{
			int a = m_Light_Rot_Value / 360.0f;
			m_Light_Rot_Value = m_Light_Rot_Value - a * 360;
		}
	}

	virtual void Control_Ambient_Color(const int& sign)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Diffuse_Color[0] += 0.1f * sign;
			m_Diffuse_Color[1] += 0.1f * sign;
			m_Diffuse_Color[2] += 0.1f * sign;
			glLightfv(m_Light_Number, GL_DIFFUSE, m_Diffuse_Color);
		}
	}

	virtual void Control_Diffuse_Color(const int& sign)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Diffuse_Color[0] += 0.1f * sign;
			m_Diffuse_Color[1] += 0.1f * sign;
			m_Diffuse_Color[2] += 0.1f * sign;
			glLightfv(m_Light_Number, GL_DIFFUSE, m_Diffuse_Color);
		}
	}

	virtual void Control_Specular_Color(const int& sign)
	{
		if (m_Light_Number >= GL_LIGHT0 && m_Light_Number <= GL_LIGHT7)
		{
			m_Specular_Color[0] += 0.1f * sign;
			m_Specular_Color[1] += 0.1f * sign;
			m_Specular_Color[2] += 0.1f * sign;
			glLightfv(m_Light_Number, GL_SPECULAR, m_Specular_Color);
		}
	}

public:
	Light_Object(const int& num) 
	{
		if (Set_Light_Number(num))
		{
			m_is_invalid_Light_Object = false;
			Object();
		}
		else m_is_invalid_Light_Object = true;
	}
	~Light_Object()
	{
		if (!m_is_invalid_Light_Object)
		{
			s_Light_is_Already_Used[m_Light_Number - GL_LIGHT0] = false; Light_Off();
		}
	}

	virtual void Draw()
	{
		glPushMatrix();

		glRotatef(m_Light_Rot_Value, 0.0f, 1.0f, 0.0f); // y축 공전

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glLightfv(m_Light_Number, GL_POSITION, m_Light_Pos);
		glLightfv(m_Light_Number, GL_SPOT_DIRECTION, m_Light_Dir);

		glShadeModel(GL_SMOOTH);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_Ambient_Color);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Diffuse_Color);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_Specular_Color);
		glMateriali(GL_FRONT, GL_SHININESS, 20);

		glutSolidCone(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 50, 50);

		glPopMatrix();
	}

	void Light_On() { glEnable(m_Light_Number); }
	void Light_Off() { glDisable(m_Light_Number); }

	void Light_Switch()
	{
		if (glIsEnabled(m_Light_Number)) glDisable(m_Light_Number);
		else glEnable(m_Light_Number);
	}

	virtual void Auto_Move()
	{
		if (m_is_Auto_Moving) Rotate_Light(3.0f);
	}

};