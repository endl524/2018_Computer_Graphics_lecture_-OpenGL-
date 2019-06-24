#include "Object.h"

float Object::s_Main_Speed = 1.0f;
bool Object::m_is_Stop_Moving = false;

Object::Object(const GLenum type, vector<Vertex2D>& v_vertexs, const int index, const float pos_x, const float pos_y, const float size_x, const float size_y, const float dir_x, const float dir_y, const float speed)
{
	Set_Drawing_Type(type);
	Set_Vertexs_vector(v_vertexs);
	Set_Index(index);
	Set_Position(pos_x, pos_y);
	Set_Size(size_x, size_y);
	Set_Direction(dir_x, dir_y);
	Set_Speed(speed);
}

Object::~Object()
{
}

void Object::Draw()
{
	glBegin(m_Drawing_Type);

	for (auto& data : m_Vertexs)
	{
		glColor3f(data.color.R, data.color.G, data.color.B);
		glVertex2f(data.point_x, data.point_y);
	}

	glEnd();
}

void Object::Auto_Bounding_Move()
{
	if (!m_is_Stop_Moving)
	{
		if (m_Position_X - m_Size_X * 0.5f <= 0.0f)
		{
			m_Position_X = m_Size_X * 0.5f;
			m_Direction_X *= -1;
		}
		else if (m_Position_X + m_Size_X * 0.5f >= INIT_WINDOW_SIZE_X)
		{
			m_Position_X = INIT_WINDOW_SIZE_X - m_Size_X * 0.5f;
			m_Direction_X *= -1;
		}
		if (m_Position_Y - m_Size_Y * 0.5f <= 0.0f)
		{
			m_Position_Y = m_Size_Y * 0.5f;
			m_Direction_Y *= -1;
		}
		else if (m_Position_Y + m_Size_Y * 0.5f >= INIT_WINDOW_SIZE_Y)
		{
			m_Position_Y = INIT_WINDOW_SIZE_Y - m_Size_Y * 0.5f;
			m_Direction_Y *= -1;
		}

		m_Position_X += m_Direction_X * m_Speed * s_Main_Speed;
		m_Position_Y += m_Direction_Y * m_Speed * s_Main_Speed;
	}
}

void Object::Start_Wall_Riding(const int wall_riding_dir)
{
	if (m_Moving_Type != MOVE_TYPE_WALL_RIDING)
	{
		m_Moving_Type = MOVE_TYPE_WALL_RIDING;
		m_Prev_Position_X = m_Position_X;
		m_Prev_Position_Y = m_Position_Y;
		m_Prev_Direction_X = m_Direction_X;
		m_Prev_Direction_Y = m_Direction_Y;

		for (int i = 0; i < 6; ++i) m_Wall_Riding_Touch_Check[i] = false;

		m_Wall_Riding_Direction = wall_riding_dir;
		if (m_Wall_Riding_Direction == WALL_RIDING_CLOCKWISE) m_Direction_X = 1.0f;
		else m_Direction_X = -1.0f;
		m_Direction_Y = 0.0f;
	}
}

void Object::Auto_Wall_Riding_Move()
{
	if (!m_is_Stop_Moving)
	{
		if (!m_Wall_Riding_Touch_Check[5])
		{
			if (m_Position_X - m_Size_X * 0.5f < 0.0f) // W
			{
				m_Wall_Riding_Touch_Check[0] = true;
				m_Position_X = m_Size_X * 0.55f;
				if (m_Wall_Riding_Direction == WALL_RIDING_CLOCKWISE) m_Direction_Y = 1.0f;
				else
				{
					if (m_Wall_Riding_Touch_Check[0] && m_Wall_Riding_Touch_Check[1] &&
						m_Wall_Riding_Touch_Check[2] && m_Wall_Riding_Touch_Check[3])
						m_Wall_Riding_Touch_Check[4] = true;
					m_Direction_Y = -1.0f;
				}
			}
			if (m_Position_X + m_Size_X * 0.5f > INIT_WINDOW_SIZE_X) // E
			{
				m_Wall_Riding_Touch_Check[1] = true;
				m_Position_X = INIT_WINDOW_SIZE_X - m_Size_X * 0.55f;
				if (m_Wall_Riding_Direction == WALL_RIDING_CLOCKWISE)
				{
					if (m_Wall_Riding_Touch_Check[0] && m_Wall_Riding_Touch_Check[1] &&
						m_Wall_Riding_Touch_Check[2] && m_Wall_Riding_Touch_Check[3])
						m_Wall_Riding_Touch_Check[4] = true;
					m_Direction_Y = -1.0f;
				}
				else m_Direction_Y = 1.0f;
			}
			if (m_Position_Y - m_Size_Y * 0.5f < 0.0f) // N
			{
				m_Wall_Riding_Touch_Check[2] = true;
				m_Position_Y = m_Size_Y * 0.55f;
				if (m_Wall_Riding_Direction == WALL_RIDING_CLOCKWISE) m_Direction_X = -1.0f;
				else m_Direction_X = 1.0f;
			}
			if (m_Position_Y + m_Size_Y * 0.5f > INIT_WINDOW_SIZE_Y) // S
			{
				m_Wall_Riding_Touch_Check[3] = true;
				m_Position_Y = INIT_WINDOW_SIZE_Y - m_Size_Y * 0.55f;
				if (m_Wall_Riding_Direction == WALL_RIDING_CLOCKWISE) m_Direction_X = 1.0f;
				else m_Direction_X = -1.0f;
			}
		}

		if (m_Wall_Riding_Touch_Check[4] && !m_Wall_Riding_Touch_Check[5] && m_Prev_Position_Y + m_Speed * s_Main_Speed * 0.5f >= m_Position_Y && m_Prev_Position_Y - m_Speed * s_Main_Speed * 0.5f <= m_Position_Y)
		{
			m_Direction_Y = 0.0f;
			m_Position_Y = m_Prev_Position_Y;
			if (m_Wall_Riding_Direction == WALL_RIDING_CLOCKWISE) m_Direction_X = -1.0f;
			else m_Direction_X = 1.0f;
			m_Wall_Riding_Touch_Check[5] = true;
		}
		else if (m_Wall_Riding_Touch_Check[5] &&
			m_Prev_Position_X + m_Speed * s_Main_Speed * 0.5f >= m_Position_X &&
			m_Prev_Position_X - m_Speed * s_Main_Speed * 0.5f <= m_Position_X &&
			m_Prev_Position_Y + m_Speed * s_Main_Speed * 0.5f >= m_Position_Y &&
			m_Prev_Position_Y - m_Speed * s_Main_Speed * 0.5f <= m_Position_Y)
		{
			// 거의 근접하면
			Finish_Wall_Riding();
		}

		m_Position_X += m_Direction_X * m_Speed * s_Main_Speed;
		m_Position_Y += m_Direction_Y * m_Speed * s_Main_Speed;
	}
}

