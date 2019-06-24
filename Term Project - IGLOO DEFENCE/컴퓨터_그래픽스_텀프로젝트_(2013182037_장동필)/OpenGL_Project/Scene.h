#pragma once
#include "stdafx.h"

#include "Camera.h"

#include "UI.h"
#include "Object.h"
#include "Light_Object.h"
#include "Robot.h"
#include "Floor.h"
#include "Snow_Ball.h"
#include "Rock.h"
#include "Tree.h"
#include "Fence.h"
#include "Snows.h"
#include "Igloo.h"
#include "Small_Enemy.h"

class Scene;
static Scene* s_Scene_instance;

class Scene
{
protected:
	int m_Curr_Scene_Number;
	bool m_is_Scene_Change;
	int m_Curr_Window_Size_X;
	int m_Curr_Window_Size_Y;

	Camera* m_Camera = nullptr;

	bool m_is_Left_Clicked = false;
	bool m_is_Right_Clicked = false;

	map<int, Object*> m_Object_List;
	map<int, UI*> m_UI_List;

	random_device m_Random_Device;
	mt19937_64 m_Random_SEED;

public:
	Scene* Get_Curr_Scene_Pointer() { return s_Scene_instance; }
	static void Set_Scene_instance(Scene* scene) { s_Scene_instance = scene; }
	int const Get_Curr_Scene_Number() { return m_Curr_Scene_Number; }
	void Change_Scene(const int& scene_num) { m_Curr_Scene_Number = scene_num; Set_is_Scene_Change(true); }
	bool const Get_is_Scene_Change() { return m_is_Scene_Change; }
	void Set_is_Scene_Change(const bool& b) { m_is_Scene_Change = b; }

	void Set_Window_Size(const int& x, const int& y) { m_Curr_Window_Size_X = x; m_Curr_Window_Size_Y = y; }

	void Insert_Object(Object* obj) { m_Object_List.insert(make_pair(obj->Get_Index(), obj)); };
	Object* const Get_Object(const int index) { return m_Object_List.find(index)->second; }
	void Destroy_All_Objects_In_List()
	{
		if (!m_Object_List.empty())
		{
			for (auto& Data : m_Object_List) delete Data.second;
			m_Object_List.clear();
		}
	}
	void Destroy_Objects_In_List(const int& index)
	{
		if (!m_Object_List.empty())
		{
			auto p = m_Object_List.find(index);
			if (p != m_Object_List.end())
			{
				delete p->second;
				m_Object_List.erase(p);
			}
		}
	}

	void Insert_UI(UI* ui) { m_UI_List.insert(make_pair(ui->Get_Index(), ui)); };
	UI* const Get_UI(const int index) { return m_UI_List.find(index)->second; }
	void Destroy_All_UIs_In_List()
	{
		if (!m_UI_List.empty())
		{
			for (auto& Data : m_UI_List) delete Data.second;
			m_UI_List.clear();
		}
	}
	void Destroy_UIs_In_List(const int& index)
	{
		if (!m_UI_List.empty())
		{
			auto p = m_UI_List.find(index);
			if (p != m_UI_List.end())
			{
				delete p->second;
				m_UI_List.erase(p);
			}
		}
	}

	virtual void Set_Random_Device()
	{
		mt19937_64 seed(m_Random_Device());
		m_Random_SEED = seed;
	}

public:
	Scene() {}
	virtual ~Scene() { Destroy_Scene(); }

	void Initialize_Scene(const int& scene_num, const int& window_width, const int& window_height)
	{
		m_Curr_Scene_Number = scene_num;
		s_Scene_instance = this;

		Set_Window_Size(window_width, window_height);

		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);

