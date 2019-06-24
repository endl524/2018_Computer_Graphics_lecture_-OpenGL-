#include "Object.h"

int Object::s_Object_Count = 0;

Object::Object(const bool& is_active)
{
	Set_Index(s_Object_Count++);
	Set_Active(is_active);
	Set_Random_Device();
}

Object::~Object()
{
}

void Object::Draw()
{
	if (m_is_Activated)
	{
		glLoadIdentity();

		glPushMatrix(); // 다음의 변환들을 스택에 넣는다.
		glRotatef(m_World_Rotate_Angle, 0.0f, m_World_Rotate_Y, 0.0f); // 전역 y축을 기준으로 돌리자.

		// 로컬 변환 적용.
		glRotatef(m_Local_Rotate_Angle, m_Local_Rotate_X, m_Local_Rotate_Y, m_Local_Rotate_Z);
		glTranslatef(m_Local_Translate_X, m_Local_Translate_Y, m_Local_Translate_Z);
		glScalef(m_Local_Scale_X, m_Local_Scale_Y, m_Local_Scale_Z);

		glLineWidth(2.0f);
		glBegin(m_Drawing_Type); // 그리기 시작
		for (auto& data : m_Vertexs)
		{
			glColor3f(data.color.R, data.color.G, data.color.B);
			glVertex3f(data.point_x, data.point_y, data.point_z);
		}
		glEnd(); // 그리기 종료

		glPopMatrix(); // 스택의 top에 있는 변환들을 꺼낸다.
	}
}

void Object::Set_Random_Device()
{
	mt19937_64 seed(m_Random_Device());
	m_Random_SEED = seed;

	uniform_real_distribution<> rand_Color(0.0f, 1.0f);
	m_Random_Color = rand_Color;
}

void Object::Set_Path_Line(const int& type)
{
	m_Vertexs.clear();

	Vertex3D temp_vertex;
	temp_vertex.color.R = 0.0f;
	temp_vertex.color.G = 0.0f;
	temp_vertex.color.B = 0.0f;

	m_World_Rotate_Angle = 0.0f;
	m_World_Rotate_Y = 0.0f;

	float radian = 0.0f;
	float center_x = 0.0f;
	float center_y = 0.0f;
	int count = 0;
	bool sign = 0; // 0: UP, 1: DOWN
	float tornado_radius = PATH_RADIUS * 2.0f;

	m_Drawing_Type = GL_LINE_STRIP;
	m_Object_Type = OBJECT_TYPE_PATH_LINE;
	m_Path_Line_Type = type;

	switch (type)
	{
	case PATH_TYPE_CIRCLE:
		while (radian <= 360.0f)
		{
			temp_vertex.point_x = center_x + cos(radian * 3.1416f / 180.0f) * PATH_RADIUS;
			temp_vertex.point_y = center_y + sin(radian * 3.1416f / 180.0f) * PATH_RADIUS;
			m_Vertexs.push_back(temp_vertex);
			radian += 10.0f;
		}
		break;

	case PATH_TYPE_SIN_CURVE:
		radian = -360.0f;

		while (radian <= 360.0f)
		{
			temp_vertex.point_x = center_x + radian * 3.1416f / 180.0f * PATH_RADIUS * 0.5f;
			temp_vertex.point_y = center_y + sin(radian * 3.1416f / 180.0f) * PATH_RADIUS * 0.5f;
			m_Vertexs.push_back(temp_vertex);
			radian += 10.0f;
		}
		break;

	case PATH_TYPE_TORNADO_SIDE:
		radian = 0.0f;
		center_y = INIT_WINDOW_SIZE_Y * 0.4f;

		while (count < 8)
		{
			temp_vertex.point_x = center_x - cos(radian * 3.1416f / 180.0f) * tornado_radius;
			temp_vertex.point_y = center_y;
			m_Vertexs.push_back(temp_vertex);
			
			radian = radian < 360.0f ? radian + 10.0f : 0.0f;
			if (radian >= 180.0f)
			{
				temp_vertex.color.R = 0.5f;
				temp_vertex.color.G = 0.5f;
				temp_vertex.color.B = 0.5f;
			}
			else
			{
				temp_vertex.color.R = 0.0f;
				temp_vertex.color.G = 0.0f;
				temp_vertex.color.B = 0.0f;
			}
			tornado_radius = cos(radian * 3.1416f / 180.0f) >= 1.0f || cos(radian * 3.1416f / 180.0f) <= -1.0f ? tornado_radius - TORNADO_SIDE_X_RELATIVE_VALUE : tornado_radius;
			count = radian == 0.0f ? count + 1 : count;
			center_y -= TORNADO_SIDE_Y_RELATIVE_VALUE;
		}
		break;

	case PATH_TYPE_ZIGZAG:
		while (count <= 12)
		{
			temp_vertex.point_x = -INIT_WINDOW_SIZE_X * 0.3f + ZIGZAG_WIDTH * count;
			if (!sign) temp_vertex.point_y = INIT_WINDOW_SIZE_Y * 0.4f;
			else temp_vertex.point_y = -INIT_WINDOW_SIZE_Y * 0.4f;
			
			m_Vertexs.push_back(temp_vertex);

			++count;
			sign = sign == false ? true : false;
		}
		break;

	case PATH_TYPE_TORNADO_TOP:
		radian = 0.0f;

		while (count <= 4)
		{
			temp_vertex.point_x = center_x + cos(radian * 3.1416f / 180.0f) * tornado_radius;
			temp_vertex.point_y = center_y + sin(radian * 3.1416f / 180.0f) * tornado_radius;
			m_Vertexs.push_back(temp_vertex);

			radian = radian < 360.0f ? radian + 10.0f : 0.0f;
			count = radian == 0.0f ? count + 1 : count;
			tornado_radius = tornado_radius - TORNADO_TOP_RADIUS_RELATIVE_VALUE >= 0.0f ? tornado_radius - TORNADO_TOP_RADIUS_RELATIVE_VALUE : 0.0f;
		}
		break;
	}
}

