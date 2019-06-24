#pragma once
#include "Object.h"

class Tree : Object
{
private:
	vector<vector<Vector3>> m_Tree_Leaves_Model;
	vector<Vector3> m_Tree_Leaves_Normals;
	vector<Vector3>::iterator m_Tree_Leaves_Normals_iter;

	Material m_Leaf_Material;
	Texture* m_Leaf_Texture = nullptr;

	vector<vector<Vector3>> m_Tree_Log_Model;
	vector<Vector3> m_Tree_Log_Normals;
	vector<Vector3>::iterator m_Tree_Log_Normals_iter;

public:

public:
	Tree() : Object() { Tree_Init(); }
	~Tree() { if (m_Leaf_Texture != nullptr) { delete m_Leaf_Texture; } }

	void Tree_Init()
	{
		m_Scale_X = 0.35f; m_Scale_Y = 1.5f; m_Scale_Z = 0.35f;
		m_Position.y = OBJECT_SIZE * m_Scale_Y * 0.1f;

		m_Material.Ambient[0] = 0.3f; m_Material.Ambient[1] = 0.3f; m_Material.Ambient[2] = 0.3f; m_Material.Ambient[3] = 1.0f;
		m_Material.Diffuse[0] = 0.5f; m_Material.Diffuse[1] = 0.5f; m_Material.Diffuse[2] = 0.5f; m_Material.Diffuse[3] = 1.0f;
		m_Material.Specular[0] = 0.6f; m_Material.Specular[1] = 0.6f; m_Material.Specular[2] = 0.6f; m_Material.Specular[3] = 1.0f;

		m_Leaf_Material.Ambient[0] = 0.7f; m_Leaf_Material.Ambient[1] = 0.7f; m_Leaf_Material.Ambient[2] = 0.7f; m_Leaf_Material.Ambient[3] = 1.0f;
		m_Leaf_Material.Diffuse[0] = 1.0f; m_Leaf_Material.Diffuse[1] = 1.0f; m_Leaf_Material.Diffuse[2] = 1.0f; m_Leaf_Material.Diffuse[3] = 1.0f;
		m_Leaf_Material.Specular[0] = 1.0f; m_Leaf_Material.Specular[1] = 1.0f; m_Leaf_Material.Specular[2] = 1.0f; m_Leaf_Material.Specular[3] = 1.0f;


		Set_Texture("Resources/Texture/Obstacle/Snow_Tree.bmp", 256, 512);
		m_Leaf_Texture = new Texture();
		m_Leaf_Texture->Set_Texture("Resources/Texture/Obstacle/Pine_Tree_Leaf.bmp", 1024, 1024);

		Tree_Leaf_Modelling();
		Tree_Log_Modelling();
	}

