#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	
	glutCreateWindow("컴그숙제2_장동필");

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
	glutMouseFunc(Mouse_Manager);
	glutMotionFunc(Mouse_Move_Manager);
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
	if (m_Robot != nullptr) { m_Robot = nullptr; }
	if (m_Floor != nullptr) { m_Floor = nullptr; }
	if (m_Directional_Light != nullptr) { m_Directional_Light = nullptr; }
	if (is_Reset)
	{
		m_instance->Init_Objects();
		m_instance->m_Camera->Set_is_FPS(false);
		m_instance->m_is_Perspect = false;
		m_instance->m_Camera->Top_View();
		Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
		m_instance->m_Robot->Release_Camera();
	}
}

void GL_Main::GL_Initializing()
{
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	glEnable(GL_LIGHTING);
	float ambient[4]{ 0.6f, 0.6f, 0.6f, 1.0f };
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

	uniform_real_distribution<> rand_Pos_x((-FLOOR_WIDTH + 3) * FLOOR_PIECE_WIDTH_SIZE, (FLOOR_WIDTH - 3) * FLOOR_PIECE_WIDTH_SIZE);
	m_Random_Position_X = rand_Pos_x;

	uniform_real_distribution<> rand_Pos_z((-FLOOR_HEIGHT + 3) * FLOOR_PIECE_HEIGHT_SIZE, (FLOOR_HEIGHT - 3) * FLOOR_PIECE_HEIGHT_SIZE);
	m_Random_Position_Z = rand_Pos_z;

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

	case 'x': m_instance->m_Camera->Set_Rotation_Value(0, 6.0f); break;
	case 'X': m_instance->m_Camera->Set_Rotation_Value(0, -6.0f); break;
	case 'c': m_instance->m_Camera->Set_Rotation_Value(1, 6.0f); break;
	case 'C': m_instance->m_Camera->Set_Rotation_Value(1, -6.0f); break;
	case 'z': m_instance->m_Camera->Set_Rotation_Value(2, 6.0f); break;
	case 'Z': m_instance->m_Camera->Set_Rotation_Value(2, -6.0f); break;

		//case 'q': m_instance->m_Camera->Yaw(-5.0f); break;
		//case 'e': m_instance->m_Camera->Yaw(5.0f); break;

	case 'w': m_instance->m_Camera->Forward_Moving(1); break;
	case 's': m_instance->m_Camera->Forward_Moving(-1); break;

	case 'a': m_instance->m_Camera->Right_Moving(1); break;
	case 'd': m_instance->m_Camera->Right_Moving(-1); break;


	case 'v':
		if (m_instance->m_Camera->Get_is_Free_View() && m_instance->m_Camera->Get_Target() == nullptr)
			if (!m_instance->m_Camera->Get_is_Zoomming_to_Main())
			{
				m_instance->m_Camera->Set_is_Zoomming_to_Main(true);
				glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, Start_Camera_Zoom_to_Main_Timer, -10);
			}
		break;

	case '1': 
		if (m_instance->m_Roller_Coaster->Get_is_CP_Making_Over())
		{
			m_instance->m_Camera->Set_is_FPS(true);
			m_instance->m_is_Perspect = true;
			m_instance->m_Camera->Free_View();
			Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
			m_instance->m_Robot->Set_Camera(m_instance->m_Camera);
		}
		break;
	case '2':
		if (m_instance->m_Roller_Coaster->Get_is_CP_Making_Over())
		{
			m_instance->m_Camera->Set_is_FPS(false);
			m_instance->m_is_Perspect = true;
			m_instance->m_Camera->Free_View();
			Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
			m_instance->m_Robot->Set_Camera(m_instance->m_Camera);
		}
		break;
	case '3': 
		if (m_instance->m_Roller_Coaster->Get_is_CP_Making_Over())
		{
			m_instance->m_Camera->Set_is_FPS(false);
			m_instance->m_is_Perspect = true;
			m_instance->m_Camera->Free_View();
			Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
			m_instance->m_Robot->Release_Camera();
		}
		break;
	case '4': 
		if (m_instance->m_Roller_Coaster->Get_is_CP_Making_Over())
		{
			m_instance->m_Camera->Set_is_FPS(true);
			m_instance->m_is_Perspect = true;
			m_instance->m_Camera->Free_View();
			Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
			m_instance->m_Roller_Coaster->Set_Camera(m_instance->m_Camera);
		}
		break;

	case '5':
		if (!m_instance->m_Roller_Coaster->Get_is_CP_Making_Over())
		{
			m_instance->m_Camera->Set_is_FPS(false);
			m_instance->m_is_Perspect = false;
			m_instance->m_Camera->Top_View();
			Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
			m_instance->m_Robot->Release_Camera();
		}
		break;
	case '6':
		if (!m_instance->m_Roller_Coaster->Get_is_CP_Making_Over())
		{
			m_instance->m_Camera->Set_is_FPS(false);
			m_instance->m_is_Perspect = false;
			Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
			m_instance->m_Camera->Front_View();
			m_instance->m_Robot->Release_Camera();
		}
		break;

		// =================================================


		// =================================================
			// 시스템 명령어
	case 'r': m_instance->Reset(true); break;

	case 27: glutLeaveMainLoop(); break;

		// =================================================


		// =================================================
			// 기타 명령어

	case 'p': m_instance->m_Directional_Light->Light_Switch(); break;

	case 'b': m_instance->m_Floor->Wheather_Initialize(WHEATHER_TYPE_SNOW, 70, false); break;
	case 'B': m_instance->m_Floor->Wheather_Initialize(WHEATHER_TYPE_RAIN, 150, false); break;
	case 'n': m_instance->m_Floor->Switching_Wheathering_Freeze(); break;
	case 'm': m_instance->m_Floor->Switching_is_Wheathering(); break;

	case '[': m_instance->m_Robot->Yaw(-5.0f); break;
	case ']': m_instance->m_Robot->Yaw(5.0f); break;

	case 'g':
		m_instance->m_Roller_Coaster->Set_is_CP_Making_Over(true);
		if (m_instance->m_Roller_Coaster->Get_is_CP_Making_Over())
		{
			m_instance->m_is_Perspect = true;
			m_instance->m_Camera->Free_View();
			Reshape(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
			m_instance->m_Camera->Zoom_to_Main();
			reinterpret_cast<Object*>(m_instance->m_Robot_2)->Forward_Move_On();
		}
		break;

	case 32:
		if (m_instance->m_Roller_Coaster->Get_is_CP_Making_Over())
		{
			if (m_instance->m_Robot->Get_is_Shootable() && !m_instance->m_Bullet_Pool.empty())
			{
				m_instance->m_Robot->Shoot();
				Bullet* temp_Bullet = m_instance->m_Bullet_Pool.front();
				temp_Bullet->Bullet_Load(reinterpret_cast<Object*>(m_instance->m_Robot));
				m_instance->m_Bullet_Pool.pop();
			}
		}
		break;
	}
}

