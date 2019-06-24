#pragma once
#include "stdafx.h"

class Object
{
private: // 멤버변수 선언구역
	int m_Index = -1;
	GLenum m_Drawing_Type;
	vector<Vertex2D> m_Vertexs;

public: // Getter, Setter 정의구역
	void Set_Index(const int index) { m_Index = index; }
	int const Get_Index() { return m_Index; }
	void Set_Drawing_Type(const GLenum type) { m_Drawing_Type = type; }
	GLenum const Get_Drawing_Type() { return m_Drawing_Type; }

	void Set_Vertexs_vector(vector<Vertex2D>& v_vertexs) { m_Vertexs = v_vertexs; }
	vector<Vertex2D> Get_VertexList() { return m_Vertexs; }
	Vertex2D const Get_Vertex(const int index) { return m_Vertexs[index]; }
	

public: // public 메소드 정의구역
	Object(const GLenum type, vector<Vertex2D>& v_vertexs, const int index);
	~Object();

	void Draw();
};

