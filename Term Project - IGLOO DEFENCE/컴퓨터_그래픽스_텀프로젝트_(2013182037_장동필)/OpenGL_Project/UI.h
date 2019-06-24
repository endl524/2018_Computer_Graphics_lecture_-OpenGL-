#pragma once
#include "stdafx.h"
#include "Texture.h"

class UI
{
protected:
	static int s_UI_Count;
	int m_Index = -1;

	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;
	Color4f m_Color;

	string m_Text;
	Color4f m_Text_Color;
	Texture* m_Texture;

	float m_Width;
	float m_Height;

	bool m_is_Fade_UI = false;
	bool m_is_Activate = true;

public:
	void Set_Index(const int& index) { m_Index = index; }
	int const Get_Index() { return m_Index; }

	void Set_Position(const float& x, const float& y, const float& z) { m_Position.x = x; m_Position.y = y; m_Position.z = z; }
	
	void Set_Position(const int& axis, const float& v)
	{
		if (axis == 0) m_Position.x = v;
		else if (axis == 1) m_Position.y = v;
		else if (axis == 2) m_Position.z = v;
	}
	void Plus_Position(const int& axis, const float& v)
	{
		if (axis == 0) m_Position.x += v;
		else if (axis == 1) m_Position.y += v;
		else if (axis == 2) m_Position.z += v;
	}
	float const Get_Position(const int& axis)
	{
		if (axis == 0) return m_Position.x;
		else if (axis == 1) return m_Position.y;
		else if (axis == 2) return m_Position.z;
		else return -1;
	}
	
	void Set_Rotation(const float& x, const float& y, const float& z) { m_Rotation.x = x; m_Rotation.y = y; m_Rotation.z = z; }
	void Set_Rotate_X(const float& v) { m_Rotation.x = v; }
	void Set_Rotate_Y(const float& v) { m_Rotation.y = v; }
	void Plus_Rotate_Y(const float& v) { m_Rotation.y += v; }
	void Set_Rotate_Z(const float& v) { m_Rotation.z = v; }
	void Plus_Rotate_Z(const float& v) { m_Rotation.z += v; }
	void Set_Scale(const float& x, const float& y, const float& z) 
	{ 
		m_Scale.x = x; m_Scale.y = y; m_Scale.z = z;
		if (m_Texture != nullptr)
		{
			m_Width = m_Scale.x * m_Texture->Get_Width();
			m_Height = m_Scale.y * m_Texture->Get_Height();
		}
	}
	void Set_Scale(const int& axis, const float& v)
	{
		if (axis == 0) m_Scale.x = v;
		else if (axis == 1) m_Scale.y = v;
		else if (axis == 2) m_Scale.z = v;

		if (m_Texture != nullptr)
		{
			m_Width = m_Scale.x * m_Texture->Get_Width();
			m_Height = m_Scale.y * m_Texture->Get_Height();
		}
	}
	void Plus_Scale(const int& axis, const float& v)
	{
		if (axis == 0) m_Scale.x += v;
		else if (axis == 1) m_Scale.y += v;
		else if (axis == 2) m_Scale.z += v;

		if (m_Texture != nullptr)
		{
			m_Width = m_Scale.x * m_Texture->Get_Width();
			m_Height = m_Scale.y * m_Texture->Get_Height();
		}
	}
	void Set_Color(const float& r, const float& g, const float& b, const float& a) { m_Color.R = r; m_Color.G = g; m_Color.B = b; m_Color.A = a; }
	
	void Set_Text(string text) { m_Text = text; }
	void Set_Text_Color(const float& r, const float& g, const float& b, const float& a) { m_Text_Color.R = r; m_Text_Color.G = g; m_Text_Color.B = b; m_Text_Color.A = a; }
	
	void Set_Texture(const char* image_file_path, const int& width, const int& height)
	{
		if (m_Texture != nullptr)
		{
			delete m_Texture;
			m_Texture = nullptr;
		}
		m_Texture = new Texture();
		m_Texture->Set_Texture(image_file_path, width, height);
		m_Width = m_Scale.x * m_Texture->Get_Width();
		m_Height = m_Scale.y * m_Texture->Get_Height();
	}
	Texture* const Get_Texture() { return m_Texture; }


