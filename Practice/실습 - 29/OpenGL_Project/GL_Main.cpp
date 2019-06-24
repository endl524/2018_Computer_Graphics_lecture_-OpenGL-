#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	
	glutCreateWindow("실습 29");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutDisplayFunc(Render_Scene);
	glutReshapeFunc(Reshape);
	
	//Set_Menu();

	Set_Random_Device();
	Init_Objects();
	
	//glEnable(GL_DEPTH_TEST);
	//glShadeModel(GL_SMOOTH);

	glutKeyboardFunc(Key_Manager);
	//glutSpecialFunc(Special_Key_Manager);
	glutMouseFunc(Mouse_Manager);
	glutMotionFunc(Mouse_Move_Manager);
	glutMainLoop();
}

GL_Main::~GL_Main()
{
	Destroy_All_Objects_In_List();
	if (m_Camera != nullptr) { delete m_Camera; m_Camera = nullptr; }
	if (m_Object != nullptr) { m_Object = nullptr; }
}

void GL_Main::Set_Random_Device()
{
	mt19937_64 seed(m_Random_Device());
	m_Random_SEED = seed;

	uniform_real_distribution<> rand_Color(0.0f, 1.0f);
	m_Random_Color = rand_Color;

	uniform_int_distribution<> rand_Speed(OBJECT_SPEED_MIN, OBJECT_SPEED_MAX);
	m_Random_Speed = rand_Speed;
}

void GL_Main::Set_Menu()
{
	int subMenu0 = glutCreateMenu(MenuFunc);
	glutAddMenuEntry("On", 0);
	glutAddMenuEntry("Off", 1);

	int subMenu1 = glutCreateMenu(MenuFunc);
	glutAddMenuEntry("On", 2);
	glutAddMenuEntry("Off", 3);

	int subMenu2 = glutCreateMenu(MenuFunc);
	glutAddMenuEntry("Flat", 4);
	glutAddMenuEntry("Smooth", 5);

	int mainMenu = glutCreateMenu(MenuFunc);
	glutAddSubMenu("Depth Test", subMenu0);
	glutAddSubMenu("Culling", subMenu1);
	glutAddSubMenu("Shading", subMenu2);
	glutAddMenuEntry("Exit", 999);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void GL_Main::MenuFunc(int button)
{
	switch (button)
	{
	case 0: glEnable(GL_DEPTH_TEST); break;
	case 1: glDisable(GL_DEPTH_TEST); break;
	case 2: glEnable(GL_CULL_FACE); break;
	case 3: glDisable(GL_CULL_FACE); break;
	case 4: glShadeModel(GL_FLAT); break;
	case 5: glShadeModel(GL_SMOOTH); break;

	case 999: glutLeaveMainLoop(); break;
	}
}

void GL_Main::Key_Manager(unsigned char key, int x, int y)
{
	switch (key)
	{
	// =================================================
		// 카메라 명령어
		
	case 'x': m_instance->m_Camera->Set_Rotation_Value(0, 3.0f); break;
	case 'X': m_instance->m_Camera->Set_Rotation_Value(0, -3.0f); break;
	case 'c': m_instance->m_Camera->Set_Rotation_Value(1, 3.0f); break;
	case 'C': m_instance->m_Camera->Set_Rotation_Value(1, -3.0f); break;
	case 'z': m_instance->m_Camera->Set_Rotation_Value(2, 3.0f); break;
	case 'Z': m_instance->m_Camera->Set_Rotation_Value(2, -3.0f); break;

	case '+': m_instance->m_Camera->Forward_Moving(-CAMERA_MOVE_VALUE); break;
	case '-': m_instance->m_Camera->Forward_Moving(CAMERA_MOVE_VALUE); break;

		
	case 'w': m_instance->m_Camera->XY_Moving(true, CAMERA_MOVE_VALUE); break;
	case 's': m_instance->m_Camera->XY_Moving(true, -CAMERA_MOVE_VALUE); break;
	case 'a': m_instance->m_Camera->XY_Moving(false, -CAMERA_MOVE_VALUE); break;
	case 'd': m_instance->m_Camera->XY_Moving(false, CAMERA_MOVE_VALUE); break;
	

	case 'v':
		if (!m_instance->m_Camera->Get_is_Zoomming_to_Main())
		{
			m_instance->m_Camera->Set_is_Zoomming_to_Main(true);
			glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Camera_Zoom_to_Main_Timer, -10);
		}
		break;

	// =================================================


	// =================================================
		// 시스템 명령어
	case 32: if (!m_instance->m_is_Perspect) m_instance->m_is_Perspect = true; else m_instance->m_is_Perspect = false; Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y); break;

	case 'm': m_instance->m_Object->Switch_is_Solid(); break;
	case 'n': 
		m_instance->m_Object->Auto_Moving_Switch(); 
		
		break;

	case 'r':
		m_instance->Destroy_All_Objects_In_List();
		if (m_instance->m_Camera != nullptr) { delete m_instance->m_Camera; m_instance->m_Camera = nullptr; }
		m_instance->Init_Objects();
		m_instance->Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
		break;

	case 27: glutLeaveMainLoop(); break;

	// =================================================

	}
}