	virtual void Draw()
	{
		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
		glMateriali(GL_FRONT, GL_SHININESS, 60);

		int polygon_Count = 0;

		m_Tree_Log_Normals_iter = m_Tree_Log_Normals.begin();
		for (auto& polygon : m_Tree_Log_Model)
		{
			m_Texture->Mapping_Texture(false);
			glBegin(GL_POLYGON);
			glNormal3f(m_Tree_Log_Normals_iter->x, m_Tree_Log_Normals_iter->y, m_Tree_Log_Normals_iter->z);

			int vertex_Count = 0;
			for (auto& vertex : polygon)
			{
				if (vertex_Count == 0) glTexCoord2f(0.0f, 1.0f);
				else if (vertex_Count == 1) glTexCoord2f(0.0f, 0.0f); 
				else if (vertex_Count == 2) glTexCoord2f(1.0f, 0.0f);
				else if (vertex_Count == 3) glTexCoord2f(1.0f, 1.0f);

				glVertex3f(vertex.x, vertex.y, vertex.z);
				++vertex_Count;
			}
			glEnd();
			m_Texture->Mapping_Off(false);
			++m_Tree_Log_Normals_iter;
			++polygon_Count;
		}

		{
			glPushMatrix();

			glTranslatef(0.0f, OBJECT_SIZE * m_Scale_Y * 0.5f, 0.0f);
			glScalef(3.0f, 1.0f, 3.0f);
			polygon_Count = 0;

			glMaterialfv(GL_FRONT, GL_AMBIENT, m_Leaf_Material.Ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Leaf_Material.Diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_Leaf_Material.Specular);
			glMateriali(GL_FRONT, GL_SHININESS, 30);

			m_Tree_Leaves_Normals_iter = m_Tree_Leaves_Normals.begin();
			for (auto& polygon : m_Tree_Leaves_Model)
			{
				m_Leaf_Texture->Mapping_Texture(false);
				glBegin(GL_POLYGON);
				glNormal3f(m_Tree_Leaves_Normals_iter->x, m_Tree_Leaves_Normals_iter->y, m_Tree_Leaves_Normals_iter->z);

				int vertex_Count = 0;
				for (auto& vertex : polygon)
				{
					if (vertex_Count == 0)
					{
						if (polygon_Count == 0) glTexCoord2f(0.0f, 1.0f);
						else  glTexCoord2f(0.5f, 1.0f);
					}
					else if (vertex_Count == 1)
					{
						if (polygon_Count == 0) glTexCoord2f(0.0f, 0.0f);
						else  glTexCoord2f(0.0f, 0.0f);
					}
					else if (vertex_Count == 2)
					{
						if (polygon_Count == 0) glTexCoord2f(1.0f, 0.0f);
						else  glTexCoord2f(1.0f, 0.0f);
					}
					else if (vertex_Count == 3) glTexCoord2f(1.0f, 1.0f);

					glVertex3f(vertex.x, vertex.y, vertex.z);
					++vertex_Count;
				}
				glEnd();
				m_Leaf_Texture->Mapping_Off(false);
				++m_Tree_Leaves_Normals_iter;
				++polygon_Count;
			}
			glPopMatrix();
		}

		glPopMatrix();
	}

	void Tree_Leaf_Modelling()
	{
		for (auto& polygon : m_Tree_Leaves_Model) polygon.clear();
		m_Tree_Leaves_Model.clear();

		Vector3 temp_Vertex;
		vector<Vector3> temp_Polygon;
		Vector3 temp_Normal;

		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Leaves_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal.x = 0.0f; temp_Normal.y = -1.0f; temp_Normal.z = 0.0f;
		m_Tree_Leaves_Normals.push_back(temp_Normal);

		temp_Vertex.x = 0.0f; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = 0.0f;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Leaves_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal = Get_Normal_Vector(-OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE, OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE);
		m_Tree_Leaves_Normals.push_back(temp_Normal);

		temp_Vertex.x = 0.0f; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = 0.0f;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Leaves_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal = Get_Normal_Vector(OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE, OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE);
		m_Tree_Leaves_Normals.push_back(temp_Normal);

		temp_Vertex.x = 0.0f; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = 0.0f;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Leaves_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal = Get_Normal_Vector(OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE);
		m_Tree_Leaves_Normals.push_back(temp_Normal);

		temp_Vertex.x = 0.0f; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = 0.0f;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Leaves_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal = Get_Normal_Vector(-OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE);
		m_Tree_Leaves_Normals.push_back(temp_Normal);

	}

	void Tree_Log_Modelling()
	{
		for (auto& polygon : m_Tree_Log_Model) polygon.clear();
		m_Tree_Log_Model.clear();

		Vector3 temp_Vertex;
		vector<Vector3> temp_Polygon;
		Vector3 temp_Normal;

		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Log_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal.x = 0.0f; temp_Normal.y = 1.0f; temp_Normal.z = 0.0f;
		m_Tree_Log_Normals.push_back(temp_Normal);

		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Log_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal.x = 0.0f; temp_Normal.y = 0.0f; temp_Normal.z = 1.0f;
		m_Tree_Log_Normals.push_back(temp_Normal);

		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Log_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal.x = 1.0f; temp_Normal.y = 0.0f; temp_Normal.z = 0.0f;
		m_Tree_Log_Normals.push_back(temp_Normal);

		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Log_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal.x = 0.0f; temp_Normal.y = 0.0f; temp_Normal.z = -1.0f;
		m_Tree_Log_Normals.push_back(temp_Normal);

		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Log_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal.x = -1.0f; temp_Normal.y = 0.0f; temp_Normal.z = 0.0f;
		m_Tree_Log_Normals.push_back(temp_Normal);

		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Tree_Log_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal.x = 0.0f; temp_Normal.y = -1.0f; temp_Normal.z = 0.0f;
		m_Tree_Log_Normals.push_back(temp_Normal);
	}
};