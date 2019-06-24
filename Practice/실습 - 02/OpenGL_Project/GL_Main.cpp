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
	uniform_int_distribution<> rand_div_x(RANDOM_DIVISION_NUM_MIN_X, RANDOM_DIVISION_NUM_MAX_X);
	uniform_int_distribution<> rand_div_y(RANDOM_DIVISION_NUM_MIN_Y, RANDOM_DIVISION_NUM_MAX_Y);
	m_Division_Num_X = rand_div_x(m_Random_SEED);
	m_Division_Num_Y = rand_div_y(m_Random_SEED);
	m_Size_X = INIT_WINDOW_SIZE_X / m_Division_Num_X;
	m_Size_Y = INIT_WINDOW_SIZE_Y / m_Division_Num_Y;

	Init_Objects();

	glutKeyboardFunc(Key_Manager);
	glutMouseFunc(Mouse_Manager);

	glutMainLoop();
}

GL_Main::~GL_Main()
{
}

void GL_Main::Key_Manager(unsigned char key, int x, int y)
{

}

void GL_Main::Mouse_Manager(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && !m_instance->m_is_Done_Refresh)
		{
			m_instance->Init_Objects();
			m_instance->m_is_Done_Refresh = true;
		}
		if (state == GLUT_UP) m_instance->m_is_Done_Refresh = false;
		break;
	}
}

void GL_Main::Draw_Scene(void)
{
	m_instance->Draw_Background();
	m_instance->Draw_Objects();
	glFlush();
}

void GL_Main::Reshape(const int w, const int h) { glViewport(0, 0, w, h); glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1.0f, 1.0f); }

void GL_Main::Draw_Background() { glClearColor(0.0f, 0.0f, 1.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT); }

void GL_Main::Draw_Objects() 
{
	for (auto& object : m_instance->m_Object_List) object.second->Draw(); 
}

void GL_Main::Init_Objects()
{
	Destroy_Objects_In_List();

	Object* temp_Object;
	Vertex2D temp_Vertex;
	vector<Vertex2D> v_vertexs;

	uniform_real_distribution<> rand_color(0.0f, 1.0f);
	m_Random_Color = rand_color;

	for (int i = 0; i < 4; ++i)
	{
		m_RColor[i].R = m_Random_Color(m_Random_SEED);
		m_RColor[i].G = m_Random_Color(m_Random_SEED);
		m_RColor[i].B = m_Random_Color(m_Random_SEED);
	}
	
	for (int i = 0; i < m_Division_Num_X; ++i)
	{
		for (int j = 0; j < m_Division_Num_Y; ++j)
		{
			Set_Rectangle(temp_Vertex, v_vertexs, i, j);
			temp_Object = Make_Object(GL_POLYGON, v_vertexs);
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

void GL_Main::Set_Rectangle(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, const float index_x, const float index_y)
{
	float x = (-INIT_WINDOW_SIZE_X / 2) + index_x * m_Size_X;
	float y = (INIT_WINDOW_SIZE_Y / 2) - index_y * m_Size_Y;

	float relative_Value_N = (m_Division_Num_Y - index_y) / m_Division_Num_Y;
	float relative_Value_S = index_y / m_Division_Num_Y;
	float relative_Value_W = (m_Division_Num_X - index_x) / m_Division_Num_X;
	float relative_Value_E = index_x / m_Division_Num_X;

	temp_vertex.color.R = m_RColor[0].R * relative_Value_N + m_RColor[1].R * relative_Value_S + m_RColor[2].R * relative_Value_W + m_RColor[3].R * relative_Value_E;
	temp_vertex.color.G = m_RColor[0].G * relative_Value_N + m_RColor[1].G * relative_Value_S + m_RColor[2].G * relative_Value_W + m_RColor[3].G * relative_Value_E;
	temp_vertex.color.B = m_RColor[0].B * relative_Value_N + m_RColor[1].B * relative_Value_S + m_RColor[2].B * relative_Value_W + m_RColor[3].B * relative_Value_E;

	temp_vertex.point_x = x;
	temp_vertex.point_y = y;
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x;
	temp_vertex.point_y = y - m_Size_Y;
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x + m_Size_X;
	temp_vertex.point_y = y - m_Size_Y;
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = x + m_Size_X;
	temp_vertex.point_y = y;
	v_vertexs.push_back(temp_vertex);
}

void GL_Main::Destroy_Objects_In_List()
{
	if (!m_Object_List.empty())
	{
		for (auto& Data : m_Object_List) delete Data.second;
		m_Object_List.clear();
	}
	m_Curr_Object_Count = 0;
}