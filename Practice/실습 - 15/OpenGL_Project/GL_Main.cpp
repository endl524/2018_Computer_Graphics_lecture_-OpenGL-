#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	
	glutCreateWindow("실습 15");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutDisplayFunc(Render_Scene);
	glutReshapeFunc(Reshape);

	Init_Objects();
	glutTimerFunc(0, Get_Time_Timer, 9999); // 시간 받기 시작.

	glutKeyboardFunc(Key_Manager);
	//glutMouseFunc(Mouse_Manager);
	glutMainLoop();
}

GL_Main::~GL_Main()
{
	Destroy_All_Objects_In_List();
	if (m_Object_World_Controller != nullptr) delete m_Object_World_Controller;
}

void GL_Main::Init_Objects()
{
	m_Object_World_Controller = new Object(true);

	int index, count = 0, offset_x = -NUMBER_INTERVAL_CLOSE_VALUE * 4, offset_y = -OBJECT_SIZE * 0.5f;
	vector<int> temp_vector;

	for (int i = -3; i <= 2; ++i)
	{
		temp_vector.clear();

		for (int j = -1; j < 2; ++j)
		{
			index = m_instance->Build_Object();
			m_instance->Get_Object(index)->Set_Local_Translate_X(i * OBJECT_SIZE * 2 + OBJECT_SIZE + offset_x);
			m_instance->Get_Object(index)->Set_Local_Translate_Y(j * OBJECT_SIZE * 2);
			m_instance->Get_Object(index)->Set_Local_Scale(1.8f, 0.5f, 1.0f);
			temp_vector.push_back(index);
		}

		index = m_instance->Build_Object();
		m_instance->Get_Object(index)->Set_Local_Translate_X(i * OBJECT_SIZE * 2 + OBJECT_SIZE - OBJECT_SIZE + offset_x);
		m_instance->Get_Object(index)->Set_Local_Translate_Y(OBJECT_SIZE);
		m_instance->Get_Object(index)->Set_Local_Scale(0.5f, 1.8f, 1.0f);
		temp_vector.push_back(index);

		index = m_instance->Build_Object();
		m_instance->Get_Object(index)->Set_Local_Translate_X(i * OBJECT_SIZE * 2 + OBJECT_SIZE + OBJECT_SIZE + offset_x);
		m_instance->Get_Object(index)->Set_Local_Translate_Y(OBJECT_SIZE);
		m_instance->Get_Object(index)->Set_Local_Scale(0.5f, 1.8f, 1.0f);
		temp_vector.push_back(index);

		index = m_instance->Build_Object();
		m_instance->Get_Object(index)->Set_Local_Translate_X(i * OBJECT_SIZE * 2 + OBJECT_SIZE - OBJECT_SIZE + offset_x);
		m_instance->Get_Object(index)->Set_Local_Translate_Y(-OBJECT_SIZE);
		m_instance->Get_Object(index)->Set_Local_Scale(0.5f, 1.8f, 1.0f);
		temp_vector.push_back(index);

		index = m_instance->Build_Object();
		m_instance->Get_Object(index)->Set_Local_Translate_X(i * OBJECT_SIZE * 2 + OBJECT_SIZE + OBJECT_SIZE + offset_x);
		m_instance->Get_Object(index)->Set_Local_Translate_Y(-OBJECT_SIZE);
		m_instance->Get_Object(index)->Set_Local_Scale(0.5f, 1.8f, 1.0f);
		temp_vector.push_back(index);

		++count;
		offset_x += count % 2 == 0 ? NUMBER_INTERVAL_FAR_VALUE : NUMBER_INTERVAL_CLOSE_VALUE;

		m_Number_Bar_indexs.push_back(temp_vector);

		if (count % 2 == 0 && count != 6)
		{
			for (int j = 0; j < 2; ++j)
			{
				index = m_instance->Build_Object();
				m_instance->Get_Object(index)->Set_Local_Translate_X(i * OBJECT_SIZE * 2 + offset_x + OBJECT_SIZE * 1.2f);
				m_instance->Get_Object(index)->Set_Local_Translate_Y(j * OBJECT_SIZE + offset_y);
				m_instance->Get_Object(index)->Set_Local_Scale(0.5f, 0.5f, 1.0f);
				m_Colon_Indexs.push_back(index);
			}
		}
	}

}

void GL_Main::Key_Manager(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'y': m_instance->m_Object_World_Controller->World_Rotate(-1); break;
	case 'Y': m_instance->m_Object_World_Controller->World_Rotate(1); break;

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
			int index = m_instance->Build_Object();
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_instance->Render_Background();
	m_instance->Render_Objects();
	glutSwapBuffers();
	glutPostRedisplay();
}

