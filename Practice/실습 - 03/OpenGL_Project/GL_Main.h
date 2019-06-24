#pragma once
#include "stdafx.h"
#include "Object.h"

#define INIT_WINDOW_POSITION_X 100
#define INIT_WINDOW_POSITION_Y 100
#define INIT_WINDOW_SIZE_X 800
#define INIT_WINDOW_SIZE_Y 600

#define LINE 0
#define TRIANGLE 1
#define RECTANGLE 2
#define PENTAGON 3

class GL_Main;
static GL_Main* m_instance;

class GL_Main
{
private: // 멤버변수 선언구역
	random_device m_Random_Device;
	mt19937_64 m_Random_SEED;
	uniform_real_distribution<> m_Random_Color;

	int m_Curr_Shape_Num;
	int m_Curr_Object_Count;
	int m_Division_Num_X;
	int m_Division_Num_Y;
	float m_Size_X;
	float m_Size_Y;
	float m_Offset_X;
	float m_Offset_Y;

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
	void Set_Line(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, int index_x, int index_y);
	void Set_Triangle(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, int index_x, int index_y);
	void Set_Rectangle(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, int index_x, int index_y);
	void Set_Pentagon(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, int index_x, int index_y);
};