	float const Get_Position_X() { return m_Position.x; }
	float const Get_Position_Y() { return m_Position.y; }
	float const Get_Position_Z() { return m_Position.z; }

	float const Get_Scale_X() { return m_Scale.x; }
	float const Get_Scale_Y() { return m_Scale.y; }
	float const Get_Scale_Z() { return m_Scale.z; }

	float const Get_Width() { return m_Width; }
	float const Get_Height() { return m_Height; }

	void Set_is_Fade_UI(const bool& b) { m_is_Fade_UI = b; }
	void Plus_Fade(const float& a) { m_Color.R += a; m_Color.G += a; m_Color.B += a; m_Color.A += a; }
	void Plus_Fade(const float& c, const float& a_max) { m_Color.R += c; m_Color.G += c; m_Color.B += c;  if (m_Color.A < a_max) m_Color.A += c; else m_Color.A = a_max; }
	void Set_Fade(const float& a) { m_Color.R = a; m_Color.G = a; m_Color.B = a; m_Color.A = a; }
	float const Get_Alpha() { return m_Color.A; }

	void Set_is_Activate(const bool& b) { m_is_Activate = b; }


public:
	UI() { Set_Index(s_UI_Count++); UI_Init(); }
	~UI() 
	{
		if (m_Texture != nullptr)
		{
			delete m_Texture;
			m_Texture = nullptr;
		}
	}

	void UI_Init()
	{
		Set_Position(0.0f, 0.0f, 0.0f);
		Set_Rotation(0.0f, 0.0f, 0.0f);
		Set_Scale(1.0f, 1.0f, 1.0f);
		Set_Color(1.0f, 1.0f, 1.0f, 1.0f);
		m_Width = 0.0f; m_Height = 0.0f;
	}

	virtual void Draw()
	{
		if (m_is_Activate)
		{
			glPushMatrix();
			glTranslatef(m_Position.x, m_Position.y, m_Position.z);
			glRotatef(m_Rotation.x, 1.0f, 0.0f, 0.0f);
			glRotatef(m_Rotation.y, 0.0f, 1.0f, 0.0f);
			glRotatef(m_Rotation.z, 0.0f, 0.0f, 1.0f);
			glScalef(m_Scale.x, m_Scale.y, m_Scale.z);

			glDisable(GL_LIGHTING);
			if (!m_is_Fade_UI) glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			else glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

			if (m_Texture != nullptr) m_Texture->Mapping_Texture(false);

			glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);

			float half_width = 0, half_height = 0;
			if (m_Texture != nullptr) { half_width = m_Texture->Get_Width() * 0.5f; half_height = m_Texture->Get_Height() * 0.5f; }
			else { half_width = OBJECT_SIZE; half_height = OBJECT_SIZE; }

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f);
			glVertex3f(-half_width, half_height, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f(-half_width, -half_height, 0.0f);
			glTexCoord2f(1.0f, 0.0f);
			glVertex3f(half_width, -half_height, 0.0f);
			glTexCoord2f(1.0f, 1.0f);
			glVertex3f(half_width, half_height, 0.0f);

			if (m_Texture != nullptr) m_Texture->Mapping_Off(false);

			glEnd();

			if (!m_Text.empty())
			{
				glColor4f(m_Color.R, m_Color.G, m_Color.B, m_Color.A);
				for (auto& character : m_Text) glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, character);
			}

			glEnable(GL_LIGHTING);

			glPopMatrix();
		}
	}

	void Color_Bright() { m_Color.A = 1.0f; m_Text_Color.A = 1.0f; }
	void Color_less_Dark() { m_Color.A = 0.5f; m_Text_Color.A = 0.5f; }
	void Color_Dark() { m_Color.A = 0.3f; m_Text_Color.A = 0.3f; }
};