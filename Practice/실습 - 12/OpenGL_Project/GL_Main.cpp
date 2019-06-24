#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	glutCreateWindow("½Ç½À 12");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutDisplayFunc(Render_Scene);
	glutReshapeFunc(Reshape);

	m_instance->m_Path_Line = new Object(true);
	m_instance->m_Path_Line->Set_Path_Line(PATH_TYPE_CIRCLE);

	int index = m_instance->Build_Object(true);
	m_instance->Get_Object(index)->Init_Object();
	m_instance->Get_Object(index)->Set_Moving_Object(OBJECT_TYPE_RECTANGLE, m_Path_Line);
	m_instance->Get_Object(index)->Go_To_Start_Point();
	m_instance->Object_Activate(index);

	glutKeyboardFunc(Key_Manager);
	glutMouseFunc(Mouse_Manager);
	glutMainLoop();
}

GL_Main::~GL_Main()
{
}



void GL_Main::Key_Manager(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '0':
		if (m_instance->m_Path_Line) m_instance->m_Path_Line->Set_Path_Line(PATH_TYPE_CIRCLE);
		m_instance->m_is_Set_Over_Custom_Path_Vertex = true;
		for (auto& object : m_instance->m_Object_List)
		{
			if (!object.second->Get_is_Activated()) m_instance->Object_Activate(object.second->Get_Index());
			object.second->Init_Object();
			object.second->Go_To_Start_Point();
		}
		break;

	case '1':
		if (m_instance->m_Path_Line) m_instance->m_Path_Line->Set_Path_Line(PATH_TYPE_SIN_CURVE);
		m_instance->m_is_Set_Over_Custom_Path_Vertex = true;
		for (auto& object : m_instance->m_Object_List)
		{
			if (!object.second->Get_is_Activated()) m_instance->Object_Activate(object.second->Get_Index());
			object.second->Init_Object();
			object.second->Go_To_Start_Point();
		}
		break;

	case '2':
		if (m_instance->m_Path_Line) m_instance->m_Path_Line->Set_Path_Line(PATH_TYPE_TORNADO_SIDE);
		m_instance->m_is_Set_Over_Custom_Path_Vertex = true;
		for (auto& object : m_instance->m_Object_List)
		{
			if (!object.second->Get_is_Activated()) m_instance->Object_Activate(object.second->Get_Index());
			object.second->Init_Object();
			object.second->Go_To_Start_Point();
		}
		break;

	case '3':
		if (m_instance->m_Path_Line) m_instance->m_Path_Line->Set_Path_Line(PATH_TYPE_ZIGZAG);
		m_instance->m_is_Set_Over_Custom_Path_Vertex = true;
		for (auto& object : m_instance->m_Object_List)
		{
			if (!object.second->Get_is_Activated()) m_instance->Object_Activate(object.second->Get_Index());
			object.second->Init_Object();
			object.second->Go_To_Start_Point();
		}
		break;

	case '4':
		if (m_instance->m_Path_Line)
		{
			m_instance->m_Vertexs.clear();
			m_instance->m_Path_Line->Clear_Vectexs_Vector();
			m_instance->m_Path_Line->Init_Object();
			m_instance->m_is_Set_Over_Custom_Path_Vertex = false;
			m_instance->m_Custom_Path_Vertex_Count = 0;
			for (auto& object : m_instance->m_Object_List) object.second->Set_Active(false);
		}
		break;
	case '5':
		if (m_instance->m_Path_Line) m_instance->m_Path_Line->Set_Path_Line(PATH_TYPE_TORNADO_TOP);
		m_instance->m_is_Set_Over_Custom_Path_Vertex = true;
		for (auto& object : m_instance->m_Object_List)
		{
			if (!object.second->Get_is_Activated()) m_instance->Object_Activate(object.second->Get_Index());
			object.second->Init_Object();
			object.second->Go_To_Start_Point();
		}
		break;

	case 'z':
	case 'Z':
		for (auto& object : m_instance->m_Object_List)
			object.second->Set_Moving_Object(OBJECT_TYPE_TRIANGLE);
		break;

	case 'x':
	case 'X':
		for (auto& object : m_instance->m_Object_List)
			object.second->Set_Moving_Object(OBJECT_TYPE_RECTANGLE);
		break;

	case 'c':
	case 'C':
		for (auto& object : m_instance->m_Object_List)
		{
			if (object.second->Get_is_Auto_Scaling())
				object.second->Set_is_Auto_Scaling(false);
			else object.second->Set_is_Auto_Scaling(true);
		}
		break;

	case 's':
	case 'S':
		for (auto& object : m_instance->m_Object_List)
		{
			if (!object.second->Get_Auto_Rotate())
			{
				if (object.second->Get_is_Auto_Move())
					object.second->Set_Auto_Move(false);
				else object.second->Set_Auto_Move(true);
			}
		}
		break;

	case 'r':
	case 'R':
		if (m_instance->m_Path_Line->Get_Path_Line_Type() == PATH_TYPE_CIRCLE)
		{
			for (auto& object : m_instance->m_Object_List)
			{
				if (object.second->Get_Auto_Rotate()) object.second->Auto_Rotate_End();
				else object.second->Auto_Rotate_Start();
			}
		}
		break;

	case 'y':
		m_instance->m_Path_Line->Rotate(-1);
		for (auto& object : m_instance->m_Object_List)
			object.second->Rotate(-1);
		break;
	case 'Y':
		m_instance->m_Path_Line->Rotate(1);
		for (auto& object : m_instance->m_Object_List)
			object.second->Rotate(1);
		break;
	}
}