void GL_Main::Special_Key_Manager(int key, int x, int y)
{
	if (m_instance->m_Roller_Coaster->Get_is_CP_Making_Over())
	{
		if (key == GLUT_KEY_UP) { reinterpret_cast<Object*>(m_instance->m_Robot)->Forward_Move_On(); }
		if (key == GLUT_KEY_DOWN) { reinterpret_cast<Object*>(m_instance->m_Robot)->Backward_Move_On(); }
		if (key == GLUT_KEY_LEFT) { reinterpret_cast<Object*>(m_instance->m_Robot)->Left_Move_On(); }
		if (key == GLUT_KEY_RIGHT) { reinterpret_cast<Object*>(m_instance->m_Robot)->Right_Move_On(); }
	}
}
void GL_Main::Special_Key_UP_Manager(int key, int x, int y)
{
	if (m_instance->m_Roller_Coaster->Get_is_CP_Making_Over())
	{
		if (key == GLUT_KEY_UP) { reinterpret_cast<Object*>(m_instance->m_Robot)->Forward_Move_Off(); }
		if (key == GLUT_KEY_DOWN) { reinterpret_cast<Object*>(m_instance->m_Robot)->Backward_Move_Off(); }
		if (key == GLUT_KEY_LEFT) { reinterpret_cast<Object*>(m_instance->m_Robot)->Left_Move_Off(); }
		if (key == GLUT_KEY_RIGHT) { reinterpret_cast<Object*>(m_instance->m_Robot)->Right_Move_Off(); }
	}
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
			if (m_instance->m_Camera->Get_is_Top_View())
			{
				m_instance->m_Roller_Coaster->Insert_CP((x - INIT_WINDOW_SIZE_X * 0.5f) * 2.0f, (y - INIT_WINDOW_SIZE_Y * 0.5f) * 2.0f, m_instance->m_Obstacles);
			}
			m_instance->m_is_Left_Clicked = false;
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN && !m_instance->m_is_Right_Clicked)
		{
			if (m_instance->m_Camera->Get_is_Top_View())
			{
				m_instance->m_Roller_Coaster->Select_CP_TOP_VIEW((x - INIT_WINDOW_SIZE_X * 0.5f) * 2.0f, (y - INIT_WINDOW_SIZE_Y * 0.5f) * 2.0f);
			}
			else if (m_instance->m_Camera->Get_is_Front_View())
			{
				m_instance->m_Roller_Coaster->Select_CP_FRONT_VIEW((x - INIT_WINDOW_SIZE_X * 0.5f) * 2.0f, (INIT_WINDOW_SIZE_Y * 0.5f - y) * 2.0f);
			}
			m_instance->m_is_Right_Clicked = true;
		}
		else if (state == GLUT_UP)
		{
			m_instance->m_Roller_Coaster->CP_Unselect();
			m_instance->m_is_Right_Clicked = false;
		}
		break;
	}
}
void GL_Main::Mouse_Move_Manager(int x, int y)
{
	if (m_instance->m_is_Right_Clicked)
	{
		if (m_instance->m_Camera->Get_is_Top_View())
			m_instance->m_Roller_Coaster->Control_CP((x - INIT_WINDOW_SIZE_X * 0.5f) * 2.0f, (y - INIT_WINDOW_SIZE_Y * 0.5f) * 2.0f);
		else if (m_instance->m_Camera->Get_is_Front_View()) 
			m_instance->m_Roller_Coaster->Control_CP((INIT_WINDOW_SIZE_Y * 0.5f - y) * 2.0f);
	}
}


