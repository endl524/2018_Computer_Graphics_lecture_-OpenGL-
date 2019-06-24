#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Camera.h"

class GL_Main;
static GL_Main* m_instance;

class GL_Main
{
private: // 멤버변수 선언구역
	map<int, Object*> m_Object_List;

	Camera* m_Camera = nullptr;
	int m_Object_Indices[2];
	int m_Objects_Type;
	Object* m_World_Rotate_Manager = nullptr;

	bool m_is_Key_Down = false;
	bool m_is_Clicked = false;

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
	
	void Init_Objects();
	int Build_Object();
	Object* Make_Object();
	void Destroy_All_Objects_In_List();
	void Destroy_Objects_In_List(const int& index);

	static void Start_Object_Timer(const int object_num);

	void Object_Activate(const int& index);
};

