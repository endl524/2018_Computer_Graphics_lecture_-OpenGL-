#pragma once
#include "stdafx.h"
#include "Object.h"

class GL_Main;
static GL_Main* m_instance;

class GL_Main
{
private: // 멤버변수 선언구역
	random_device m_Random_Device;
	mt19937_64 m_Random_SEED;
	uniform_real_distribution<> m_Random_Color;
	uniform_real_distribution<> m_Random_Dir_X;
	uniform_real_distribution<> m_Random_Dir_Y;
	uniform_real_distribution<> m_Random_Speed;
	uniform_real_distribution<> m_Random_Position_X;
	uniform_real_distribution<> m_Random_Position_Y;
	uniform_real_distribution<> m_Random_Size_X;
	uniform_real_distribution<> m_Random_Size_Y;
	uniform_int_distribution<> m_Random_Wall_Riding_Direction;

	map<int, Object*> m_Object_List;

	Object* m_Eraser_Object;
	bool m_is_Done_Building_Object;

public: // Getter, Setter 정의구역
	static GL_Main* Get_Instance() { return m_instance; };
	void Insert_Object(Object* obj)
	{
		m_instance->m_Object_List.insert(make_pair(obj->Get_Index(), obj)); // 삽입
	};
	Object* const Get_Object(const int index) { return m_instance->m_Object_List.find(index)->second; }

	void Set_Random_Device();

public: // public 메소드 정의구역
	GL_Main(int argc, char* argv[]);
	~GL_Main();

	static void Draw_Scene(void);
	static void Reshape(const int w, const int h);
	void Draw_Background();
	void Draw_Objects();

	static void Key_Manager(unsigned char key, int x, int y);
	static void Special_Key_Manager(int key, int x, int y);
	static void Mouse_Manager(int button, int state, int x, int y);
	static void Mouse_Clicked_Move_Manager(int x, int y);
	

	void Build_Object(const float& x, const float& y, const bool& is_active, const int& type);
	Object* Make_Object(const GLenum& type, vector<Vertex2D>& v_vertexs, const float& pos_x, const float& pos_y, const float& size_x, const float& size_y, const bool& is_active, const int& obj_type);
	void Set_Rectangle(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, const float& pos_x, const float& pos_y, const float& size_x, const float& size_y, const int& type);
	void Destroy_All_Objects_In_List();
	void Destroy_Objects_In_List(const int index);

	void Init_Objects(const int& num);

	static void Start_Object_Timer(const int object_num);
};

