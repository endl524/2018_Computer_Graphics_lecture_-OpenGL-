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

	Set_Random_Device();

	//glutKeyboardFunc(Key_Manager);
	//glutSpecialFunc(Special_Key_Manager);
	glutMouseFunc(Mouse_Manager);
	
	glutMainLoop();
}

GL_Main::~GL_Main()
{
}

void GL_Main::Set_Random_Device()
{
	mt19937_64 seed(m_Random_Device());
	m_Random_SEED = seed;

	uniform_real_distribution<> rand_Color(0.0f, 1.0f);
	m_Random_Color = rand_Color;

	uniform_real_distribution<> rand_Speed(0.1f, 0.3f);
	m_Random_Speed = rand_Speed;

	uniform_int_distribution<> rand_Type(DRAW_TYPE_POINT, DRAW_TYPE_LINE);
	m_Random_Type = rand_Type;

	uniform_int_distribution<> rand_Dir(0, 1);
	m_Random_Direction = rand_Dir;

	uniform_int_distribution<> rand_round_num(2, 3);
	m_Random_Round_Number = rand_round_num;
}

void GL_Main::Key_Manager(unsigned char key, int x, int y)
{

}

void GL_Main::Special_Key_Manager(int key, int x, int y)
{

}

void GL_Main::Mouse_Manager(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && !m_instance->m_is_Done_Building_Object)
		{
			m_instance->Build_Object(x, INIT_WINDOW_SIZE_Y - y);
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

void GL_Main::Reshape(const int w, const int h) { glViewport(0, 0, w, h); glOrtho(0.0f, w, 0.0f, h, -1.0f, 1.0f); glutPostRedisplay(); }

void GL_Main::Draw_Background() { glClearColor(0.0f, 0.0f, 1.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT); }

void GL_Main::Draw_Objects() 
{
	for (auto& object : m_instance->m_Object_List)
	{
		if (object.second->Get_is_Destroyed()) Destroy_Objects_In_List(object.second->Get_Index());
		else object.second->Draw();
	}
}

void GL_Main::Build_Object(const float& x, const float& y)
{
	Object* temp_Object;
	Vertex2D temp_Vertex;
	vector<Vertex2D> v_vertexs;

	bool is_Clockwise = m_Random_Direction(m_Random_SEED);
	Set_Vertex(temp_Vertex, v_vertexs, x, y, is_Clockwise);
	int type = m_Random_Type(m_Random_SEED);
	GLenum draw_type;
	if (type == DRAW_TYPE_POINT) draw_type = GL_POINTS;
	else if (type == DRAW_TYPE_LINE)  draw_type = GL_LINE_STRIP;
	temp_Object = Make_Object(draw_type, v_vertexs, x, y, is_Clockwise);
	Insert_Object(temp_Object);
	glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME_MS, Start_Object_Timer, m_Curr_Object_Num);
	v_vertexs.clear();
}

Object* GL_Main::Make_Object(const GLenum& type, vector<Vertex2D>& v_vertexs, const float& pos_x, const float& pos_y, const bool& is_clockwise)
{
	Object* temp_Object = new Object(type, v_vertexs, m_Curr_Object_Num, pos_x, pos_y, m_Random_Speed(m_Random_SEED), m_Random_Round_Number(m_Random_SEED), is_clockwise);
	++m_Curr_Object_Num;
	return temp_Object;
}

void GL_Main::Set_Vertex(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, const float& pos_x, const float& pos_y, const bool& is_clockwise)
{
	temp_vertex.color.R = m_Random_Color(m_Random_SEED);
	temp_vertex.color.G = m_Random_Color(m_Random_SEED);
	temp_vertex.color.B = m_Random_Color(m_Random_SEED);

	temp_vertex.angle = is_clockwise == true ? -60.0f : 60.0f;

	temp_vertex.point_x = pos_x;
	temp_vertex.point_y = pos_y;
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
		if (p != m_instance->m_Object_List.end())
		{
			delete p->second;
			m_Object_List.erase(p);
		}
	}
}

void GL_Main::Start_Object_Timer(const int object_num)
{
	auto p = m_instance->m_Object_List.find(object_num);
	if (p != m_instance->m_Object_List.end())
	{
		if (!p->second->Get_is_Drawing_Over())
			p->second->Auto_Drawing();
		p->second->Auto_ColorScale();
		glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME_MS, Start_Object_Timer, object_num);
	}
	glutPostRedisplay();
}