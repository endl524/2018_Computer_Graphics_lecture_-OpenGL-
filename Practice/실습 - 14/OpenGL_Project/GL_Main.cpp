#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	glutCreateWindow("½Ç½À 14");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutDisplayFunc(Render_Scene);
	glutReshapeFunc(Reshape);

	glutKeyboardFunc(Key_Manager);
	glutMouseFunc(Mouse_Manager);
	glutMainLoop();
}

GL_Main::~GL_Main()
{
	Destroy_All_Objects_In_List();
	if (m_Object_World_Controller != nullptr) delete m_Object_World_Controller;
}

void GL_Main::Init_Objects()
{
	int index = m_instance->Build_Object(true);
	m_instance->Object_Activate(index);
}

void GL_Main::Key_Manager(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
	case 'R':
		for (auto& object : m_instance->m_Object_List)
			object.second->Reverse_Rotate_Direction();
		break;

	case 27: glutLeaveMainLoop(); break;
	}
}

void GL_Main::Mouse_Manager(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && !m_instance->m_is_Clicked)
		{
			int index = m_instance->Build_Object(true);
			m_instance->Object_Activate(index);
			m_instance->Get_Object(index)->Set_Local_Translate(x - INIT_WINDOW_SIZE_X * 0.5f, INIT_WINDOW_SIZE_Y * 0.5f - y, 0.0f);
			m_instance->m_is_Clicked = true;
		}
		else if (state == GLUT_UP) m_instance->m_is_Clicked = false;
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