#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	
	glutCreateWindow("½Ç½À 16");

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutDisplayFunc(Render_Scene);
	glutReshapeFunc(Reshape);

	Init_Objects();

	glutKeyboardFunc(Key_Manager);
	glutMainLoop();
}

GL_Main::~GL_Main()
{
	Destroy_All_Objects_In_List();
	if (m_Camera != nullptr) delete m_Camera;
	if (m_World_Rotate_Manager != nullptr) delete m_World_Rotate_Manager;
}

void GL_Main::Init_Objects()
{
	m_Camera = new Camera(); // Camera
	m_World_Rotate_Manager = new Object(true); // WRM

	int index = 0;

	index = m_instance->Build_Object(); // X-Axis
	m_instance->Get_Object(index)->Set_Object_Type(OBJECT_TYPE_CUBE);
	m_instance->Get_Object(index)->Set_Translate(OBJECT_SIZE * 0.25f, 0.0f, 0.0f);
	m_instance->Get_Object(index)->Set_Scale(0.5f, 0.02f, 0.02f);
	m_instance->Get_Object(index)->Set_Color(1.0f, 0.0f, 0.0f, 1.0f);
	m_instance->Get_Object(index)->Set_is_Solid_Shape(true);

	index = m_instance->Build_Object(); // Y-Axis
	m_instance->Get_Object(index)->Set_Object_Type(OBJECT_TYPE_CUBE);
	m_instance->Get_Object(index)->Set_Translate(0.0f, OBJECT_SIZE * 0.25f, 0.0f);
	m_instance->Get_Object(index)->Set_Scale(0.02f, 0.5f, 0.02f);
	m_instance->Get_Object(index)->Set_Color(0.0f, 1.0f, 0.0f, 1.0f);
	m_instance->Get_Object(index)->Set_is_Solid_Shape(true);

	index = m_instance->Build_Object(); // Z-Axis
	m_instance->Get_Object(index)->Set_Object_Type(OBJECT_TYPE_CUBE);
	m_instance->Get_Object(index)->Set_Translate(0.0f, 0.0f, OBJECT_SIZE * 0.25f);
	m_instance->Get_Object(index)->Set_Scale(0.02f, 0.02f, 0.5f);
	m_instance->Get_Object(index)->Set_Color(0.0f, 0.0f, 1.0f, 1.0f);
	m_instance->Get_Object(index)->Set_is_Solid_Shape(true);

	index = m_instance->Build_Object(); // Plane
	m_instance->Get_Object(index)->Set_Object_Type(OBJECT_TYPE_CUBE);
	m_instance->Get_Object(index)->Set_Translate(0.0f, -OBJECT_SIZE, 0.0f);
	m_instance->Get_Object(index)->Set_Color(1.0f, 0.5f, 0.5f, 0.5f);
	m_instance->Get_Object(index)->Set_Scale(5.0f, 0.02f, 5.0f);
	m_instance->Get_Object(index)->Set_is_Solid_Shape(true);

	index = m_instance->Build_Object(); // Object_1
	m_Object_Indices[0] = index;
	m_instance->Get_Object(index)->Set_Object_Type(OBJECT_TYPE_CUBE);
	m_instance->Get_Object(index)->Set_Translate(-OBJECT_SIZE, 0.0f, 0.0f);
	m_instance->Get_Object(index)->Set_Color(1.0f, 1.0f, 0.0f, 1.0f);
	m_instance->Get_Object(index)->Set_Scale(1.0f, 2.0f, 1.0f);
	m_instance->Get_Object(index)->Set_is_Solid_Shape(true);
	m_instance->Get_Object(index)->Set_is_Shape_Object(true);

	index = m_instance->Build_Object(); // Object_2
	m_Object_Indices[1] = index;
	m_instance->Get_Object(index)->Set_Object_Type(OBJECT_TYPE_CUBE);
	m_instance->Get_Object(index)->Set_Translate(OBJECT_SIZE, 0.0f, 0.0f);
	m_instance->Get_Object(index)->Set_Color(1.0f, 1.0f, 0.0f, 1.0f);
	m_instance->Get_Object(index)->Set_Scale(1.0f, 2.0f, 1.0f);
	m_instance->Get_Object(index)->Set_is_Solid_Shape(false);
	m_instance->Get_Object(index)->Set_is_Shape_Object(true);
}

