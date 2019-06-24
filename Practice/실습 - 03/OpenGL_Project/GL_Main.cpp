#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	glutCreateWindow("Example");
	glutDisplayFunc(Draw_Scene);
	glutReshapeFunc(Reshape);

	m_Curr_Object_Count = 0;

	mt19937_64 seed(m_Random_Device());
	m_Random_SEED = seed;

	m_Division_Num_X = 3;
	m_Division_Num_Y = 3;
	m_Size_X = INIT_WINDOW_SIZE_X / m_Division_Num_X;
	m_Size_Y = INIT_WINDOW_SIZE_Y / m_Division_Num_Y;
	m_Offset_X = m_Size_X * 0.1f;
	m_Offset_Y = m_Size_Y * 0.1f;

	uniform_int_distribution<> random_Shape(LINE, PENTAGON);
	m_Curr_Shape_Num = random_Shape(m_Random_SEED);
	Init_Objects();

	glutMainLoop();
}

GL_Main::~GL_Main()
{
}

void GL_Main::Draw_Scene(void)
{
	//m_instance->Draw_Background();
	m_instance->Draw_Objects();
	glFlush();
}

void GL_Main::Reshape(const int w, const int h) { glViewport(0, 0, w, h); glOrtho(-w * 0.5f, w  * 0.5f, -h * 0.5f, h  * 0.5f, -1.0f, 1.0f); }

void GL_Main::Draw_Background() { glClearColor(0.0f, 0.0f, 1.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT); }

void GL_Main::Draw_Objects() 
{
	for (auto& object : m_instance->m_Object_List) object.second->Draw(); 
}

void GL_Main::Init_Objects()
{
	Object* temp_Object = nullptr;
	Vertex2D temp_Vertex;
	vector<Vertex2D> v_vertexs;

	uniform_real_distribution<> rand_color(0.05f, 1.0f);
	m_Random_Color = rand_color;

	int sign = 1;

	for (int j = 0; j < m_Division_Num_Y; ++j)
	{
		for (int i = 0; i < m_Division_Num_X; ++i)
		{

			switch (m_Curr_Shape_Num)
			{
			case LINE:
				Set_Line(temp_Vertex, v_vertexs, i, j);
				temp_Object = Make_Object(GL_LINES, v_vertexs);
				break;

			case TRIANGLE:
				Set_Triangle(temp_Vertex, v_vertexs, i, j);
				temp_Object = Make_Object(GL_POLYGON, v_vertexs);
				break;
			case RECTANGLE:
				Set_Rectangle(temp_Vertex, v_vertexs, i, j);
				temp_Object = Make_Object(GL_POLYGON, v_vertexs);
				break;

			case PENTAGON:
				Set_Pentagon(temp_Vertex, v_vertexs, i, j);
				temp_Object = Make_Object(GL_POLYGON, v_vertexs);
				break;
			}
			if (m_Curr_Shape_Num == PENTAGON || m_Curr_Shape_Num == LINE) sign *= -1;
			if (sign == 1) ++m_Curr_Shape_Num;
			else --m_Curr_Shape_Num;

			Insert_Object(temp_Object);
			v_vertexs.clear();
		}
	}
}

Object* GL_Main::Make_Object(const GLenum type, vector<Vertex2D>& v_vertexs)
{
	Object* temp_Object = new Object(type, v_vertexs, m_Curr_Object_Count);
	++m_Curr_Object_Count;
	return temp_Object;
}

void GL_Main::Set_Line(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, const int index_x, const int index_y)
{
	float x = (-INIT_WINDOW_SIZE_X * 0.5f) + index_x * m_Size_X;
	float y = (INIT_WINDOW_SIZE_Y * 0.5f) - index_y * m_Size_Y;

	temp_vertex.point_x = x + m_Offset_X;
	temp_vertex.point_y = y - m_Offset_Y;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x + (m_Size_X - m_Offset_X);
	temp_vertex.point_y = y - (m_Size_Y - m_Offset_Y);
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
}

void GL_Main::Set_Triangle(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, const int index_x, const int index_y)
{
	float x = (-INIT_WINDOW_SIZE_X * 0.5f) + index_x * m_Size_X;
	float y = (INIT_WINDOW_SIZE_Y * 0.5f) - index_y * m_Size_Y;

	temp_vertex.point_x = x + (m_Size_X * 0.5f);
	temp_vertex.point_y = y - m_Offset_Y;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x + m_Offset_X;
	temp_vertex.point_y = y - (m_Size_Y - m_Offset_Y);
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x + (m_Size_X - m_Offset_X);
	temp_vertex.point_y = y - (m_Size_Y - m_Offset_Y);
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
}

void GL_Main::Set_Rectangle(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, const int index_x, const int index_y)
{
	float x = (-INIT_WINDOW_SIZE_X * 0.5f) + index_x * m_Size_X;
	float y = (INIT_WINDOW_SIZE_Y * 0.5f) - index_y * m_Size_Y;

	temp_vertex.point_x = x + m_Offset_X;
	temp_vertex.point_y = y - m_Offset_Y;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x + m_Offset_X;
	temp_vertex.point_y = y - (m_Size_Y - m_Offset_Y) ;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x + (m_Size_X - m_Offset_X);
	temp_vertex.point_y = y - (m_Size_Y - m_Offset_Y);
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x + (m_Size_X - m_Offset_X);
	temp_vertex.point_y = y - m_Offset_Y;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
}

void GL_Main::Set_Pentagon(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, const int index_x, const int index_y)
{
	float x = (-INIT_WINDOW_SIZE_X * 0.5f) + index_x * m_Size_X + (m_Size_X * 0.5f);
	float y = (INIT_WINDOW_SIZE_Y * 0.5f) - index_y * m_Size_Y;

	temp_vertex.point_x = x; // 상단 중심점
	temp_vertex.point_y = y - m_Offset_Y;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x - (m_Size_X * 0.4f) + m_Offset_X;
	temp_vertex.point_y = y - m_Size_Y * 0.5f + m_Offset_Y;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x - (m_Size_X * 0.25f) + m_Offset_X;
	temp_vertex.point_y = y - m_Size_Y + m_Offset_Y;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x + (m_Size_X * 0.25f) - m_Offset_X;
	temp_vertex.point_y = y - m_Size_Y + m_Offset_Y;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x + (m_Size_X * 0.4f) - m_Offset_X;
	temp_vertex.point_y = y - m_Size_Y * 0.5f + m_Offset_Y;
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	v_vertexs.push_back(temp_vertex);
}