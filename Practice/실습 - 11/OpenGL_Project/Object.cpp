#include "Object.h"

int Object::s_Object_Count = 0;

Object::Object(const bool& is_active)
{
	Set_Index(s_Object_Count++);
	Set_Active(is_active);
	Set_Random_Device();

	Shape_Change(OBJECT_TYPE_SIN_CURVE);
}

Object::~Object()
{
}

void Object::Draw()
{
	if (m_is_Activated)
	{
		glLineWidth(2.0f);
		
		glLoadIdentity();

		glPushMatrix(); // 다음의 변환들을 스택에 넣는다.

		glTranslatef(m_Scale_Pos, 1.0f, 1.0f);
		glScalef(m_Scale_Size, 1.0f, 1.0f);

		glBegin(m_Drawing_Type);

		for (auto& data : m_Vertexs)
		{
			glColor3f(data.color.R, data.color.G, data.color.B);
			glVertex3f(data.point_x, data.point_y, data.point_z);
		}

		glEnd();

		glPopMatrix(); // 이전 변환들을 꺼낸다.
	}
}

void Object::Set_Random_Device()
{
	mt19937_64 seed(m_Random_Device());
	m_Random_SEED = seed;

	uniform_real_distribution<> rand_Color(0.0f, 1.0f);
	m_Random_Color = rand_Color;
}

void Object::Shape_Change(const int& type)
{
	m_Object_Type = type;
	m_Radian = 0.0f;
	m_Scale_Size = 1.0f;
	m_Scale_Pos = 0.0f;
	m_Size = 50.0f;
	
	m_Vertexs.clear();

	switch (type)
	{
	case OBJECT_TYPE_SIN_CURVE: Make_Sin_Curve(); break;

	case OBJECT_TYPE_SPRING: Make_Spring(); break;

	case OBJECT_TYPE_RIBBON: Make_Ribbon(); break;

	case OBJECT_TYPE_RECTANGLE: Make_Rectangle(); break;
	}
}

void Object::Make_Sin_Curve()
{
	m_Drawing_Type = GL_LINE_STRIP;

	Vertex2D temp_vertex;
	float x, y;
	bool is_make_over = false;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);

	while (!is_make_over)
	{
		temp_vertex.point_x = m_Radian * 3.1416f / 180.0f * m_Size + INIT_WINDOW_SIZE_X * 0.105f;
		temp_vertex.point_y = sin(m_Radian * 3.1416f / 180.0f) * m_Size + INIT_WINDOW_SIZE_Y * 0.5f;
		m_Vertexs.push_back(temp_vertex);

		if (m_Radian >= 180.0f * 24)
		{
			is_make_over = true;
			m_Radian = 0.0f;
		}
		m_Radian += 10.0f;
	}
}

void Object::Make_Spring()
{
	m_Drawing_Type = GL_LINE_STRIP;
	m_Spring_Center_X = 0.0f;
	m_is_Moved_Once = false;

	Vertex2D temp_vertex;
	float center_x = 0.0f;
	float center_y = 0.0f;
	bool is_make_over = false;
	int count = 0;

	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	center_x = INIT_WINDOW_SIZE_X * 0.25f + SPRING_RADIUS;
	center_y = INIT_WINDOW_SIZE_Y * 0.5f;

	while (!is_make_over)
	{
		temp_vertex.point_y = center_y + sin(m_Radian * 3.1416f / 180.0f) * SPRING_RADIUS;
		if (m_Radian <= 180.0f) temp_vertex.point_x = center_x - cos(m_Radian * 3.1416f / 180.0f) * SPRING_RADIUS;
		else temp_vertex.point_x = (center_x + SPRING_RADIUS * 0.5f) - cos(m_Radian * 3.1416f / 180.0f) * SPRING_RADIUS * 0.5f ;
		m_Vertexs.push_back(temp_vertex);

		m_Radian += 10.0f;
		if (m_Radian >= 360.0f)
		{
			m_Radian = 0.0f;
			center_x = m_Vertexs.rbegin()->point_x + SPRING_RADIUS;
			++count;
			if (count >= 10) is_make_over = true;
		}
	}
}