void Object::Set_Path_Line(const int& type, const vector<Vertex3D>& vertexs)
{
	m_Vertexs.clear();
	m_Vertexs = vertexs;

	m_Object_Type = OBJECT_TYPE_PATH_LINE;

	if (m_Vertexs.size() > 1) m_Drawing_Type = GL_LINE_STRIP;
	else
	{
		m_Drawing_Type = GL_POINTS;
		glPointSize(3.0f);
	}
}

void Object::Init_Object()
{
	m_Local_Translate_X = 0.0f;
	m_Local_Translate_Y = 0.0f;
	m_Auto_Translate_Value_X = 0.0f;
	m_Auto_Translate_Value_Y = 0.0f;

	m_Local_Rotate_Angle = 0.0f;
	m_Local_Rotate_X = 0.0f;
	m_Local_Rotate_Y = 0.0f;
	m_Local_Rotate_Z = 0.0f;
	m_Auto_Rotate_Angle = 0.0f;
	m_World_Rotate_Angle = 0.0f;
	m_World_Rotate_Y = 0.0f;

	m_Local_Scale_X = 1.0f;
	m_Local_Scale_Y = 1.0f;
	m_Local_Scale_Z = 1.0f;
	m_Auto_Scale_Sign = 1;

	m_Solid_Color.R = 1.0f;
	m_Solid_Color.G = 0.2f;
	m_Solid_Color.B = 0.2f;

	m_is_Auto_Moving = true;
	m_is_Auto_Rotating = false;
	m_is_Auto_Scaling = false;
}

