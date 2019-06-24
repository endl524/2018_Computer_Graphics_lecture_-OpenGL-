#pragma once
#include "stdafx.h"
#include "Object.h"

class GL_Main;
static GL_Main* m_instance;

class GL_Main
{
private: // ������� ���𱸿�
	map<int, Object*> m_Object_List;

	Object* m_Path_Line = nullptr;

	vector<Vertex3D> m_Vertexs;
	int m_Custom_Path_Vertex_Count = 0;
	bool m_is_Clicked = false;
	bool m_is_Set_Over_Custom_Path_Vertex = true;

public: // Getter, Setter ���Ǳ���
	static GL_Main* Get_Instance() { return m_instance; };
	void Insert_Object(Object* obj)
	{
		m_instance->m_Object_List.insert(make_pair(obj->Get_Index(), obj)); // ����
	};
	Object* const Get_Object(const int index) { return m_instance->m_Object_List.find(index)->second; }

public: // public �޼ҵ� ���Ǳ���
	GL_Main(int argc, char* argv[]);
	~GL_Main();

	static void Render_Scene(void);
	static void Reshape(const int w, const int h);
	void Render_Background();
	void Render_Objects();

	static void Key_Manager(unsigned char key, int x, int y);
	static void Mouse_Manager(int button, int state, int x, int y);
	

	int Build_Object(const bool& is_active);
	Object* Make_Object(const bool& is_active);
	void Destroy_All_Objects_In_List();
	void Destroy_Objects_In_List(const int& index);

	static void Start_Object_Timer(const int object_num);

	void Object_Activate(const int& index);
};

