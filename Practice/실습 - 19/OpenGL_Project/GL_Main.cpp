#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	
	glutCreateWindow("½Ç½À 19");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutDisplayFunc(Render_Scene);
	glutReshapeFunc(Reshape);
	//Set_Menu();

	Set_Random_Device();
	Init_Objects();

	glutKeyboardFunc(Key_Manager);
	//glutSpecialFunc(Special_Key_Manager);
	//glutMouseFunc(Mouse_Manager);
	glutMainLoop();
}

GL_Main::~GL_Main()
{
	Destroy_All_Objects_In_List();
	if (m_Camera != nullptr) { delete m_Camera; m_Camera = nullptr; }
	if (m_Sphere != nullptr) { delete m_Sphere; m_Sphere = nullptr; }
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
	//int subMenu1 = glutCreateMenu(MenuFunc);
	//glutAddMenuEntry("Solid", 1);

	int mainMenu = glutCreateMenu(MenuFunc);
	//glutAddSubMenu("Draw_Mode", subMenu1);
	glutAddMenuEntry("Exit", 999);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void GL_Main::MenuFunc(int button)
{
	switch (button)
	{
	case 999: glutLeaveMainLoop(); break;
	}
}

void GL_Main::Init_Objects()
{
	m_Camera = new Camera(); // Camera

	int index = 0;

	index = m_instance->Build_Object(); // Y-Axis
	m_instance->Get_Object(index)->Set_Object_Type(OBJECT_TYPE_CUBE);
	m_instance->Get_Object(index)->Set_Color(0.0f, 1.0f, 0.0f, 1.0f);
	m_instance->Get_Object(index)->Set_Scale(0.02f, 2.0f, 0.02f);
	m_instance->Get_Object(index)->Set_Translate(0.0f, OBJECT_SIZE * 0.5f, 0.0f);
	m_instance->Get_Object(index)->Set_is_Solid_Shape(true);

	index = m_instance->Build_Object(); // Plane
	m_instance->Get_Object(index)->Set_Object_Type(OBJECT_TYPE_CUBE);
	m_instance->Get_Object(index)->Set_Color(1.0f, 1.0f, 0.9f, 1.0f);
	m_instance->Get_Object(index)->Set_Translate(0.0f, -OBJECT_SIZE * 0.5f, 0.0f);
	m_instance->Get_Object(index)->Set_Scale(5.0f, 0.02f, 5.0f);
	m_instance->Get_Object(index)->Set_is_Solid_Shape(true);

	m_instance->m_Sphere = new Object(); // Center Sphere
	m_instance->m_Sphere->Set_Object_Type(OBJECT_TYPE_SPHERE);
	m_instance->m_Sphere->Set_Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_instance->m_Sphere->Set_is_Solid_Shape(false);
}

void GL_Main::Key_Manager(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x': m_instance->m_Camera->Set_Rotation_Value(0, 3.0f); break;
	case 'X': m_instance->m_Camera->Set_Rotation_Value(0, -3.0f); break;
	case 'y': m_instance->m_Camera->Set_Rotation_Value(1, 3.0f); break;
	case 'Y': m_instance->m_Camera->Set_Rotation_Value(1, -3.0f); break;
	case 'z': m_instance->m_Camera->Set_Rotation_Value(2, 3.0f); break;
	case 'Z': m_instance->m_Camera->Set_Rotation_Value(2, -3.0f); break;

	case 'i': 
		m_instance->m_Camera->Init_Camera();
		m_instance->Destroy_All_Objects_In_List();
		m_instance->Init_Objects();
		m_instance->m_is_Perspective = true;
		m_instance->Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
		break;

	case '+': m_instance->m_Camera->Forward_Moving(-CAMERA_MOVE_VALUE); break;
	case '-': m_instance->m_Camera->Forward_Moving(CAMERA_MOVE_VALUE); break;

	case 'w': m_instance->m_Camera->XY_Moving(true, CAMERA_MOVE_VALUE); break;
	case 's': m_instance->m_Camera->XY_Moving(true, -CAMERA_MOVE_VALUE); break;
	case 'a': m_instance->m_Camera->XY_Moving(false, -CAMERA_MOVE_VALUE); break;
	case 'd': m_instance->m_Camera->XY_Moving(false, CAMERA_MOVE_VALUE); break;

	case 'l': m_instance->m_Sphere->Move(0, -3.0f); break;
	case 'L': m_instance->m_Sphere->Move(0, 3.0f); break;

	case 'm': m_instance->m_Sphere->Move(1, -3.0f); break;
	case 'M': m_instance->m_Sphere->Move(1, 3.0f); break;

	case 'n': m_instance->m_Sphere->Move(2, -3.0f); break;
	case 'N': m_instance->m_Sphere->Move(2, 3.0f); break;

	

	case 27: glutLeaveMainLoop(); break;
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
		if (state == GLUT_DOWN && !m_instance->m_is_Clicked)
		{
			int index = m_instance->Build_Object();
			m_instance->Object_Activate(index);
			m_instance->Get_Object(index)->Set_Translate(x - INIT_WINDOW_SIZE_X * 0.5f, INIT_WINDOW_SIZE_Y * 0.5f - y, 0.0f);
			m_instance->m_is_Clicked = true;
		}
		else if (state == GLUT_UP) m_instance->m_is_Clicked = false;
		break;
	}
}

void GL_Main::Render_Scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	m_instance->m_Camera->Work();
	m_instance->Render_Background();
	m_instance->Render_Objects();

	glutSwapBuffers();
	glutPostRedisplay();
}

void GL_Main::Reshape(const int w, const int h) 
{ 
	glViewport(0, 0, w, h); 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60.0f, w / h, 1.0f, 800.0f);
	//if (m_instance->m_is_Perspective) 
	//else glOrtho(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f, 1.0f, 1000.0f);
}

void GL_Main::Render_Background() 
{ 
	glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
}

void GL_Main::Render_Objects()
{
	for (auto& object : m_instance->m_Object_List) object.second->Draw();
	m_instance->m_Sphere->Draw();
}

int GL_Main::Build_Object()
{
	Object* temp_Object;
	Vertex3D temp_Vertex;

	temp_Object = Make_Object();

	Insert_Object(temp_Object);

	return temp_Object->Get_Index();
}

Object* GL_Main::Make_Object()
{
	Object* temp_Object = new Object();
	
	return temp_Object;
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