void Object::Set_Moving_Object(const int& type)
{
	m_Object_Type = type;

	m_Vertexs.clear();

	Vertex3D temp_object;
	temp_object.color.R = m_Random_Color(m_Random_SEED);
	temp_object.color.G = m_Random_Color(m_Random_SEED);
	temp_object.color.B = m_Random_Color(m_Random_SEED);

	switch (type)
	{
	case OBJECT_TYPE_TRIANGLE:
		m_Drawing_Type = GL_TRIANGLES;
		temp_object.point_x = 0.0f;
		temp_object.point_y = OBJECT_SIZE;
		m_Vertexs.push_back(temp_object);

		temp_object.point_x = -OBJECT_SIZE;
		temp_object.point_y = -OBJECT_SIZE;
		m_Vertexs.push_back(temp_object);

		temp_object.point_x = OBJECT_SIZE;
		temp_object.point_y = -OBJECT_SIZE;
		m_Vertexs.push_back(temp_object);
		break;

	case OBJECT_TYPE_RECTANGLE:
		m_Drawing_Type = GL_QUADS;
		temp_object.point_x = -OBJECT_SIZE;
		temp_object.point_y = OBJECT_SIZE;
		m_Vertexs.push_back(temp_object);

		temp_object.point_x = -OBJECT_SIZE;
		temp_object.point_y = -OBJECT_SIZE;
		m_Vertexs.push_back(temp_object);

		temp_object.point_x = OBJECT_SIZE;
		temp_object.point_y = -OBJECT_SIZE;
		m_Vertexs.push_back(temp_object);

		temp_object.point_x = OBJECT_SIZE;
		temp_object.point_y = OBJECT_SIZE;
		m_Vertexs.push_back(temp_object);
		break;
	}
}

void Object::Set_Moving_Object(const int& type, Object* path_line)
{
	m_Path_Line_Object = path_line;
	m_Curr_Vertex_iter = m_Path_Line_Object->m_Vertexs.begin();
	Set_Moving_Object(type);
	Init_Object();
}

void Object::Auto_ColorScale()
{
	if (m_is_Auto_Scaling)
	{
		if (m_Object_Type == OBJECT_TYPE_PATH_LINE)
		{
			m_Vertexs.front().color.R += COLOR_SCALE_VALUE_R * m_Color_Scale_Sign_R;
			if (m_Vertexs.front().color.R >= 1.0f || m_Vertexs.front().color.R <= 0.0f) { m_Color_Scale_Sign_R *= -1; }
			m_Vertexs.front().color.G += COLOR_SCALE_VALUE_G * m_Color_Scale_Sign_G;
			if (m_Vertexs.front().color.G >= 1.0f || m_Vertexs.front().color.G <= 0.0f) { m_Color_Scale_Sign_G *= -1; }
			m_Vertexs.front().color.B += COLOR_SCALE_VALUE_B * m_Color_Scale_Sign_B;
			if (m_Vertexs.front().color.B >= 1.0f || m_Vertexs.front().color.B <= 0.0f) { m_Color_Scale_Sign_B *= -1; }

			for (auto& Data : m_Vertexs)
				Data.color = m_Vertexs.front().color;
		}
		else
		{
			m_Solid_Color.R += COLOR_SCALE_VALUE_R * m_Color_Scale_Sign_R;
			if (m_Solid_Color.R >= 1.0f || m_Solid_Color.R <= 0.0f) { m_Color_Scale_Sign_R *= -1; }
			m_Solid_Color.G += COLOR_SCALE_VALUE_G * m_Color_Scale_Sign_G;
			if (m_Solid_Color.G >= 1.0f || m_Solid_Color.G <= 0.0f) { m_Color_Scale_Sign_G *= -1; }
			m_Solid_Color.B += COLOR_SCALE_VALUE_B * m_Color_Scale_Sign_B;
			if (m_Solid_Color.B >= 1.0f || m_Solid_Color.B <= 0.0f) { m_Color_Scale_Sign_B *= -1; }
		}
	}
}

