#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	glutCreateWindow("½Ç½À 11");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutDisplayFunc(Render_Scene);
	glutReshapeFunc(Reshape);

	m_instance->Build_Object(true);

	glutKeyboardFunc(Key_Manager);
	glutMainLoop();
}

GL_Main::~GL_Main()
{
}



void GL_Main::Key_Manager(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		if (!m_instance->m_Object_List.empty())
		{
			m_instance->m_Curr_Shape = OBJECT_TYPE_SIN_CURVE;
			m_instance->m_Object_List.begin()->second->Shape_Change(m_instance->m_Curr_Shape);
		}
		break;
	case '2':
		if (!m_instance->m_Object_List.empty())
		{
			m_instance->m_Curr_Shape = OBJECT_TYPE_SPRING;
			m_instance->m_Object_List.begin()->second->Shape_Change(m_instance->m_Curr_Shape);
		}
		break;
	case '3':
		if (!m_instance->m_Object_List.empty())
		{
			m_instance->m_Curr_Shape = OBJECT_TYPE_RIBBON;
			m_instance->m_Object_List.begin()->second->Shape_Change(m_instance->m_Curr_Shape);
		}
		break;
	case '4':
		if (!m_instance->m_Object_List.empty())
		{
			m_instance->m_Curr_Shape = OBJECT_TYPE_RECTANGLE;
			m_instance->m_Object_List.begin()->second->Shape_Change(m_instance->m_Curr_Shape);
		}
		break;

	case 'x':
		if (!m_instance->m_Object_List.empty())
			m_instance->m_Object_List.begin()->second->Move(MOVE_LEFT);
		break;
	case 'X': 
		if (!m_instance->m_Object_List.empty())
			m_instance->m_Object_List.begin()->second->Move(MOVE_RIGHT);
		break;

	case 'y':
		if (!m_instance->m_Object_List.empty())
			m_instance->m_Object_List.begin()->second->Move(MOVE_UP);
		break;
	case 'Y':
		if (!m_instance->m_Object_List.empty())
			m_instance->m_Object_List.begin()->second->Move(MOVE_DOWN);
		break;

	case 's': 
		if (!m_instance->m_Object_List.empty())
			m_instance->m_Object_List.begin()->second->Scale(-1); 
		break;

	case 'S': 
		if (!m_instance->m_Object_List.empty())
			m_instance->m_Object_List.begin()->second->Scale(1); 
		break;

	case 'r': 
		if (!m_instance->m_Object_List.empty())
		{
			if (m_instance->m_Object_List.begin()->second->Get_Animation_Sign() != -1)
			{
				m_instance->m_Object_List.begin()->second->Set_Animation_Sign(-1);
				m_instance->m_Object_List.begin()->second->Animation();
				m_instance->m_Object_List.begin()->second->Set_Animation_Sign(0);
			}
		}
		break;

	case 'R': 
		if (!m_instance->m_Object_List.empty())
		{
			if (m_instance->m_Object_List.begin()->second->Get_Animation_Sign() != 1)
			{
				m_instance->m_Object_List.begin()->second->Set_Animation_Sign(1);
				m_instance->m_Object_List.begin()->second->Animation();
				m_instance->m_Object_List.begin()->second->Set_Animation_Sign(0);
			}
		}
		break;

	case 'a': 
		if (!m_instance->m_Object_List.empty())
		{
			if (m_instance->m_Object_List.begin()->second->Get_Animation_Sign() != -1)
			{
				bool is_Already_Timer_Working = true;
				if (m_instance->m_Object_List.begin()->second->Get_Animation_Sign() == 0) is_Already_Timer_Working = false;
				m_instance->m_Object_List.begin()->second->Set_Animation_Sign(-1);
				if (m_instance->m_Curr_Shape == OBJECT_TYPE_SPRING)
					m_instance->m_Object_List.begin()->second->Shape_Change(m_instance->m_Curr_Shape);
				if (!is_Already_Timer_Working) glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Object_Timer, m_instance->m_Object_List.begin()->second->Get_Index());
			}
		}
		break;
	case 'A': 
		if (!m_instance->m_Object_List.empty())
		{
			if (m_instance->m_Object_List.begin()->second->Get_Animation_Sign() != 1)
			{
				bool is_Already_Timer_Working = true;
				if (m_instance->m_Object_List.begin()->second->Get_Animation_Sign() == 0) is_Already_Timer_Working = false;
				m_instance->m_Object_List.begin()->second->Set_Animation_Sign(1);
				if (m_instance->m_Curr_Shape == OBJECT_TYPE_SPRING)
					m_instance->m_Object_List.begin()->second->Shape_Change(m_instance->m_Curr_Shape);
				if (!is_Already_Timer_Working) glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Object_Timer, m_instance->m_Object_List.begin()->second->Get_Index());
			}
		}
		break;

	case 'T':
		if (!m_instance->m_Object_List.empty())
			m_instance->m_Object_List.begin()->second->Set_Animation_Sign(0);
		break;

	case 'Q': glutLeaveMainLoop(); break;
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

void GL_Main::Reshape(const int w, const int h) { glViewport(0, 0, w, h); glMatrixMode(GL_PROJECTION); glOrtho(0.0f, w, 0.0f, h, -400.0f, 400.0f); glutPostRedisplay(); }

void GL_Main::Render_Background() 
{ 
	glViewport(0, 0, INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	glClearColor(0.8f, 0.8f, 1.0f, 1.0f);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex2f(INIT_WINDOW_SIZE_X * 0.5f, 0.0f);
	glVertex2f(INIT_WINDOW_SIZE_X * 0.5f, INIT_WINDOW_SIZE_Y);
	glVertex2f(0.0f, INIT_WINDOW_SIZE_Y * 0.5f);
	glVertex2f(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y * 0.5f);
	glEnd();
}

void GL_Main::Render_Objects()
{
	glViewport(INIT_WINDOW_SIZE_X * 0.125f, INIT_WINDOW_SIZE_Y * 0.125f, INIT_WINDOW_SIZE_X * 0.75f, INIT_WINDOW_SIZE_Y * 0.75f);
	
	for (auto& object : m_instance->m_Object_List)
	{
		if (object.second->Get_is_Activated()) object.second->Draw();
	}
}

void GL_Main::Build_Object(const bool& is_active)
{
	Object* temp_Object;
	Vertex2D temp_Vertex;

	temp_Object = Make_Object(is_active);

	Insert_Object(temp_Object);
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
		p->second->Update();

		if (!p->second->Get_is_Destroyed() && p->second->Get_Animation_Sign() != 0)
			glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Object_Timer, object_num);
		else if (p->second->Get_is_Destroyed()) m_instance->Destroy_Objects_In_List(object_num);
	}
}