void GL_Main::Init_Objects()
{
	m_Camera = new Camera(); // Camera
	
	m_instance->Object_Activate(m_instance->Build_Object(FLOOR_OBJECT));
	m_Floor->Wheather_Initialize(WHEATHER_TYPE_SNOW, 60, false);
	m_Floor->Switching_Wheathering();

	int num = 0;

	num = m_instance->Build_Object(ROBOT_OBJECT);
	m_Robot = reinterpret_cast<Robot*>(m_instance->Get_Object(num));
	reinterpret_cast<Object*>(m_Robot)->Set_Position_X(100.0f);
	reinterpret_cast<Object*>(m_Robot)->Set_Position_Z(0.0f);
	reinterpret_cast<Object*>(m_Robot)->Get_OBB()->Set_Center_Position(reinterpret_cast<Object*>(m_Robot)->Get_Position());
	m_Robot->Spot_Light_Setting();
	m_Robot->Init_Head();
	m_Robot->Set_is_Playable(true);
	m_instance->Object_Activate(num);

	num = m_instance->Build_Object(ROBOT_OBJECT);
	m_Robot_2 = reinterpret_cast<Robot*>(m_instance->Get_Object(num));
	m_instance->Object_Activate(num);

	num = m_instance->Build_Object(DIRECTIONAL_LIGHT_OBJECT);
	m_Directional_Light = reinterpret_cast<Light_Object*>(m_instance->Get_Object(num));

	for (int i = 0; i < 10; ++i)
	{
		m_Obstacles[i] = m_instance->Get_Object(m_instance->Build_Object(ROCK_OBSTACLE));
		i += 1;
		m_Obstacles[i] = m_instance->Get_Object(m_instance->Build_Object(TREE_OBSTACLE));
	}

	num = m_instance->Build_Object(ROLLER_COASTER);
	m_instance->Object_Activate(num);
	m_Roller_Coaster = reinterpret_cast<Roller_Coaster*>(m_instance->Get_Object(num));

	for (int i = 0; i < 20; ++i)
	{
		num = m_instance->Build_Object(BULLET_OBJECT);
		m_instance->Object_Activate(num);
		m_Bullet_Pool.push(reinterpret_cast<Bullet*>(m_instance->Get_Object(num)));
	}
}