void Object::Finish_Wall_Riding()
{
	if (m_Moving_Type == MOVE_TYPE_WALL_RIDING)
	{
		m_Moving_Type = MOVE_TYPE_BOUNDING;
		m_Position_X = m_Prev_Position_X;
		m_Position_Y = m_Prev_Position_Y;
		m_Direction_X = m_Prev_Direction_X;
		m_Direction_Y = m_Prev_Direction_Y;
	}
}

void Object::Auto_Scale()
{
	m_Vertexs[0].color.R += COLOR_SCALE_VALUE_R * m_Color_Scale_Sign_R;
	if (m_Vertexs[0].color.R >= 1.0f || m_Vertexs[0].color.R <= 0.0f) { m_Color_Scale_Sign_R *= -1; }
	m_Vertexs[0].color.G += COLOR_SCALE_VALUE_G * m_Color_Scale_Sign_G;
	if (m_Vertexs[0].color.G >= 1.0f || m_Vertexs[0].color.G <= 0.0f) { m_Color_Scale_Sign_G *= -1; }
	m_Vertexs[0].color.B += COLOR_SCALE_VALUE_B * m_Color_Scale_Sign_B;
	if (m_Vertexs[0].color.B >= 1.0f || m_Vertexs[0].color.B <= 0.0f) { m_Color_Scale_Sign_B *= -1; }

	for (int i = 1; i < 4; ++i)
	{
		m_Vertexs[i].color.R = m_Vertexs[0].color.R;
		m_Vertexs[i].color.G = m_Vertexs[0].color.G;
		m_Vertexs[i].color.B = m_Vertexs[0].color.B;
	}

	m_Size_X += SCALE_VALUE_X * m_Scale_Sign_X;
	if (m_Size_X >= RECT_SIZE_MAX_X || m_Size_X <= RECT_SIZE_MIN_X) { m_Scale_Sign_X *= -1; }
	m_Size_Y += SCALE_VALUE_Y * m_Scale_Sign_Y;
	if (m_Size_Y >= RECT_SIZE_MAX_Y || m_Size_Y <= RECT_SIZE_MIN_Y) { m_Scale_Sign_Y *= -1; }

	Size_Refresh();
}

void Object::Size_Refresh()
{
	m_Vertexs[0].point_x = m_Position_X - m_Size_X * 0.5f;
	m_Vertexs[0].point_y = m_Position_Y + m_Size_Y * 0.5f;
	m_Vertexs[1].point_x = m_Position_X - m_Size_X * 0.5f;
	m_Vertexs[1].point_y = m_Position_Y - m_Size_Y * 0.5f;
	m_Vertexs[2].point_x = m_Position_X + m_Size_X * 0.5f;
	m_Vertexs[2].point_y = m_Position_Y - m_Size_Y * 0.5f;
	m_Vertexs[3].point_x = m_Position_X + m_Size_X * 0.5f;
	m_Vertexs[3].point_y = m_Position_Y + m_Size_Y * 0.5f;
}


void Object::Change_Speed(const int sign)
{
	s_Main_Speed += MAIN_SPEED_RELATIVE_VALUE * sign;
}

void Object::Stop_Move()
{
	m_is_Stop_Moving = true;
}

void Object::Start_Move()
{
	m_is_Stop_Moving = false;
}