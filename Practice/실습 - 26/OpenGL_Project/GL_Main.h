#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Camera.h"

#include "Cube.h"
#include "Ball.h"
#include "Box.h"


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
	uniform_int_distribution<> m_Random_Speed;
	uniform_real_distribution<> m_Random_Position;
	uniform_real_distribution<> m_Random_Direction;

	Cube* m_Cube = nullptr;
	Box* m_Box[3]{ nullptr };

	bool m_is_Key_Down = false;
	bool m_is_Clicked = false;
	int m_Prev_Mouse_X_Value = 0;

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

	static void Render_Scene(void);
	static void Reshape(const int w, const int h);
	void Render_Background();
	void Render_Objects();

	static void Key_Manager(unsigned char key, int x, int y);
	static void Special_Key_Manager(int key, int x, int y);
	static void Mouse_Manager(int button, int state, int x, int y);
	static void Mouse_Move_Manager(int x, int y);
	static void MenuFunc(int button);

	void Set_Random_Device();
	void Set_Menu();
	
	void Init_Objects();
	int Build_Object(const int& type);
	void Destroy_All_Objects_In_List();
	void Destroy_Objects_In_List(const int& index);

	static void Start_Object_Timer(const int object_num);

	void Object_Activate(const int& index);
};

