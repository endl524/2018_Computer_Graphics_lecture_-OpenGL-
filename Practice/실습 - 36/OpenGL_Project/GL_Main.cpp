#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	
	glutCreateWindow("실습 36");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutDisplayFunc(Render_Scene);
	glutReshapeFunc(Reshape);
	
	GL_Initializing();

	//Set_Menu();

	Set_Random_Device();
	Init_Objects();

	glutKeyboardFunc(Key_Manager);
	glutSpecialFunc(Special_Key_Manager);
	glutSpecialUpFunc(Special_Key_UP_Manager);
	//glutMouseFunc(Mouse_Manager);
	//glutMotionFunc(Mouse_Move_Manager);
	glutMainLoop();
}

GL_Main::~GL_Main()
{
	Reset(false);
}

void GL_Main::Reset(const bool& is_Reset)
{
	Destroy_All_Objects_In_List();
	if (m_Camera != nullptr) { delete m_Camera; m_Camera = nullptr; }
	if (m_Spot_Light_1 != nullptr) { m_Spot_Light_1 = nullptr; }
	if (m_Spot_Light_2 != nullptr) { m_Spot_Light_2 = nullptr; }
	if (m_Pyramid[0] != nullptr) { for (int i = 0; i < 5; ++i) m_Pyramid[i] = nullptr; }
	if (m_Robot != nullptr) { m_Robot = nullptr; }
	if (m_Floor != nullptr) { m_Floor = nullptr; }
	if (is_Reset)
	{
		m_instance->Init_Objects();
		m_instance->Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	}
}

void GL_Main::GL_Initializing()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	float ambient[4]{ 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0f);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0f);

	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
}

void GL_Main::Set_Random_Device()
{
	mt19937_64 seed(m_Random_Device());
	m_Random_SEED = seed;

	uniform_real_distribution<> rand_Color(0.0f, 1.0f);
	m_Random_Color = rand_Color;

	uniform_real_distribution<> rand_Particle_Speed(PARTICLE_SPEED_MIN, PARTICLE_SPEED_MAX);
	m_Random_Particle_Speed = rand_Particle_Speed;

	uniform_real_distribution<> rand_Particle_Direction(PARTICLE_DIR_MIN, PARTICLE_DIR_MAX);
	m_Random_Particle_Direction = rand_Particle_Direction;
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
	//case 32: if (!m_instance->m_is_Perspect) m_instance->m_is_Perspect = true; else m_instance->m_is_Perspect = false; Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y); break;

	case 'r': m_instance->Reset(true); break;

	case 27: glutLeaveMainLoop(); break;

	// =================================================


	// =================================================
		// 조명 명령어
	case '1': m_instance->m_Spot_Light_1->Light_Switch(); break;
	case '2': m_instance->m_Spot_Light_2->Light_Switch(); break;

	case 'y':
		m_instance->m_Spot_Light_1->Rotate_Light(3.0f);
		m_instance->m_Spot_Light_2->Rotate_Light(3.0f);
		break;
	case 'Y':
		m_instance->m_Spot_Light_1->Rotate_Light(-3.0f);
		m_instance->m_Spot_Light_2->Rotate_Light(-3.0f);
		break;

	case 'u':
		reinterpret_cast<Object*>(m_instance->m_Spot_Light_1)->Auto_Moving_Switch();
		reinterpret_cast<Object*>(m_instance->m_Spot_Light_2)->Auto_Moving_Switch();
		break;

	case 'g':
		m_instance->m_Spot_Light_1->Control_Ambient_Color(1);
		m_instance->m_Spot_Light_2->Control_Ambient_Color(1);
		break;
	case 'G':
		m_instance->m_Spot_Light_1->Control_Ambient_Color(-1);
		m_instance->m_Spot_Light_2->Control_Ambient_Color(-1);
		break;

	case 'h':
		m_instance->m_Spot_Light_1->Control_Diffuse_Color(1);
		m_instance->m_Spot_Light_2->Control_Diffuse_Color(1);
		break;
	case 'H':
		m_instance->m_Spot_Light_1->Control_Diffuse_Color(-1);
		m_instance->m_Spot_Light_2->Control_Diffuse_Color(-1);
		break;

	case 'j':
		m_instance->m_Spot_Light_1->Control_Specular_Color(1);
		m_instance->m_Spot_Light_2->Control_Specular_Color(1);
		break;
	case 'J':
		m_instance->m_Spot_Light_1->Control_Specular_Color(-1);
		m_instance->m_Spot_Light_2->Control_Specular_Color(-1);
		break;

	// =================================================

	case 'b': for (int i = 0; i < 5; ++i) m_instance->m_Pyramid[i]->Switching_is_Plane_Normal_On(); break;
	case 'n': m_instance->m_Floor->Switching_Wheathering_Freeze(); break;
	}
}

