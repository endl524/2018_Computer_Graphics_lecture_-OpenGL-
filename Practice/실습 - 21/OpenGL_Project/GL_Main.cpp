#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	
	glutCreateWindow("실습 21");

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

	m_instance->Object_Activate(m_instance->Build_Object(TREE_OBJECT)); // Tree

	m_instance->Object_Activate(m_instance->Build_Object(RUNNING_MACHINE_OBJECT)); // Running Machine

	m_instance->Object_Activate(m_instance->Build_Object(BAR_OBJECT)); // Bar

	m_instance->Object_Activate(m_instance->Build_Object(BENCH_PRESS_OBJECT)); // Bench Press

	m_instance->Object_Activate(m_instance->Build_Object(CRANE_OBJECT)); // Crane

	m_instance->Object_Activate(m_instance->Build_Object(SPHERE_OBJECT)); // Sphere

	m_instance->Build_Object(FLOOR_OBJECT); // FLOOR
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
		// 크레인 명령어

		// 하단
	case 'm': m_instance->m_Crane->Yaw(-9.0f); break;
	case 'M': m_instance->m_Crane->Yaw(9.0f); break;

		// 중단
	case 'k': m_instance->m_Crane->Middle_Rotate_X(9.0f); break;
	case 'K': m_instance->m_Crane->Middle_Rotate_X(-9.0f); break;
	case 'l': m_instance->m_Crane->Middle_Rotate_Y(-9.0f); break;
	case 'L': m_instance->m_Crane->Middle_Rotate_Y(9.0f); break;

		// 상단
	case 'o': m_instance->m_Crane->Upper_Rotate_X(9.0f); break;
	case 'O': m_instance->m_Crane->Upper_Rotate_X(-9.0f); break;
	case 'p': m_instance->m_Crane->Upper_Rotate_Z(-9.0f); break;
	case 'P': m_instance->m_Crane->Upper_Rotate_Z(9.0f); break;

		// 이동 제어
	case 'b': m_instance->m_Crane->Set_Move_Direction(0); break;
	case 'B': m_instance->m_Crane->Set_Move_Direction(1); break;

	case 'n': m_instance->m_Crane->Set_Move_Direction(2); break;
	case 'N': m_instance->m_Crane->Set_Move_Direction(3); break;
	// =================================================


	// =================================================
		// 구 명령어

		// 이동 제어
	case 'g': m_instance->m_Sphere->Set_Move_Direction(0); break;
	case 'G': m_instance->m_Sphere->Set_Move_Direction(1); break;

	case 'h': m_instance->m_Sphere->Set_Move_Direction(2); break;
	case 'H': m_instance->m_Sphere->Set_Move_Direction(3); break;

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

	gluPerspective(60.0f, w / h, 1.0f, 1500.0f);
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
	Tree* temp_Tree = nullptr;
	Running_Machine* temp_Running_Machine = nullptr;
	Bar* temp_Bar = nullptr;
	Bench_Press* temp_Bench_Press = nullptr;

	switch (type)
	{
	case FLOOR_OBJECT:
		temp_Object = new Object();
		temp_Object->Set_Color(1.0f, 1.0f, 0.5f, 1.0f);
		temp_Object->Set_Translate(0.0f, 0.0f, 0.0f);
		temp_Object->Set_Scale(20.0f, 0.02f, 20.0f);
		temp_Object->Set_Object_Type(FLOOR_OBJECT);
		temp_Object->Set_is_Static(true);
		break;

	case TREE_OBJECT:
		temp_Tree = new Tree();
		temp_Object = reinterpret_cast<Object*>(temp_Tree);
		temp_Object->Set_is_Static(true);
		break;

	case RUNNING_MACHINE_OBJECT:
		temp_Running_Machine = new Running_Machine();
		temp_Object = reinterpret_cast<Object*>(temp_Running_Machine);
		temp_Object->Set_is_Static(true);
		break;

	case BAR_OBJECT:
		temp_Bar = new Bar();
		temp_Object = reinterpret_cast<Object*>(temp_Bar);
		temp_Object->Set_is_Static(true);
		break;

	case BENCH_PRESS_OBJECT:
		temp_Bench_Press = new Bench_Press();
		temp_Object = reinterpret_cast<Object*>(temp_Bench_Press);
		temp_Object->Set_is_Static(true);
		break;

	case CRANE_OBJECT:
		m_Crane = new Crane();
		temp_Object = reinterpret_cast<Object*>(m_Crane);
		temp_Object->Set_is_Static(false);
		break;

	case SPHERE_OBJECT:
		m_Sphere = new Object();
		m_Sphere->Set_Object_Type(SPHERE_OBJECT);
		m_Sphere->Set_Color(1.0f, 0.0f, 0.0f, 1.0f);
		m_Sphere->Set_Scale(3.0f, 3.0f, 3.0f);
		m_Sphere->Set_Translate(0.0f, OBJECT_SIZE * 0.5f * m_Sphere->Get_Scale_Y(), 0.0f);
		m_Sphere->Set_Move_Direction(3);
		m_Sphere->Set_is_Static(false);
		temp_Object = m_Sphere;
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
				float* my_col_Box = p->second->Get_Collider_Box();
				float* temp_col_Box;
				for (auto& object : m_instance->m_Object_List)
				{
					if (object != *p && object.second->Get_is_Activated())
					{
						temp_col_Box = object.second->Get_Collider_Box();
						if (my_col_Box[0] <= temp_col_Box[1] && my_col_Box[1] >= temp_col_Box[0])
						{
							if (my_col_Box[2] <= temp_col_Box[3] && my_col_Box[3] >= temp_col_Box[2])
							{
								p->second->Collision_Procedure();
								object.second->Collision_Procedure();
							}
						}
					}
				}
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