void GL_Main::Reshape(const GLsizei w, const GLsizei h)
{ 
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.0f, 1.25f, 1.0f, 3000.0f);
	if (m_instance->m_is_Perspect) gluPerspective(60.0f, 1.25f, 1.0f, 3000.0f);
	else glOrtho(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f, -200.0f, 3000.0f);
}


void GL_Main::Render_Scene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	m_instance->m_Camera->Work();

	m_instance->Render_Background();
	m_instance->Render_Objects();
	
	glPopMatrix();

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
	Robot* temp_Robot = nullptr;
	Light_Object* temp_Light = nullptr;
	Obstacle* temp_Obstacle = nullptr;
	Roller_Coaster* temp_RC = nullptr;
	Bullet* temp_Bullet = nullptr;

	switch (type)
	{
	case FLOOR_OBJECT:
		m_Floor = new Floor(FLOOR_WIDTH, FLOOR_HEIGHT, FLOOR_PIECE_WIDTH_SIZE, FLOOR_PIECE_HEIGHT_SIZE);
		reinterpret_cast<Object*>(m_Floor)->Set_Object_Type(FLOOR_OBJECT);
		reinterpret_cast<Object*>(m_Floor)->Set_Translate(0.0f, -OBJECT_SIZE, 0.0f);
		reinterpret_cast<Object*>(m_Floor)->Set_is_Static(true);
		temp_Object = reinterpret_cast<Object*>(m_Floor);
		break;

	case ROBOT_OBJECT:
		temp_Robot = new Robot();
		temp_Object = reinterpret_cast<Object*>(temp_Robot);
		break;

	case DIRECTIONAL_LIGHT_OBJECT:
		temp_Light = new Light_Object(0);
		if (!temp_Light->Get_is_invalid_Light_Object())
		{
			reinterpret_cast<Object*>(temp_Light)->Set_Translate(0.0f, OBJECT_SIZE * 10.0f, 0.0f);
			reinterpret_cast<Object*>(temp_Light)->Pitch(90.0f);
			temp_Light->Set_Material_Ambient_Color(0.15f, 0.15f, 0.15f, 1.0f);
			temp_Light->Set_Material_Diffuse_Color(0.8f, 0.8f, 0.3f, 1.0f);
			temp_Light->Set_Material_Specular_Color(1.0f, 1.0f, 1.0f, 1.0f);
			temp_Light->Set_Light_Position(0.0f, 0.0f, 0.0f, 1.0f);
			reinterpret_cast<Object*>(temp_Light)->Set_is_Static(true);

			temp_Light->Light_On();
			temp_Object = reinterpret_cast<Object*>(temp_Light);
		}
		break;

	case ROCK_OBSTACLE:
		temp_Obstacle = new Obstacle(ROCK_OBSTACLE, m_Random_Position_X(m_Random_SEED), 0.0f, m_Random_Position_Z(m_Random_SEED));
		temp_Object = reinterpret_cast<Object*>(temp_Obstacle);
		break;

	case TREE_OBSTACLE:
		temp_Obstacle = new Obstacle(TREE_OBSTACLE, m_Random_Position_X(m_Random_SEED), 0.0f, m_Random_Position_Z(m_Random_SEED));
		temp_Object = reinterpret_cast<Object*>(temp_Obstacle);
		break;

	case ROLLER_COASTER:
		temp_RC = new Roller_Coaster(0.0f, -OBJECT_SIZE, 0.0f);
		temp_Object = reinterpret_cast<Object*>(temp_RC);
		break;

	case BULLET_OBJECT:
		temp_Bullet = new Bullet();
		temp_Bullet->Set_Pool_pointer(&m_Bullet_Pool);
		temp_Object = reinterpret_cast<Object*>(temp_Bullet);
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
					if (object != *p) p->second->Collision_Check(object.second);

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