#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Camera.h"

#include "Light_Object.h"
#include "Robot.h"
#include "Floor.h"
#include "Obstacle.h"
#include "Roller_Coaster.h"
#include "Bullet.h"

class GL_Main;
static GL_Main* m_instance;

class GL_Main
{
private: // 멤버변수 선언구역
	map<int, Object*> m_Object_List;

	Camera* m_Camera = nullptr;

	random_device m_Random_Device;
	mt19937_64 m_Random_SEED;
	uniform_real_distribution<> m_Random_Color;
	uniform_real_distribution<> m_Random_Position_X;
	uniform_real_distribution<> m_Random_Position_Z;
	uniform_real_distribution<> m_Random_Particle_Speed;
	uniform_real_distribution<> m_Random_Particle_Direction;

	Robot* m_Robot = nullptr;
	Robot* m_Robot_2 = nullptr;
	Floor* m_Floor = nullptr;
	Light_Object* m_Directional_Light = nullptr;
	Roller_Coaster* m_Roller_Coaster = nullptr;
	Object* m_Obstacles[10]{ nullptr, };

	queue<Bullet*> m_Bullet_Pool;

	bool m_is_Key_Down = false;
	bool m_is_Left_Clicked = false;
	bool m_is_Right_Clicked = false;

	bool m_is_Perspect = false;

	bool m_is_Arrow_Key_Down = false;
	bool m_Dont_Processing_Arrow_Key = true;

public: // Getter, Setter 정의구역
	static GL_Main* Get_Instance() { return m_instance; };
	void Insert_Object(Object* obj)
	{
		m_instance->m_Object_List.insert(make_pair(obj->Get_Index(), obj)); // 삽입
	};
	Object* const Get_Object(const int index) { return m_instance->m_Object_List.find(index)->second; }

public: // public 메소드 정의구역
	GL_Main(int argc, char* argv[]);
	~GL_Main();

	void Reset(const bool& is_Reset);
	static void Render_Scene(void);
	static void Reshape(const GLsizei w, const GLsizei h);
	void Render_Background();
	void Render_Objects();

	static void Key_Manager(unsigned char key, int x, int y);
	static void Special_Key_Manager(int key, int x, int y);
	static void Special_Key_UP_Manager(int key, int x, int y);
	static void Mouse_Manager(int button, int state, int x, int y);
	static void Mouse_Move_Manager(int x, int y);
	static void MenuFunc(int button);

	void GL_Initializing();
	void Set_Random_Device();
	void Set_Menu();
	
	void Init_Objects();
	int Build_Object(const int& type);
	void Destroy_All_Objects_In_List();
	void Destroy_Objects_In_List(const int& index);

	static void Start_Object_Timer(const int object_num);
	static void Start_Camera_Zoom_to_Main_Timer(const int object_num);

	void Object_Activate(const int& index);
};