void GL_Main::Special_Key_Manager(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) m_instance->m_Robot->Set_Move_to_Direction(0);
	if (key == GLUT_KEY_DOWN) m_instance->m_Robot->Set_Move_to_Direction(1);
	if (key == GLUT_KEY_LEFT) m_instance->m_Robot->Set_Move_to_Direction(2);
	if (key == GLUT_KEY_RIGHT) m_instance->m_Robot->Set_Move_to_Direction(3);
}
void GL_Main::Special_Key_UP_Manager(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) m_instance->m_Robot->Set_Move_to_Direction(4);
	if (key == GLUT_KEY_DOWN) m_instance->m_Robot->Set_Move_to_Direction(5);
	if (key == GLUT_KEY_LEFT) m_instance->m_Robot->Set_Move_to_Direction(6);
	if (key == GLUT_KEY_RIGHT) m_instance->m_Robot->Set_Move_to_Direction(7);
}

void GL_Main::Mouse_Manager(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN && !m_instance->m_is_Left_Clicked)
		{
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
			m_instance->m_is_Right_Clicked = true;
		}
		else if (state == GLUT_UP)
		{
			m_instance->m_is_Right_Clicked = false;
		}
		break;
	}
}
void GL_Main::Mouse_Move_Manager(int x, int y)
{
}


void GL_Main::Init_Objects()
{
	m_Camera = new Camera(); // Camera
	
	m_instance->Object_Activate(m_instance->Build_Object(SPOT_LIGHT_1_OBJECT));
	m_instance->Object_Activate(m_instance->Build_Object(SPOT_LIGHT_2_OBJECT));
	m_instance->Object_Activate(m_instance->Build_Object(SPHERE_OBJECT));
	m_instance->Object_Activate(m_instance->Build_Object(ROBOT_OBJECT));
	m_instance->Object_Activate(m_instance->Build_Object(FLOOR_OBJECT));
	m_Floor->Wheather_Initialize(WHEATHER_TYPE_SNOW, 50, false);
	m_Floor->Switching_Wheathering();

	int num = 0;
	for (int i = 0; i < 5; ++i)
	{
		num = m_instance->Build_Object(PYRAMID_OBJECT);
		m_Pyramid[i] = reinterpret_cast<Pyramid*>(m_instance->Get_Object(num));
		m_instance->Object_Activate(num);
	}
	m_Pyramid[0]->Pyramid_Initialize();
	reinterpret_cast<Object*>(m_Pyramid[0])->Make_OBB();
	reinterpret_cast<Object*>(m_Pyramid[1])->Set_Translate(-200.0f, 0.0f, -200.0f);
	m_Pyramid[1]->Pyramid_Initialize();
	reinterpret_cast<Object*>(m_Pyramid[1])->Make_OBB();
	reinterpret_cast<Object*>(m_Pyramid[2])->Set_Translate(-200.0f, 0.0f, 200.0f);
	m_Pyramid[2]->Pyramid_Initialize();
	reinterpret_cast<Object*>(m_Pyramid[2])->Make_OBB();
	reinterpret_cast<Object*>(m_Pyramid[3])->Set_Translate(200.0f, 0.0f, 200.0f);
	m_Pyramid[3]->Pyramid_Initialize();
	reinterpret_cast<Object*>(m_Pyramid[3])->Make_OBB();
	reinterpret_cast<Object*>(m_Pyramid[4])->Set_Translate(200.0f, 0.0f, -200.0f);
	m_Pyramid[4]->Pyramid_Initialize();
	reinterpret_cast<Object*>(m_Pyramid[4])->Make_OBB();
}


void GL_Main::Reshape(const GLsizei w, const GLsizei h)
{ 
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.25f, 1.0f, 3000.0f);
	//if (m_instance->m_is_Perspect) gluPerspective(60.0f, 1.25f, 1.0f, 3000.0f);
	//else glOrtho(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f, -200.0f, 5000.0f);
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


