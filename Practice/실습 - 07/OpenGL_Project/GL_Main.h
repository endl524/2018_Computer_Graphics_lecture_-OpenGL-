#pragma once
#include "stdafx.h"
#include "Object.h"

class GL_Main;
static GL_Main* m_instance;

class GL_Main
{
private: // ������� ���𱸿�
	random_device m_Random_Device;
	mt19937_64 m_Random_SEED;
	uniform_real_distribution<> m_Random_Color;
	uniform_real_distribution<> m_Random_Speed;
	uniform_int_distribution<> m_Random_Type;
	int m_Curr_Object_Num;
	map<int, Object*> m_Object_List;

	bool m_is_Done_Building_Object;

public: // Getter, Setter ���Ǳ���
	static GL_Main* Get_Instance() { return m_instance; };
	void Insert_Object(Object* obj)
	{ 
		if (!m_instance->m_Object_List.empty()) // �̹� �ִ� ���� ���쵵�� ����
		{
			auto p = m_instance->m_Object_List.find(m_Curr_Object_Num); // "���� ��ȣ"�� ������ ã�´�.
			if (p != m_instance->m_Object_List.end()) // ���� �Ѵٸ� �����
			{
				delete p->second;
				m_instance->m_Object_List.erase(p);
			}
		}

		m_instance->m_Object_List.insert(make_pair(m_instance->m_Curr_Object_Num, obj)); // ����
	};
	Object* const Get_Object(const int index) { return m_instance->m_Object_List.find(index)->second; }

	void Set_Random_Device();

public: // public �޼ҵ� ���Ǳ���
	GL_Main(int argc, char* argv[]);
	~GL_Main();

	static void Draw_Scene(void);
	static void Reshape(const int w, const int h);
	void Draw_Background();
	void Draw_Objects();

	static void Key_Manager(unsigned char key, int x, int y);
	static void Special_Key_Manager(int key, int x, int y);
	static void Mouse_Manager(int button, int state, int x, int y);

	void Build_Object(const float x, const float y);
	Object* Make_Object(const GLenum type, vector<Vertex2D>& v_vertexs, const float pos_x, const float pos_y);
	void Set_Circle(Vertex2D& temp_vertex, vector<Vertex2D>& v_vertexs, float pos_x, float pos_y);
	void Destroy_All_Objects_In_List();
	void Destroy_Objects_In_List(const int index);


	static void Start_Object_Timer(const int object_num);
};