void Object::Make_Ribbon()
{
	m_Drawing_Type = GL_TRIANGLES;

	Vertex2D temp_vertex;
	float x, y;

	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);

	temp_vertex.point_x = INIT_WINDOW_SIZE_X * 0.25f;
	temp_vertex.point_y = INIT_WINDOW_SIZE_Y * 0.6f;
	m_Vertexs.push_back(temp_vertex);

	temp_vertex.point_x = INIT_WINDOW_SIZE_X * 0.25f;
	temp_vertex.point_y = INIT_WINDOW_SIZE_Y * 0.4f;
	m_Vertexs.push_back(temp_vertex);

	temp_vertex.point_x = INIT_WINDOW_SIZE_X * 0.5f;
	temp_vertex.point_y = INIT_WINDOW_SIZE_Y * 0.5f;
	m_Vertexs.push_back(temp_vertex);
	m_Vertexs.push_back(temp_vertex);

	temp_vertex.point_x = INIT_WINDOW_SIZE_X * 0.75f;
	temp_vertex.point_y = INIT_WINDOW_SIZE_Y * 0.6f;
	m_Vertexs.push_back(temp_vertex);

	temp_vertex.point_x = INIT_WINDOW_SIZE_X * 0.75f;
	temp_vertex.point_y = INIT_WINDOW_SIZE_Y * 0.4f;
	m_Vertexs.push_back(temp_vertex);
}

void Object::Make_Rectangle()
{
	m_Drawing_Type = GL_QUADS;

	Vertex2D temp_vertex;
	float pos_x[RECT_COUNT], pos_y = INIT_WINDOW_SIZE_Y * 0.5f;
	for (int i = 0; i < RECT_COUNT; ++i)
	{
		if (i != 0) pos_x[i] = 2 * RECT_SIZE_VALUE + pos_x[i - 1];
		else pos_x[i] = -RECT_SIZE_VALUE;
	}
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);

	for (int i = 0; i < RECT_COUNT; ++i)
	{
		temp_vertex.point_x = pos_x[i] - RECT_SIZE_VALUE * 0.5f;
		temp_vertex.point_y = pos_y + RECT_SIZE_VALUE * 0.5f;
		m_Vertexs.push_back(temp_vertex);

		temp_vertex.point_x = pos_x[i] - RECT_SIZE_VALUE * 0.5f;
		temp_vertex.point_y = pos_y - RECT_SIZE_VALUE * 0.5f;
		m_Vertexs.push_back(temp_vertex);

		temp_vertex.point_x = pos_x[i] + RECT_SIZE_VALUE * 0.5f;
		temp_vertex.point_y = pos_y - RECT_SIZE_VALUE * 0.5f;
		m_Vertexs.push_back(temp_vertex);

		temp_vertex.point_x = pos_x[i] + RECT_SIZE_VALUE * 0.5f;
		temp_vertex.point_y = pos_y + RECT_SIZE_VALUE * 0.5f;
		m_Vertexs.push_back(temp_vertex);
	}
	
}

void Object::Auto_ColorScale()
{
	m_Vertexs.front().color.R += COLOR_SCALE_VALUE_R * m_Color_Scale_Sign_R;
	if (m_Vertexs.front().color.R >= 1.0f || m_Vertexs.front().color.R <= 0.0f) { m_Color_Scale_Sign_R *= -1; }
	m_Vertexs.front().color.G += COLOR_SCALE_VALUE_G * m_Color_Scale_Sign_G;
	if (m_Vertexs.front().color.G >= 1.0f || m_Vertexs.front().color.G <= 0.0f) { m_Color_Scale_Sign_G *= -1; }
	m_Vertexs.front().color.B += COLOR_SCALE_VALUE_B * m_Color_Scale_Sign_B;
	if (m_Vertexs.front().color.B >= 1.0f || m_Vertexs.front().color.B <= 0.0f) { m_Color_Scale_Sign_B *= -1; }

	for (auto& Data: m_Vertexs)
		Data.color = m_Vertexs.front().color;
}

