#pragma once
#include "stdafx.h"

#include "Scene.h"
#include "Main_Scene.h"
#include "InGame_Scene.h"

class GL_Main;
static GL_Main* s_GL_Main_instance;

class GL_Main
{
private: // 멤버변수 선언구역
	int m_Curr_Window_Size_X;
	int m_Curr_Window_Size_Y;

	Scene* m_Curr_Scene = nullptr;


public: // Getter, Setter 정의구역

public: // public 메소드 정의구역
	GL_Main(int argc, char* argv[])
	{
		s_GL_Main_instance = this;

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
		m_Curr_Window_Size_X = INIT_WINDOW_SIZE_X; m_Curr_Window_Size_Y = INIT_WINDOW_SIZE_Y;
		glutInitWindowSize(m_Curr_Window_Size_X, m_Curr_Window_Size_Y);

		glutCreateWindow("Igloo Defence!");

		glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
		glutDisplayFunc(Render_Scene);
		glutReshapeFunc(Reshape);

		glutKeyboardFunc(Key_Manager);
		glutKeyboardUpFunc(Key_Up_Manager);
		glutSpecialFunc(Special_Key_Manager);
		glutSpecialUpFunc(Special_Key_UP_Manager);
		glutMouseFunc(Mouse_Manager);
		glutMotionFunc(Mouse_Clicked_Move_Manager);
		glutPassiveMotionFunc(Mouse_Non_Clicked_Move_Manager);

		glutTimerFunc(1, s_GL_Main_instance->Scene_Change_Check, -777);

		if (Init_Main_Scene()) glutMainLoop();
	}

	~GL_Main()
	{
		if (m_Curr_Scene != nullptr)
		{
			delete m_Curr_Scene;
			m_Curr_Scene = nullptr;
		}
	}

	static void Render_Scene(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		if (s_GL_Main_instance->m_Curr_Scene != nullptr) s_GL_Main_instance->m_Curr_Scene->Render_Scene();

		glutSwapBuffers();
		glutPostRedisplay();
	}
	static void Reshape(const GLsizei w, const GLsizei h)
	{
		s_GL_Main_instance->m_Curr_Window_Size_X = w;
		s_GL_Main_instance->m_Curr_Window_Size_Y = h;
		s_GL_Main_instance->m_Curr_Scene->Set_Window_Size(s_GL_Main_instance->m_Curr_Window_Size_X, s_GL_Main_instance->m_Curr_Window_Size_Y);

		glViewport(0, 0, w, h);
	}

	static void Key_Manager(unsigned char key, int x, int y) { if (s_GL_Main_instance->m_Curr_Scene != nullptr) s_GL_Main_instance->m_Curr_Scene->Key_Manager(key, x, y); }
	static void Key_Up_Manager(unsigned char key, int x, int y) { if (s_GL_Main_instance->m_Curr_Scene != nullptr) s_GL_Main_instance->m_Curr_Scene->Key_Up_Manager(key, x, y); }
	static void Special_Key_Manager(int key, int x, int y) { if (s_GL_Main_instance->m_Curr_Scene != nullptr) s_GL_Main_instance->m_Curr_Scene->Special_Key_Manager(key, x, y); }
	static void Special_Key_UP_Manager(int key, int x, int y) { if (s_GL_Main_instance->m_Curr_Scene != nullptr) s_GL_Main_instance->m_Curr_Scene->Special_Key_Up_Manager(key, x, y); }
	static void Mouse_Manager(int button, int state, int x, int y) { if (s_GL_Main_instance->m_Curr_Scene != nullptr) s_GL_Main_instance->m_Curr_Scene->Mouse_Manager(button, state, x, y); }
	static void Mouse_Clicked_Move_Manager(int x, int y) { if (s_GL_Main_instance->m_Curr_Scene != nullptr) s_GL_Main_instance->m_Curr_Scene->Mouse_Clicked_Move_Manager(x, y); }
	static void Mouse_Non_Clicked_Move_Manager(int x, int y) { if (s_GL_Main_instance->m_Curr_Scene != nullptr) s_GL_Main_instance->m_Curr_Scene->Mouse_Non_Clicked_Move_Manager(x, y); }

	bool Init_Main_Scene()
	{
		Main_Scene* temp_Main_Scene = nullptr;
		temp_Main_Scene = new Main_Scene();

		m_Curr_Scene = reinterpret_cast<Scene*>(temp_Main_Scene);
		m_Curr_Scene->Initialize_Scene(0, s_GL_Main_instance->m_Curr_Window_Size_X, s_GL_Main_instance->m_Curr_Window_Size_Y);
		m_Curr_Scene->Set_is_Scene_Change(false);
		if (m_Curr_Scene != nullptr) return true;
		else return false;
	}

	static void Scene_Change_Check(const int num)
	{
		if (s_GL_Main_instance->m_Curr_Scene != nullptr)
		{
			if (s_GL_Main_instance->m_Curr_Scene->Get_is_Scene_Change())
			{
				switch (s_GL_Main_instance->m_Curr_Scene->Get_Curr_Scene_Number())
				{
				case 0:
					delete s_GL_Main_instance->m_Curr_Scene;
					Main_Scene* temp_Main_Scene;
					temp_Main_Scene = new Main_Scene();
					s_GL_Main_instance->m_Curr_Scene = reinterpret_cast<Scene*>(temp_Main_Scene);
					s_GL_Main_instance->m_Curr_Scene->Initialize_Scene(0, s_GL_Main_instance->m_Curr_Window_Size_X, s_GL_Main_instance->m_Curr_Window_Size_Y);
					s_GL_Main_instance->m_Curr_Scene->Set_is_Scene_Change(false);
					break;

				case 1:
					delete s_GL_Main_instance->m_Curr_Scene;
					InGame_Scene* temp_InGame_Scene;
					temp_InGame_Scene = new InGame_Scene();
					s_GL_Main_instance->m_Curr_Scene = reinterpret_cast<Scene*>(temp_InGame_Scene);
					s_GL_Main_instance->m_Curr_Scene->Initialize_Scene(1, s_GL_Main_instance->m_Curr_Window_Size_X, s_GL_Main_instance->m_Curr_Window_Size_Y);
					s_GL_Main_instance->m_Curr_Scene->Set_is_Scene_Change(false);
					break;

				default:
					break;
				}
			}
		}
		else s_GL_Main_instance->Init_Main_Scene();

		glutTimerFunc(1, s_GL_Main_instance->Scene_Change_Check, -777);
	}
};

