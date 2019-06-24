#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	glutCreateWindow("½Ç½À 9¹ø");
	glutDisplayFunc(Draw_Scene);
	glutReshapeFunc(Reshape);

	Set_Random_Device();

	Init_Objects(OBJECTS_COUNT);

	glutMouseFunc(Mouse_Manager);
	glutMotionFunc(Mouse_Clicked_Move_Manager);
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

	uniform_real_distribution<> rand_Pos_X(RANDOM_POSITION_MIN_X, RANDOM_POSITION_MAX_X);
	uniform_real_distribution<> rand_Pos_Y(RANDOM_POSITION_MIN_Y, RANDOM_POSITION_MAX_Y);
	m_Random_Position_X = rand_Pos_X;
	m_Random_Position_Y = rand_Pos_Y;
}

void GL_Main::Mouse_Manager(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			m_instance->m_Eraser_Object->Set_Active(true);
			m_instance->m_Eraser_Object->Set_Position(x, INIT_WINDOW_SIZE_Y - y);
			m_instance->m_Eraser_Object->Position_Update();
			m_instance->m_Eraser_Object->Collision_Check(m_instance->m_Object_List);
		}
		else if (state == GLUT_UP)
		{
			m_instance->m_Eraser_Object->Set_Active(false);
		}
		break;
	}
}

void GL_Main::Mouse_Clicked_Move_Manager(int x, int y)
{
	if (m_instance->m_Eraser_Object->Get_is_Activated())
	{
		m_instance->m_Eraser_Object->Set_Position(x, INIT_WINDOW_SIZE_Y - y);
		m_instance->m_Eraser_Object->Position_Update();
		m_instance->m_Eraser_Object->Collision_Check(m_instance->m_Object_List);
		glutPostRedisplay();
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
	if (m_instance->m_Eraser_Object->Get_is_Activated()) 
		m_instance->m_Eraser_Object->Draw();

	for (auto& object : m_instance->m_Object_List)
	{
		if (object.second->Get_is_Activated()) object.second->Draw();
	}
}

void GL_Main::Build_Object(const float& x, const float& y, const bool& is_active, const int& type)
{
	Object* temp_Object;
	Vertex2D temp_Vertex;
	vector<Vertex2D> v_vertexs;
	int size_x, size_y;

	if (type == OBJECT_TYPE_ERASER) { size_x = ERASER_SIZE_X; size_y = ERASER_SIZE_Y; }
	else { size_x = FIXED_RECT_SIZE_X; size_y = FIXED_RECT_SIZE_Y; }

	Set_Rectangle(temp_Vertex, v_vertexs, x, y, size_x, size_y, type);
	temp_Object = Make_Object(GL_POLYGON, v_vertexs, x, y, size_x, size_y, is_active, type);

	if (type == OBJECT_TYPE_ERASER) m_Eraser_Object = temp_Object;
	else
	{
		Insert_Object(temp_Object);
		glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Object_Timer, temp_Object->Get_Index());
	}

	v_vertexs.clear();
}

Object* GL_Main::Make_Object(const GLenum& type, vector<Vertex2D>& v_vertexs, const float& pos_x, const float& pos_y, const float& size_x, const float& size_y, const bool& is_active, const int& obj_type)
{
	Object* temp_Object = new Object(type, v_vertexs, pos_x, pos_y, size_x, size_y, is_active, obj_type);
	
	return temp_Object;
}

void GL_Main::Set_Rectangle(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, const float& pos_x, const float& pos_y, const float& size_x, const float& size_y, const int& type)
{
	if (type == OBJECT_TYPE_ERASER)
	{
		temp_vertex.color.R = 0.0f;
		temp_vertex.color.G = 0.0f;
		temp_vertex.color.B = 0.0f;
	}
	else
	{
		temp_vertex.color.R = m_Random_Color(m_Random_SEED);
		temp_vertex.color.G = m_Random_Color(m_Random_SEED);
		temp_vertex.color.B = m_Random_Color(m_Random_SEED);
	}

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
		if (p->second->Get_is_Activated() && p->second->Get_Type() != OBJECT_TYPE_ERASER)
		{
			p->second->Auto_ColorScale();

			p->second->Position_Update();
		}
		if (!p->second->Get_is_Destroyed())
			glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Object_Timer, object_num);
		else m_instance->Destroy_Objects_In_List(object_num);
	}
	glutPostRedisplay();
}

void GL_Main::Init_Objects(const int& num)
{
	m_instance->Build_Object(0.0f, 0.0f, false, OBJECT_TYPE_ERASER);

	float x, y;
	
	for (int i = 0; i < num; ++i)
	{
		x = m_Random_Position_X(m_Random_SEED);
		y = m_Random_Position_Y(m_Random_SEED);
		m_instance->Build_Object(x, y, true, OBJECT_TYPE_NORMAL_RECT);
	}
}