void GL_Main::Render_Background() 
{ 
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
	Pyramid* temp_Pyramid = nullptr;
	Particle_Object* temp_Particle = nullptr;

	switch (type)
	{

	case SPHERE_OBJECT:
		temp_Object = new Object();
		temp_Object->Set_Object_Type(SPHERE_OBJECT);
		temp_Object->Set_Translate(100.0f, OBJECT_SIZE * 2.5f, 0.0f);
		temp_Object->Set_Scale(2.0f, 2.0f, 2.0f);
		temp_Object->Set_Material_Ambient_Color(0.15f, 0.15f, 0.15f, 1.0f);
		temp_Object->Set_Material_Diffuse_Color(1.0f, 1.0f, 1.0f, 1.0f);
		temp_Object->Set_Material_Specular_Color(1.0f, 1.0f, 1.0f, 1.0f);
		temp_Object->Set_is_Static(true);
		break;


	case FLOOR_OBJECT:
		m_Floor = new Floor(FLOOR_WIDTH, FLOOR_HEIGHT, FLOOR_PIECE_WIDTH_SIZE, FLOOR_PIECE_HEIGHT_SIZE);
		reinterpret_cast<Object*>(m_Floor)->Set_Object_Type(FLOOR_OBJECT);
		reinterpret_cast<Object*>(m_Floor)->Set_Translate(0.0f, -OBJECT_SIZE, 0.0f);
		reinterpret_cast<Object*>(m_Floor)->Set_is_Static(true);
		temp_Object = reinterpret_cast<Object*>(m_Floor);
		break;


	case PYRAMID_OBJECT:
		temp_Pyramid = new Pyramid();
		reinterpret_cast<Object*>(temp_Pyramid)->Set_Object_Type(PYRAMID_OBJECT);
		reinterpret_cast<Object*>(temp_Pyramid)->Set_Material_Ambient_Color(0.2f, 0.2f, 0.2f, 1.0f);
		reinterpret_cast<Object*>(temp_Pyramid)->Set_Material_Diffuse_Color(1.0f, 1.0f, 1.0f, 1.0f);
		reinterpret_cast<Object*>(temp_Pyramid)->Set_Material_Specular_Color(1.0f, 1.0f, 1.0f, 1.0f);
		reinterpret_cast<Object*>(temp_Pyramid)->Set_is_Static(false);
		temp_Object = reinterpret_cast<Object*>(temp_Pyramid);
		break;


	case SPOT_LIGHT_1_OBJECT:
		m_Spot_Light_1 = new Light_Object(0);
		if (!m_Spot_Light_1->Get_is_invalid_Light_Object())
		{
			reinterpret_cast<Object*>(m_Spot_Light_1)->Set_Object_Type(SPOT_LIGHT_1_OBJECT);
			reinterpret_cast<Object*>(m_Spot_Light_1)->Set_Translate(150.0f, OBJECT_SIZE * 5.0f, 0.0f);
			reinterpret_cast<Object*>(m_Spot_Light_1)->Yaw(-90.0f);
			reinterpret_cast<Object*>(m_Spot_Light_1)->Roll(-45.0f);
			m_Spot_Light_1->Set_Material_Ambient_Color(0.2f, 0.2f, 0.2f, 1.0f);
			m_Spot_Light_1->Set_Material_Diffuse_Color(1.0f, 0.0f, 0.0f, 1.0f);
			m_Spot_Light_1->Set_Material_Specular_Color(1.0f, 1.0f, 1.0f, 1.0f);
			m_Spot_Light_1->Set_Light_Position(0.0f, 0.0f, 0.0f, 1.0f);
			m_Spot_Light_1->Set_Light_Spot_CutOff(30.0f);
			m_Spot_Light_1->Set_Light_Spot_Exponent(0.0f);
			reinterpret_cast<Object*>(m_Spot_Light_1)->Set_is_Static(true);

			m_Spot_Light_1->Light_On();
			temp_Object = reinterpret_cast<Object*>(m_Spot_Light_1);
		}
		else
		{
			delete m_Spot_Light_1; m_Spot_Light_1 = nullptr; return -1;
		}
		break;


	case SPOT_LIGHT_2_OBJECT:
		m_Spot_Light_2 = new Light_Object(1);
		if (!m_Spot_Light_2->Get_is_invalid_Light_Object())
		{
			reinterpret_cast<Object*>(m_Spot_Light_2)->Set_Object_Type(SPOT_LIGHT_2_OBJECT);
			reinterpret_cast<Object*>(m_Spot_Light_2)->Set_Translate(-150.0f, OBJECT_SIZE * 5.0f, 0.0f);
			reinterpret_cast<Object*>(m_Spot_Light_2)->Yaw(90.0f);
			reinterpret_cast<Object*>(m_Spot_Light_2)->Roll(45.0f);
			m_Spot_Light_2->Set_Material_Ambient_Color(0.2f, 0.2f, 0.2f, 1.0f);
			m_Spot_Light_2->Set_Material_Diffuse_Color(0.0f, 0.0f, 1.0f, 1.0f);
			m_Spot_Light_2->Set_Material_Specular_Color(1.0f, 1.0f, 1.0f, 1.0f);
			m_Spot_Light_2->Set_Light_Position(0.0f, 0.0f, 0.0f, 1.0f);
			m_Spot_Light_2->Set_Light_Spot_CutOff(30.0f);
			m_Spot_Light_2->Set_Light_Spot_Exponent(1.0f);
			reinterpret_cast<Object*>(m_Spot_Light_1)->Set_is_Static(true);

			m_Spot_Light_2->Light_On();
			temp_Object = reinterpret_cast<Object*>(m_Spot_Light_2);
		}
		else
		{
			delete m_Spot_Light_2; m_Spot_Light_2 = nullptr; return -1;
		}
		break;

	case ROBOT_OBJECT:
		m_Robot = new Robot();
		temp_Object = reinterpret_cast<Object*>(m_Robot);
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