void GL_Main::Mouse_Manager(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && !m_instance->m_is_Set_Over_Custom_Path_Vertex && !m_instance->m_is_Clicked)
		{
			m_instance->m_is_Clicked = true;
			Vertex3D temp_vertex;
			temp_vertex.color.R = 0.0f;
			temp_vertex.color.G = 0.0f;
			temp_vertex.color.B = 0.0f;
			temp_vertex.point_x = x - INIT_WINDOW_SIZE_X * 0.5f;
			temp_vertex.point_y = INIT_WINDOW_SIZE_Y * 0.5f - y;
			m_instance->m_Vertexs.push_back(temp_vertex);
			m_instance->m_Path_Line->Set_Path_Line(PATH_TYPE_CUSTOM, m_instance->m_Vertexs);
			++m_instance->m_Custom_Path_Vertex_Count;
			if (m_instance->m_Custom_Path_Vertex_Count >= 5)
			{
				m_instance->m_is_Set_Over_Custom_Path_Vertex = true;
				for (auto& object : m_instance->m_Object_List)
				{
					object.second->Init_Object();
					object.second->Go_To_Start_Point();
					m_instance->Object_Activate(object.second->Get_Index());
				}
			}
		}
		else if (state == GLUT_UP) m_instance->m_is_Clicked = false;
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN && !m_instance->m_is_Set_Over_Custom_Path_Vertex)
		{
			m_instance->m_is_Set_Over_Custom_Path_Vertex = true;
			m_instance->m_Path_Line->Set_Path_Line(PATH_TYPE_CUSTOM, m_instance->m_Vertexs);

			if (!m_instance->m_Path_Line->Get_VertexList().empty())
			{
				for (auto& object : m_instance->m_Object_List)
				{
					object.second->Init_Object();
					object.second->Go_To_Start_Point();
					m_instance->Object_Activate(object.second->Get_Index());
				}
			}
		}
		break;
	}
}

void GL_Main::Render_Scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	m_instance->Render_Background();
	m_instance->Render_Objects();
	glutSwapBuffers();
	glutPostRedisplay();
}

void GL_Main::Reshape(const int w, const int h) { glViewport(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f); glMatrixMode(GL_PROJECTION); glOrtho(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f, -400.0f, 400.0f); glutPostRedisplay(); }

void GL_Main::Render_Background() 
{ 
	glClearColor(0.8f, 0.8f, 1.0f, 1.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(0.0f, -INIT_WINDOW_SIZE_Y);
	glVertex2f(0.0f, INIT_WINDOW_SIZE_Y);
	glVertex2f(-INIT_WINDOW_SIZE_X, 0.0f);
	glVertex2f(INIT_WINDOW_SIZE_X, 0.0f);
	glEnd();
}

void GL_Main::Render_Objects()
{
	m_instance->m_Path_Line->Draw();

	for (auto& object : m_instance->m_Object_List)
		if (object.second->Get_is_Activated()) object.second->Draw();
}

int GL_Main::Build_Object(const bool& is_active)
{
	Object* temp_Object;
	Vertex3D temp_Vertex;

	temp_Object = Make_Object(is_active);

	Insert_Object(temp_Object);

	return temp_Object->Get_Index();
}

Object* GL_Main::Make_Object(const bool& is_active)
{
	Object* temp_Object = new Object(is_active);
	
	return temp_Object;
}

void GL_Main::Destroy_All_Objects_In_List()
{
	if (!m_Object_List.empty())
	{
		for (auto& Data : m_Object_List) delete Data.second;
		m_Object_List.clear();
	}
}

void GL_Main::Destroy_Objects_In_List(const int& index)
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
		if (!p->second->Get_is_Destroyed())
		{
			if (p->second->Get_is_Activated())
			{
				p->second->Update();
				glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Object_Timer, object_num);
			}
		}
		else if (p->second->Get_is_Destroyed()) m_instance->Destroy_Objects_In_List(object_num);
	}
}

void GL_Main::Object_Activate(const int& index)
{
	auto p = m_instance->m_Object_List.find(index);
	if (p != m_instance->m_Object_List.end())
	{
		p->second->Set_Active(true);
		glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Object_Timer, index);
	}
}