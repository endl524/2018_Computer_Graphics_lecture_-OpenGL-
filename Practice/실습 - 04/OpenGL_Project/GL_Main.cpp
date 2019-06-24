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

	m_Curr_Object_Num = 0;

	mt19937_64 seed(m_Random_Device());
	m_Random_SEED = seed;

	uniform_real_distribution<> rand_Color(0.0f, 1.0f);
	m_Random_Color = rand_Color;

	uniform_real_distribution<> rand_Size_X(RECT_SIZE_MIN_X, RECT_SIZE_MAX_X);
	uniform_real_distribution<> rand_Size_Y(RECT_SIZE_MIN_Y, RECT_SIZE_MAX_Y);
	m_Random_Size_X = rand_Size_X;
	m_Random_Size_Y = rand_Size_Y;

	//glutKeyboardFunc(Key_Manager);
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
		if (state == GLUT_DOWN && !m_instance->m_is_Done_Building_Object)
		{
			m_instance->Build_Object(x, y);
			m_instance->m_is_Done_Building_Object = true;
		}
		else if (state == GLUT_UP && m_instance->m_is_Done_Building_Object)
		{
			m_instance->m_is_Done_Building_Object = false;
		}
		break;
	}
}

void GL_Main::Draw_Scene(void)
{
	m_instance->Draw_Background();
	m_instance->Draw_Objects();
	glFlush();
}

void GL_Main::Reshape(const int w, const int h) { glViewport(0, 0, w, h); glOrtho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f); }

void GL_Main::Draw_Background() { glClearColor(0.0f, 0.0f, 1.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT); }

void GL_Main::Draw_Objects() 
{
	for (auto& object : m_instance->m_Object_List) object.second->Draw(); 
}

void GL_Main::Build_Object(const float x, const float y)
{
	Object* temp_Object;
	Vertex2D temp_Vertex;
	vector<Vertex2D> v_vertexs;

	float size_X = m_Random_Size_X(m_Random_SEED);
	float size_Y = m_Random_Size_Y(m_Random_SEED);

	Set_Rectangle(temp_Vertex, v_vertexs, x, y, size_X, size_Y);
	temp_Object = Make_Object(GL_POLYGON, v_vertexs, x, y, size_X, size_Y);
	Insert_Object(temp_Object);
	glutTimerFunc(50, Start_Object_Timer, m_Curr_Object_Num);
	v_vertexs.clear();
}

Object* GL_Main::Make_Object(const GLenum type, vector<Vertex2D>& v_vertexs, const float pos_x, const float pos_y, const float size_x, const float size_y)
{
	Object* temp_Object = new Object(type, v_vertexs, m_Curr_Object_Num, pos_x, pos_y, size_x, size_y);
	++m_Curr_Object_Num;
	if (m_Curr_Object_Num >= 11)
	{
		auto p = m_instance->m_Object_List.begin();
		delete p->second;
		m_instance->m_Object_List.erase(p);
	}
	return temp_Object;
}

void GL_Main::Set_Rectangle(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, const float pos_x, const float pos_y, const float size_x, const float size_y)
{
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);

	temp_vertex.point_x = pos_x - size_x * 0.5f;
	temp_vertex.point_y = pos_y + size_y * 0.5f;
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = pos_x - size_x * 0.5f;
	temp_vertex.point_y = pos_y - size_y * 0.5f;
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = pos_x + size_x * 0.5f;
	temp_vertex.point_y = pos_y - size_y * 0.5f;
	v_vertexs.push_back(temp_vertex);
	temp_vertex.point_x = pos_x + size_x * 0.5f;
	temp_vertex.point_y = pos_y + size_y * 0.5f;
	v_vertexs.push_back(temp_vertex);
}

void GL_Main::Destroy_All_Objects_In_List()
{
	if (!m_Object_List.empty())
	{
		for (auto& Data : m_Object_List) delete Data.second;
		m_Object_List.clear();
	}
	m_Curr_Object_Num = 0;
}

void GL_Main::Destroy_Objects_In_List(const int index)
{
	if (!m_Object_List.empty())
	{
		auto p = m_instance->m_Object_List.find(index);
		delete p->second;
		m_Object_List.erase(p);
	}
}

void GL_Main::Start_Object_Timer(const int object_num)
{
	auto p = m_instance->m_Object_List.find(object_num);
	if (p != m_instance->m_Object_List.end())
	{
		p->second->Auto_Scale();
		glutTimerFunc(50, Start_Object_Timer, object_num);
	}
	glutPostRedisplay();
}