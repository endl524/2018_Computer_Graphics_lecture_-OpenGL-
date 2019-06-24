#pragma once
#include "stdafx.h"
#include "Object.h"

#define INIT_WINDOW_POSITION_X 100
#define INIT_WINDOW_POSITION_Y 100
#define INIT_WINDOW_SIZE_X 800
#define INIT_WINDOW_SIZE_Y 600

class GL_Main;
static GL_Main* m_instance;

class GL_Main
{
private: // 멤버변수 선언구역
	int m_Curr_Object_Count;
	map<int, Object*> m_Object_List;

public: // Getter, Setter 정의구역
	static GL_Main* Get_Instance() { return m_instance; };
	void Insert_Object(Object* obj) { m_instance->m_Object_List.insert(make_pair(m_instance->m_Curr_Object_Count, obj)); };
	Object* const Get_Object(const int index) { return m_instance->m_Object_List.find(index)->second; }

public: // public 메소드 정의구역
	GL_Main(int argc, char* argv[]);
	~GL_Main();

	static void Draw_Scene(void);
	static void Reshape(const int w, const int h);
	void Draw_Background();
	void Draw_Objects();

	void Init_Objects();
	Object* Make_Object(const GLenum type, vector<Vertex2D>& v_vertexs);
};