void Object::Auto_Translate()
{
	if (m_is_Auto_Moving)
	{
		if (roundf(m_Curr_Vertex_iter->point_x) - 5.0f <= roundf(m_Local_Translate_X) &&
			roundf(m_Curr_Vertex_iter->point_x) + 5.0f >= roundf(m_Local_Translate_X) &&
			roundf(m_Curr_Vertex_iter->point_y) - 5.0f <= roundf(m_Local_Translate_Y) &&
			roundf(m_Curr_Vertex_iter->point_y) + 5.0f >= roundf(m_Local_Translate_Y))
		{
			++m_Curr_Vertex_iter;

			if (m_Curr_Vertex_iter == m_Path_Line_Object->m_Vertexs.end())
			{
				m_Curr_Vertex_iter = m_Path_Line_Object->m_Vertexs.begin();
				m_Local_Translate_X = m_Curr_Vertex_iter->point_x;
				m_Local_Translate_Y = m_Curr_Vertex_iter->point_y;
			}

			m_Vertexs_Distance_X = m_Curr_Vertex_iter->point_x - m_Local_Translate_X;
			m_Vertexs_Distance_Y = m_Curr_Vertex_iter->point_y - m_Local_Translate_Y;
			m_Distance_Length = sqrtf(m_Vertexs_Distance_X * m_Vertexs_Distance_X + m_Vertexs_Distance_Y * m_Vertexs_Distance_Y);
			m_Auto_Translate_Value_X = m_Vertexs_Distance_X / m_Distance_Length * OBJECT_AUTO_MOVE_SPEED;
			m_Auto_Translate_Value_Y = m_Vertexs_Distance_Y / m_Distance_Length * OBJECT_AUTO_MOVE_SPEED;
		}

		else
		{
			m_Local_Translate_X += m_Auto_Translate_Value_X;
			m_Local_Translate_Y += m_Auto_Translate_Value_Y;
		}
	}
}

void Object::Auto_Rotate()
{
	if (m_is_Auto_Rotating)
		m_Local_Rotate_Angle = m_Local_Rotate_Angle + OBJECT_AUTO_ROTATE_SPEED <= 360.0f ? m_Local_Rotate_Angle + OBJECT_AUTO_ROTATE_SPEED : 0.0f;
}

void Object::Auto_Rotate_Start()
{
	m_is_Auto_Moving = false;
	m_Local_Translate_X = 0.0f;
	m_Local_Translate_Y = PATH_RADIUS;
	m_Local_Rotate_Z = 1.0f;
	m_is_Auto_Rotating = true;
}

void Object::Auto_Rotate_End()
{
	m_is_Auto_Rotating = false;
	Go_To_Start_Point();
	m_Local_Rotate_Angle = 0.0f;
	m_Local_Rotate_Z = 0.0f;
	m_is_Auto_Moving = true;
}

void Object::Auto_Scale()
{
	if (m_is_Auto_Scaling)
	{
		m_Local_Scale_X += 0.05f * m_Auto_Scale_Sign;
		m_Local_Scale_Y += 0.05f * m_Auto_Scale_Sign;
		if (m_Local_Scale_X >= 2.0f || m_Local_Scale_X <= 1.0f) m_Auto_Scale_Sign *= -1;
	}
}

void Object::Go_To_Start_Point()
{
	if (m_Path_Line_Object != nullptr)
	{
		m_Curr_Vertex_iter = m_Path_Line_Object->m_Vertexs.begin();
		m_Local_Translate_X = m_Curr_Vertex_iter->point_x;
		m_Local_Translate_Y = m_Curr_Vertex_iter->point_y;
	}
}

void Object::Rotate(const int& sign)
{
	m_World_Rotate_Angle = m_World_Rotate_Angle + OBJECT_ROTATE_VALUE * sign < 360.0f && m_World_Rotate_Angle + OBJECT_ROTATE_VALUE * sign > -360.0f ? m_World_Rotate_Angle + OBJECT_ROTATE_VALUE * sign : 0.0f;
	m_World_Rotate_Y = 1.0f;
}

void Object::Update()
{
	Auto_Translate();
	Auto_Rotate();
	Auto_Scale();
	Auto_ColorScale();
}