void Object::Animation()
{
	Vertex2D temp_vertex;
	auto iter = m_Vertexs.begin();
	switch (m_Object_Type)
	{
	case OBJECT_TYPE_SIN_CURVE:
		if (m_Animation_Sign == -1)
		{
			temp_vertex = m_Vertexs.front();
			m_Vertexs.pop_front();
			temp_vertex.point_x = m_Vertexs.back().point_x + 3.1416f / 18.0f * m_Size;
			temp_vertex.point_y = m_Vertexs.front().point_y + sin(m_Radian * 3.1416f / 180.0f);

			m_Vertexs.push_back(temp_vertex);
		}
		else if (m_Animation_Sign == 1)
		{
			temp_vertex = m_Vertexs.back();
			m_Vertexs.pop_back();
			temp_vertex.point_x = m_Vertexs.front().point_x - 3.1416f / 18.0f * m_Size;
			temp_vertex.point_y = m_Vertexs.back().point_y - sin(m_Radian * 3.1416f / 180.0f);

			m_Vertexs.push_front(temp_vertex);
		}

		if (m_Radian >= 180.0f * 2) m_Radian = 0.0f;
		m_Radian += 10.0f;

		for (auto& Data : m_Vertexs)
			Data.point_x += 3.1416f / 18.0f * m_Animation_Sign * m_Size;
		break;

	case OBJECT_TYPE_SPRING:
		if (m_Animation_Sign == -1)
		{
			if (!m_is_Moved_Once)
			{
				m_Spring_Center_X = m_Vertexs.back().point_x + SPRING_RADIUS;
				m_is_Moved_Once = true;
			}
			temp_vertex = m_Vertexs.front();
			m_Vertexs.pop_front();
			if (m_Radian <= 180.0f) temp_vertex.point_x = m_Spring_Center_X - cos(m_Radian * 3.1416f / 180.0f) * SPRING_RADIUS;
			else temp_vertex.point_x = (m_Spring_Center_X + SPRING_RADIUS * 0.5f) - cos(m_Radian * 3.1416f / 180.0f) * SPRING_RADIUS * 0.5f;
			m_Vertexs.push_back(temp_vertex);
			m_Radian += 10.0f;
		}
		else if (m_Animation_Sign == 1)
		{
			if (!m_is_Moved_Once)
			{
				m_Spring_Center_X = m_Vertexs.front().point_x;
				m_is_Moved_Once = true;
			}
			temp_vertex = m_Vertexs.back();
			m_Vertexs.pop_back();
			if (m_Radian >= -180.0f)  temp_vertex.point_x = (m_Spring_Center_X + SPRING_RADIUS * 0.5f) - cos(m_Radian * 3.1416f / 180.0f) * SPRING_RADIUS * 0.5f; 
			else temp_vertex.point_x = m_Spring_Center_X - cos(m_Radian * 3.1416f / 180.0f) * SPRING_RADIUS;
			m_Vertexs.push_front(temp_vertex);
			m_Radian -= 10.0f;
		}

		if (m_Radian >= 360.0f || m_Radian <= -360.0f)
		{
			m_Radian = 0.0f;
			m_is_Moved_Once = false;
		}

		for (auto& Data : m_Vertexs)
			Data.point_x += m_Animation_Sign * SPRING_ANIMATE_VALUE;
		m_Spring_Center_X += m_Animation_Sign * SPRING_ANIMATE_VALUE;
		break;

	case OBJECT_TYPE_RIBBON:
		m_Size += RIBBON_ANIMATE_VALUE * m_Animation_Sign;
		if (m_Size <= SIZE_MIN)
		{
			m_Animation_Sign *= -1; m_Size = SIZE_MIN;
		}
		else if (m_Size >= SIZE_MAX)
		{
			m_Animation_Sign *= -1; m_Size = SIZE_MAX;
		}

		iter->point_x -= RIBBON_ANIMATE_VALUE * m_Animation_Sign;
		++iter; iter->point_x -= RIBBON_ANIMATE_VALUE * m_Animation_Sign;
		++iter; ++iter;
		++iter; iter->point_x += RIBBON_ANIMATE_VALUE * m_Animation_Sign;
		++iter; iter->point_x += RIBBON_ANIMATE_VALUE * m_Animation_Sign;
		
		break;

	case OBJECT_TYPE_RECTANGLE:
		if (m_Animation_Sign == -1)
		{
			Move(MOVE_LEFT);
			if (iter->point_x < -RECT_SIZE_VALUE * 0.5f)
			{
				++iter; ++iter;
				if (iter->point_x < -RECT_SIZE_VALUE * 0.5f)
				{
					--iter; --iter;
					auto p = --m_Vertexs.end();
					iter->point_x = p->point_x + RECT_SIZE_VALUE;
					m_Vertexs.push_back(*iter);
					++iter; iter->point_x = p->point_x + RECT_SIZE_VALUE;
					m_Vertexs.push_back(*iter);
					++iter; iter->point_x = p->point_x + 2 * RECT_SIZE_VALUE;
					m_Vertexs.push_back(*iter);
					++iter; iter->point_x = p->point_x + 2 * RECT_SIZE_VALUE;
					m_Vertexs.push_back(*iter);
					m_Vertexs.pop_front();
					m_Vertexs.pop_front();
					m_Vertexs.pop_front();
					m_Vertexs.pop_front();
				}
			}
		}

		else if (m_Animation_Sign == 1)
		{
			Move(MOVE_RIGHT);
			iter = --m_Vertexs.end();
			if (iter->point_x > INIT_WINDOW_SIZE_X + RECT_SIZE_VALUE * 0.5f)
			{
				--iter; --iter;
				if (iter->point_x > INIT_WINDOW_SIZE_X + RECT_SIZE_VALUE * 0.5f)
				{
					++iter; ++iter;
					auto p = m_Vertexs.begin();
					iter->point_x = p->point_x - RECT_SIZE_VALUE;
					m_Vertexs.push_front(*iter);
					--iter; iter->point_x = p->point_x - RECT_SIZE_VALUE;
					m_Vertexs.push_front(*iter);
					--iter; iter->point_x = p->point_x - 2 * RECT_SIZE_VALUE;
					m_Vertexs.push_front(*iter);
					--iter; iter->point_x = p->point_x - 2 * RECT_SIZE_VALUE;
					m_Vertexs.push_front(*iter);
					m_Vertexs.pop_back();
					m_Vertexs.pop_back();
					m_Vertexs.pop_back();
					m_Vertexs.pop_back();
				}
			}
		}
		
		break;
	}
}