void GL_Main::Key_Manager(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		m_instance->Get_Object(m_instance->m_Object_Indices[0])->Shape_Change(OBJECT_TYPE_SPHERE, true);
		m_instance->Get_Object(m_instance->m_Object_Indices[1])->Shape_Change(OBJECT_TYPE_SPHERE, false);
		break;

	case '2':
		m_instance->Get_Object(m_instance->m_Object_Indices[0])->Shape_Change(OBJECT_TYPE_CUBE, true);
		m_instance->Get_Object(m_instance->m_Object_Indices[1])->Shape_Change(OBJECT_TYPE_CUBE, false);
		break;

	case '3':
		m_instance->Get_Object(m_instance->m_Object_Indices[0])->Shape_Change(OBJECT_TYPE_CONE, true);
		m_instance->Get_Object(m_instance->m_Object_Indices[1])->Shape_Change(OBJECT_TYPE_CONE, false);
		break;

	case '4':
		m_instance->Get_Object(m_instance->m_Object_Indices[0])->Shape_Change(OBJECT_TYPE_TEAPOT, true);
		m_instance->Get_Object(m_instance->m_Object_Indices[1])->Shape_Change(OBJECT_TYPE_TEAPOT, false);
		break;

	case 'x': m_instance->m_Camera->Set_Rotate_Pitch(m_instance->m_Camera->Get_Rotate_Pitch() - 3.0f); break;
	case 'X': m_instance->m_Camera->Set_Rotate_Pitch(m_instance->m_Camera->Get_Rotate_Pitch() + 3.0f); break;
	case 'y': m_instance->m_Camera->Set_Rotate_Yaw(m_instance->m_Camera->Get_Rotate_Yaw() - 3.0f); break;
	case 'Y': m_instance->m_Camera->Set_Rotate_Yaw(m_instance->m_Camera->Get_Rotate_Yaw() + 3.0f); break;
	case 'z': m_instance->m_Camera->Set_Rotate_Roll(m_instance->m_Camera->Get_Rotate_Roll() - 3.0f); break;
	case 'Z': m_instance->m_Camera->Set_Rotate_Roll(m_instance->m_Camera->Get_Rotate_Roll() + 3.0f); break;

	case 'l': m_instance->Get_Object(m_instance->m_Object_Indices[0])->Rotate(1, -3.0f); break;
	case 'L': m_instance->Get_Object(m_instance->m_Object_Indices[0])->Rotate(1, 3.0f); break;

	case 'r': m_instance->Get_Object(m_instance->m_Object_Indices[1])->Rotate(1, -3.0f); break;
	case 'R': m_instance->Get_Object(m_instance->m_Object_Indices[1])->Rotate(1, 3.0f); break;

	case 'o': m_instance->m_World_Rotate_Manager->World_Rotate(1, -3.0f); break;
	case 'O':m_instance->m_World_Rotate_Manager->World_Rotate(1, 3.0f); break;

	case 'c':
		m_instance->m_Objects_Type = m_instance->Get_Object(m_instance->m_Object_Indices[0])->Get_Object_Type();
		m_instance->m_Objects_Type = m_instance->m_Objects_Type == OBJECT_TYPE_SPHERE ? OBJECT_TYPE_TEAPOT : m_instance->m_Objects_Type - 1;
		m_instance->Get_Object(m_instance->m_Object_Indices[0])->Shape_Change(m_instance->m_Objects_Type, true);
		m_instance->Get_Object(m_instance->m_Object_Indices[1])->Shape_Change(m_instance->m_Objects_Type, false);
		break;

	case 'C':
		m_instance->m_Objects_Type = m_instance->Get_Object(m_instance->m_Object_Indices[0])->Get_Object_Type();
		m_instance->m_Objects_Type = m_instance->m_Objects_Type == OBJECT_TYPE_TEAPOT ? 0 : m_instance->m_Objects_Type + 1;
		m_instance->Get_Object(m_instance->m_Object_Indices[0])->Shape_Change(m_instance->m_Objects_Type, true);
		m_instance->Get_Object(m_instance->m_Object_Indices[1])->Shape_Change(m_instance->m_Objects_Type, false);
		break;

	case 27: glutLeaveMainLoop(); break;
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

	gluPerspective(45.0f, w / h, 1.0f, 1000.0f);
	gluLookAt(
		0.0f, 50.0f, 300.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f
	);
}

void GL_Main::Render_Background() 
{ 
	glClearColor(0.8f, 0.8f, 1.0f, 1.0f);
}

void GL_Main::Render_Objects()
{
	for (auto& object : m_instance->m_Object_List) object.second->Draw();
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