void GL_Main::Reshape(const int w, const int h) 
{ 
	glMatrixMode(GL_PROJECTION);
	glViewport(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f); 
	glOrtho(-w * 0.5f, w * 0.5f, -h * 0.5f, h * 0.5f, -400.0f, 400.0f); 
	glutPostRedisplay(); 

	glMatrixMode(GL_MODELVIEW);
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

void GL_Main::Get_Time_Timer(const int i)
{
	for (auto& One_Number : m_instance->m_Number_Bar_indexs)
	{
		for (auto& One_Bar : One_Number) m_instance->Get_Object(One_Bar)->Set_Active(false);
	}
	m_instance->now = time(0);
	localtime_s(&m_instance->curr_time, &m_instance->now);

	switch (m_instance->curr_time.tm_hour)
	{
	case 0: 
	case 1: 
	case 2: 
	case 3:
	case 4: 
	case 5: 
	case 6: 
	case 7: 
	case 8: 
	case 9: m_instance->Set_Number(1, m_instance->curr_time.tm_hour); break;

	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		m_instance->Set_Number(0, 1);
		m_instance->Set_Number(1, m_instance->curr_time.tm_hour - 10);
		break;

	case 20:
	case 21:
	case 22:
	case 23:
		m_instance->Set_Number(0, 2);
		m_instance->Set_Number(1, m_instance->curr_time.tm_hour - 20);
		break;
	}

	switch (m_instance->curr_time.tm_min)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9: m_instance->Set_Number(3, m_instance->curr_time.tm_min); break;

	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		m_instance->Set_Number(2, 1);
		m_instance->Set_Number(3, m_instance->curr_time.tm_min - 10);
		break;

	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
		m_instance->Set_Number(2, 2);
		m_instance->Set_Number(3, m_instance->curr_time.tm_min - 20);
		break;

	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
		m_instance->Set_Number(2, 3);
		m_instance->Set_Number(3, m_instance->curr_time.tm_min - 30);
		break;

	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
		m_instance->Set_Number(2, 4);
		m_instance->Set_Number(3, m_instance->curr_time.tm_min - 40);
		break;

	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
		m_instance->Set_Number(2, 5);
		m_instance->Set_Number(3, m_instance->curr_time.tm_min - 50);
		break;
	}

	switch (m_instance->curr_time.tm_sec)
	{
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9: m_instance->Set_Number(5, m_instance->curr_time.tm_sec); break;

	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 16:
	case 17:
	case 18:
	case 19:
		m_instance->Set_Number(4, 1);
		m_instance->Set_Number(5, m_instance->curr_time.tm_sec - 10);
		break;

	case 20:
	case 21:
	case 22:
	case 23:
	case 24:
	case 25:
	case 26:
	case 27:
	case 28:
	case 29:
		m_instance->Set_Number(4, 2);
		m_instance->Set_Number(5, m_instance->curr_time.tm_sec - 20);
		break;

	case 30:
	case 31:
	case 32:
	case 33:
	case 34:
	case 35:
	case 36:
	case 37:
	case 38:
	case 39:
		m_instance->Set_Number(4, 3);
		m_instance->Set_Number(5, m_instance->curr_time.tm_sec - 30);
		break;

	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
		m_instance->Set_Number(4, 4);
		m_instance->Set_Number(5, m_instance->curr_time.tm_sec - 40);
		break;

	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
		m_instance->Set_Number(4, 5);
		m_instance->Set_Number(5, m_instance->curr_time.tm_sec - 50);
		break;
	}

	for (auto& colon_point : m_instance->m_Colon_Indexs)
	{
		if (m_instance->Get_Object(colon_point)->Get_is_Activated())
			m_instance->Get_Object(colon_point)->Set_Active(false);
		else m_instance->Get_Object(colon_point)->Set_Active(true);
	}

	glutTimerFunc(500, Get_Time_Timer, 9999);
}

void GL_Main::Set_Number(const int& index, const int& number)
{
	switch (number)
	{
	case 0:
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][0])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][2])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][3])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][4])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][5])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][6])->Set_Active(true);
		break;

	case 1:
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][4])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][6])->Set_Active(true);
		break;

	case 2:
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][0])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][1])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][2])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][4])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][5])->Set_Active(true);
		break;

	case 3:
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][0])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][1])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][2])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][4])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][6])->Set_Active(true);
		break;

	case 4:
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][1])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][3])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][4])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][6])->Set_Active(true);
		break;

	case 5:
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][0])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][1])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][2])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][3])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][6])->Set_Active(true);
		break;

	case 6:
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][0])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][1])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][2])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][3])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][5])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][6])->Set_Active(true);
		break;

	case 7:
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][2])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][3])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][4])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][6])->Set_Active(true);
		break;

	case 8:
		for (int i = 0; i < 7; ++i)
			m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][i])->Set_Active(true);
		break;

	case 9:
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][0])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][1])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][2])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][3])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][4])->Set_Active(true);
		m_instance->Get_Object(m_instance->m_Number_Bar_indexs[index][6])->Set_Active(true);
		break;
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