void Object::Move(const int& dir)
{
	switch (dir)
	{
	case MOVE_UP:
		for (auto& Data : m_Vertexs)
			Data.point_y += MOVE_SPEED;
		
		break;

	case MOVE_DOWN:
		for (auto& Data : m_Vertexs)
			Data.point_y -= MOVE_SPEED;
		break;

	case MOVE_LEFT:
		for (auto& Data : m_Vertexs)
			Data.point_x -= MOVE_SPEED;
		m_Spring_Center_X -= MOVE_SPEED;
		break;

	case MOVE_RIGHT:
		for (auto& Data : m_Vertexs)
			Data.point_x += MOVE_SPEED;
		m_Spring_Center_X += MOVE_SPEED;
		break;
	}
}

void Object::Scale(const int& sign)
{
	m_Scale_Size += SIZE_SCALE_VALUE * sign;
	m_Scale_Pos += m_Scale_Pos > 360.0f || m_Scale_Pos < -800.0f ? 0.0f : 40.0f * -sign;
	if (m_Scale_Size >= 3.0f)
	{
		m_Scale_Size = 3.0f;
		m_Scale_Pos = -800.0f;
	}
	else if (m_Scale_Size <= 0.1f)
	{
		m_Scale_Size = 0.1f;
		m_Scale_Pos = 360.0f;
	}
}

void Object::Update()
{
	Auto_ColorScale();
	if (m_Animation_Sign != 0) Animation();
}