void GL_Main::Special_Key_Manager(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_UP: m_instance->m_Camera->Set_Rotation_Value(0, -3.0f); break;
	case GLUT_KEY_DOWN: m_instance->m_Camera->Set_Rotation_Value(0, 3.0f); break;
	case GLUT_KEY_LEFT: m_instance->m_Camera->Set_Rotation_Value(2, -3.0f); break;
	case GLUT_KEY_RIGHT: m_instance->m_Camera->Set_Rotation_Value(2, 3.0f); break;
	}
}

void GL_Main::Mouse_Manager(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && !m_instance->m_is_Left_Clicked)
		{
			m_instance->m_Object->Insert_Control_Point(x - INIT_WINDOW_SIZE_X * 0.5f, y - INIT_WINDOW_SIZE_Y * 0.5f);
			m_instance->m_Object->Insert_Polygon_Vertex(x - INIT_WINDOW_SIZE_X * 0.5f, y - INIT_WINDOW_SIZE_Y * 0.5f);
			m_instance->m_is_Left_Clicked = true;
		}
		else if (state == GLUT_UP)
		{
			m_instance->m_is_Left_Clicked = false;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN && !m_instance->m_is_Right_Clicked)
		{
			m_instance->m_Selected_Point_index = m_instance->m_Object->Find_Near_CP(x - INIT_WINDOW_SIZE_X * 0.5f, INIT_WINDOW_SIZE_Y * 0.5f - y);
			m_instance->m_is_Right_Clicked = true;
		}
		else if (state == GLUT_UP)
		{
			m_instance->m_Selected_Point_index.x = -1;
			m_instance->m_Selected_Point_index.y = -1;
			m_instance->m_is_Right_Clicked = false;
		}
		break;
	}
}

void GL_Main::Mouse_Move_Manager(int x, int y)
{
	if (m_instance->m_is_Right_Clicked && m_instance->m_Selected_Point_index.x != -1 && m_instance->m_Selected_Point_index.y != -1)
	{
		m_instance->m_Object->CP_Move(m_instance->m_Selected_Point_index, x - INIT_WINDOW_SIZE_X * 0.5f, INIT_WINDOW_SIZE_Y * 0.5f - y);
	}
}


void GL_Main::Init_Objects()
{
	m_Camera = new Camera(); // Camera
	m_instance->Object_Activate(m_instance->Build_Object(0));
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 7; ++j)
		{
			m_instance->m_Object->Set_Random_Speed(i, j, m_instance->m_Random_Speed(m_instance->m_Random_SEED) + 1);
		}
	}
}


void GL_Main::Render_Scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (m_instance->m_is_Perspect) m_instance->m_Camera->Work();
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	m_instance->Render_Background();
	m_instance->Render_Objects();

	glutSwapBuffers();
	glutPostRedisplay();
}

void GL_Main::Reshape(const GLsizei w, const GLsizei h)
{ 
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (m_instance->m_is_Perspect) gluPerspective(60.0f, 1.25f, 1.0f, 3000.0f);
	else glOrtho(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f, -200.0f, 5000.0f);
}

void GL_Main::Render_Background() 
{ 
	glClearColor(0.8f, 0.8f, 1.0f, 1.0f);

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1000.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
	glPopMatrix();
}

void GL_Main::Render_Objects()
{
	for (auto& object : m_instance->m_Object_List)
	{
		object.second->Draw();
	}
}

int GL_Main::Build_Object(const int& type)
{
	Object* temp_Object = nullptr;
	
	/*
	switch (type)
	{
		
	case BOX_OBJECT:
		m_Box = new Box();
		temp_Object = reinterpret_cast<Object*>(m_Box);
		break;
		
	}
	*/

	temp_Object = new Object(true);
	m_Object = temp_Object;

	if (temp_Object != nullptr)
	{
		Insert_Object(temp_Object);
		return temp_Object->Get_Index();
	}
	else return -1;
}

void GL_Main::Destroy_All_Objects_In_List()
{
	if (!m_Object_List.empty())
	{
		if (m_Object_List[0]->Get_Object_Count() >= 1000000) m_Object_List[0]->Set_Object_Count(0);
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
			if (m_Object_List.size() == 1) if (m_Object_List[0]->Get_Object_Count() >= 1000000) m_Object_List[0]->Set_Object_Count(0);
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

				for (auto& object : m_instance->m_Object_List)
				{
					if (object != *p) p->second->Collision_Check(object.second);
				}
				glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Object_Timer, object_num);
			}
		}
		else if (p->second->Get_is_Destroyed()) m_instance->Destroy_Objects_In_List(object_num);
	}
}

void GL_Main::Start_Camera_Zoom_to_Main_Timer(const int object_num)
{
	if (!m_instance->m_Camera->Zoom_to_Main())
		glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Camera_Zoom_to_Main_Timer, object_num);
	else m_instance->m_Camera->Set_is_Zoomming_to_Main(false);
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