		glEnable(GL_LIGHTING);
		float ambient[4]{ 0.6f, 0.6f, 0.6f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
		glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 0.0f);
		glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0f);

		glEnable(GL_NORMALIZE);
		glEnable(GL_AUTO_NORMAL);

		glEnable(GL_BLEND);

		Set_Random_Device();
		Init_Camera();
		Init_Object();
		Init_UI();
	}
	
	void Destroy_Scene() 
	{
		Destroy_All_Objects_In_List();
		Destroy_All_UIs_In_List();

		if (m_Camera != nullptr)
		{
			delete m_Camera;
			m_Camera = nullptr;
		}
	}

	virtual void Init_Camera()
	{
		if (m_Camera != nullptr)
		{
			delete m_Camera;
			m_Camera = nullptr;
		}
		m_Camera = new Camera();
		m_Camera->Free_View();
		m_Camera->Set_is_FPS(true);
	}
	virtual void Init_Object() {}
	virtual int Build_Object(const int& type) { return 0; }
	virtual void Init_UI() {}
	virtual int Build_UI(const int& type) { return 0; }

	void Render_Scene() 
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(60.0f, 1.25f, 1.0f, 5000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		m_Camera->Work();

		Render_Background();
		Render_Objects();
		Render_UI();
	}
	virtual void Render_Background() { glClearColor(1.0f, 1.0f, 1.0f, 1.0f); }
	void Render_Objects()
	{
		for (auto& object : m_Object_List) { object.second->Draw(); }
	}
	void Render_UI()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-m_Curr_Window_Size_X * 0.5f, m_Curr_Window_Size_X * 0.5f, -m_Curr_Window_Size_Y * 0.5f, m_Curr_Window_Size_Y * 0.5f, -1.0f, 5000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		for (auto& ui : m_UI_List) { ui.second->Draw(); }
	}

	virtual void Key_Manager(unsigned char key, const int& x, const int& y)
	{
		switch (key)
		{
		case 27: glutLeaveMainLoop(); break;
		}
	}
	virtual void Key_Up_Manager(unsigned char key, const int& x, const int& y) {}
	virtual void Special_Key_Manager(const int& key, const int& x, const int& y) {}
	virtual void Special_Key_Up_Manager(const int& key, const int& x, const int& y) {}
	virtual void Mouse_Manager(const int& button, const int& state, const int& x, const int& y) 
	{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN && !m_is_Left_Clicked)
			{
				m_is_Left_Clicked = true;
			}
			else if (state == GLUT_UP)
			{
				//m_Roller_Coaster->Insert_CP((x - INIT_WINDOW_SIZE_X * 0.5f) * 2.0f, (y - INIT_WINDOW_SIZE_Y * 0.5f) * 2.0f, m_Obstacles);
				
				m_is_Left_Clicked = false;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_DOWN && !m_is_Right_Clicked)
			{
				m_is_Right_Clicked = true;
			}
			else if (state == GLUT_UP)
			{
				m_is_Right_Clicked = false;
			}
			break;
		}
	}
	virtual void Mouse_Clicked_Move_Manager(const int& x, const int& y)
	{

		if (m_is_Left_Clicked)
		{

		}

		if (m_is_Right_Clicked)
		{
			
		}
	}
	virtual void Mouse_Non_Clicked_Move_Manager(const int& x, const int& y) {}
	static void Set_Object_Update_Timer(const int object_num) 
	{
		auto p = s_Scene_instance->m_Object_List.find(object_num);
		if (p != s_Scene_instance->m_Object_List.end())
		{
			if (!p->second->Get_is_Destroyed())
			{
				if (p->second->Get_is_Activated())
				{
					p->second->Update();

					for (auto& object : s_Scene_instance->m_Object_List)
						if (object != *p) p->second->Collision_Check(object.second);

					glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, s_Scene_instance->Set_Object_Update_Timer, object_num);
				}
			}
			else if (p->second->Get_is_Destroyed()) s_Scene_instance->Destroy_Objects_In_List(object_num);
		}
	}
	static void Object_Activate(const int& index)
	{
		auto p = s_Scene_instance->m_Object_List.find(index);
		if (p != s_Scene_instance->m_Object_List.end())
		{
			p->second->Set_Active(true);
			glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, s_Scene_instance->Set_Object_Update_Timer, index);
		}
	}
	static void Set_Production_Update_Timer(const int object_num)
	{
		if (s_Scene_instance->m_Camera != nullptr)
		{
			if (s_Scene_instance->Production_Update()) glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, s_Scene_instance->Set_Production_Update_Timer, object_num);
		}
	}
	virtual bool Production_Update() { return false; }
};