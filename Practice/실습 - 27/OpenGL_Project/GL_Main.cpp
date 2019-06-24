#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	
	glutCreateWindow("실습 27");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutDisplayFunc(Render_Scene);
	glutReshapeFunc(Reshape);
	
	Set_Menu();

	Set_Random_Device();
	Init_Objects();
	
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glutKeyboardFunc(Key_Manager);
	//glutSpecialFunc(Special_Key_Manager);
	//glutMouseFunc(Mouse_Manager);
	glutMainLoop();
}

GL_Main::~GL_Main()
{
	Destroy_All_Objects_In_List();
	if (m_Camera != nullptr) { delete m_Camera; m_Camera = nullptr; }
	if (m_Robot != nullptr) { delete m_Robot; m_Robot = nullptr; }
	if (m_Cube != nullptr) { delete m_Cube; m_Cube = nullptr; }
	if (m_Box != nullptr) { delete m_Box; m_Box = nullptr; }
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

		/*
	case 'w': m_instance->m_Camera->XY_Moving(true, CAMERA_MOVE_VALUE); break;
	case 's': m_instance->m_Camera->XY_Moving(true, -CAMERA_MOVE_VALUE); break;
	case 'a': m_instance->m_Camera->XY_Moving(false, -CAMERA_MOVE_VALUE); break;
	case 'd': m_instance->m_Camera->XY_Moving(false, CAMERA_MOVE_VALUE); break;
	*/

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
	case 'r':
		m_instance->m_Camera->Init_Camera();
		m_instance->Destroy_All_Objects_In_List();
		m_instance->Init_Objects();
		m_instance->Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
		break;

	case 27: glutLeaveMainLoop(); break;

	// =================================================


	// =================================================
		// 로봇 명령어

	// 이동 제어
	case 'w': m_instance->m_Robot->Set_Move_Direction(0); break;
	case 's': m_instance->m_Robot->Set_Move_Direction(1); break;
	case 'a': m_instance->m_Robot->Set_Move_Direction(2); break;
	case 'd': m_instance->m_Robot->Set_Move_Direction(3); break;

	// 점프
	case 32: m_instance->m_Robot->Jump(); break;

	// =================================================


	// =================================================
		// 큐브 명령어

	// 인트로 연출 제어
	case 'o': m_instance->m_Cube->Door_Manage(); break;

	// =================================================


	case 'g': m_instance->m_Box->Draw_Switch(); break;
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
			//int index = m_instance->Build_Object();
			//m_instance->Object_Activate(index);
			//m_instance->Get_Object(index)->Set_Translate(x - INIT_WINDOW_SIZE_X * 0.5f, INIT_WINDOW_SIZE_Y * 0.5f - y, 0.0f);
			//m_instance->m_is_Clicked = true;
		}
		else if (state == GLUT_UP) m_instance->m_is_Clicked = false;
		break;
	}
}


void GL_Main::Init_Objects()
{
	m_Camera = new Camera(); // Camera

	m_instance->Object_Activate(m_instance->Build_Object(TREE_OBJECT)); // Tree

	m_instance->Object_Activate(m_instance->Build_Object(RUNNING_MACHINE_OBJECT)); // Running Machine

	m_instance->Object_Activate(m_instance->Build_Object(BAR_OBJECT)); // Bar

	m_instance->Object_Activate(m_instance->Build_Object(BENCH_PRESS_OBJECT)); // Bench Press

	m_instance->Object_Activate(m_instance->Build_Object(ROBOT_OBJECT)); // Robot

	m_instance->Build_Object(FLOOR_OBJECT); // FLOOR

	m_instance->Object_Activate(m_instance->Build_Object(CUBE_OBJECT)); // Cube

	m_instance->Build_Object(BOX_OBJECT); // Test Box
}


void GL_Main::Render_Scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	gluPerspective(60.0f, 1.25f, 1.0f, 5000.0f);
	//if (m_instance->m_is_Perspective) 
	//else glOrtho(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f, 1.0f, 1000.0f);
}

void GL_Main::Render_Background() 
{ 
	glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
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
	Floor* temp_Floor = nullptr;
	Tree* temp_Tree = nullptr;
	Running_Machine* temp_Running_Machine = nullptr;
	Bar* temp_Bar = nullptr;
	Bench_Press* temp_Bench_Press = nullptr;
	Cube* temp_Cube = nullptr;

	switch (type)
	{
	case FLOOR_OBJECT:
		temp_Floor = new Floor();
		temp_Object = reinterpret_cast<Object*>(temp_Floor);
		break;

	case TREE_OBJECT:
		temp_Tree = new Tree();
		temp_Object = reinterpret_cast<Object*>(temp_Tree);
		break;

	case RUNNING_MACHINE_OBJECT:
		temp_Running_Machine = new Running_Machine();
		temp_Object = reinterpret_cast<Object*>(temp_Running_Machine);
		break;

	case BAR_OBJECT:
		temp_Bar = new Bar();
		temp_Object = reinterpret_cast<Object*>(temp_Bar);
		break;

	case BENCH_PRESS_OBJECT:
		temp_Bench_Press = new Bench_Press();
		temp_Object = reinterpret_cast<Object*>(temp_Bench_Press);
		break;

	case ROBOT_OBJECT:
		m_Robot = new Robot();
		temp_Object = reinterpret_cast<Object*>(m_Robot);
		break;

	case CUBE_OBJECT:
		m_Cube = new Cube();
		temp_Object = reinterpret_cast<Object*>(m_Cube);
		break;

	case BOX_OBJECT:
		m_Box = new Box();
		temp_Object = reinterpret_cast<Object*>(m_Box);
		break;
	}

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