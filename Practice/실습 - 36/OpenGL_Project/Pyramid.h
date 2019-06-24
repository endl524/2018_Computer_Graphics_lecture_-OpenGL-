#pragma once
#include "Object.h"
#include "Particle_Object.h"

class Pyramid : Object 
{
private:
	vector<vector<Vector3>> m_Pyramid_Model;
	vector<Vector3> m_Models_Polygon_Normals;
	vector<Vector3>::iterator m_Normals_iter;

	Particle_Object* m_Pyramid_Particle;

	bool m_is_Plane_Normal_On = true;

	uniform_real_distribution<> m_PP_Random_Speed;
	uniform_real_distribution<> m_PP_Random_Dir;
	random_device m_Random_Device;
	mt19937_64 m_Random_SEED;

	vector<Texture*> m_Textures;

public:
	void Set_is_Plane_Normal_On(const bool& b) { m_is_Plane_Normal_On = b; }
	bool const Get_is_Plane_Normal_On() { return m_is_Plane_Normal_On; }
	void Switching_is_Plane_Normal_On() { m_is_Plane_Normal_On = m_is_Plane_Normal_On == true ? false : true; }


public:
	Pyramid() : Object() 
	{ 
		Pyramid_Modelling(); 

		mt19937_64 seed(m_Random_Device());
		m_Random_SEED = seed;

		uniform_real_distribution<> rand_PP_Speed(PARTICLE_SPEED_MIN, PARTICLE_SPEED_MAX);
		m_PP_Random_Speed = rand_PP_Speed;
		uniform_real_distribution<> rand_PP_Dir(PARTICLE_DIR_MIN, PARTICLE_DIR_MAX);
		m_PP_Random_Dir = rand_PP_Dir;
	};
	~Pyramid() 
	{
		for (auto& polygon : m_Pyramid_Model) polygon.clear();
		m_Pyramid_Model.clear();

		if (m_Pyramid_Particle != nullptr)
		{
			delete m_Pyramid_Particle; m_Pyramid_Particle = nullptr;
		}

		for (auto& texture : m_Textures) if (texture != nullptr) delete texture;
		m_Textures.clear();
	};

	void Pyramid_Initialize()
	{
		m_Pyramid_Particle = new Particle_Object();
		reinterpret_cast<Object*>(m_Pyramid_Particle)->Set_Translate(m_Position.x, m_Position.y, m_Position.z);
		m_Pyramid_Particle->Set_Random_Speeds(m_PP_Random_Speed, m_Random_SEED);
		m_Pyramid_Particle->Set_Random_Directions(m_PP_Random_Dir, m_Random_SEED);

		Texture* temp_Texture;
		temp_Texture = new Texture();
		temp_Texture->Set_Texture("pyramid_texture.bmp", 256, 256);
		m_Textures.push_back(temp_Texture);
	}

	void Pyramid_Modelling()
	{
		for (auto& polygon : m_Pyramid_Model) polygon.clear();
		m_Pyramid_Model.clear();

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
		m_Pyramid_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal.x = 0.0f; temp_Normal.y = -1.0f; temp_Normal.z = 0.0f;
		m_Models_Polygon_Normals.push_back(temp_Normal);

		temp_Vertex.x = 0.0f; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = 0.0f;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Pyramid_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal = Get_Normal_Vector(-OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE, OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE);
		m_Models_Polygon_Normals.push_back(temp_Normal);

		temp_Vertex.x = 0.0f; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = 0.0f;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Pyramid_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal = Get_Normal_Vector(OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE, OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE);
		m_Models_Polygon_Normals.push_back(temp_Normal);

		temp_Vertex.x = 0.0f; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = 0.0f;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Pyramid_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal = Get_Normal_Vector(OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE);
		m_Models_Polygon_Normals.push_back(temp_Normal);

		temp_Vertex.x = 0.0f; temp_Vertex.y = OBJECT_SIZE; temp_Vertex.z = 0.0f;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = -OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		temp_Vertex.x = -OBJECT_SIZE; temp_Vertex.y = -OBJECT_SIZE; temp_Vertex.z = OBJECT_SIZE;
		temp_Polygon.push_back(temp_Vertex);
		m_Pyramid_Model.push_back(temp_Polygon);
		temp_Polygon.clear();
		temp_Normal = Get_Normal_Vector(-OBJECT_SIZE, -OBJECT_SIZE * 2.0f, -OBJECT_SIZE, -OBJECT_SIZE, -OBJECT_SIZE * 2.0f, OBJECT_SIZE);
		m_Models_Polygon_Normals.push_back(temp_Normal);
	}

	virtual void Draw()
	{
		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glShadeModel(GL_SMOOTH);

		glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
		glMateriali(GL_FRONT, GL_SHININESS, 20);

		if (m_is_Plane_Normal_On)
		{
			m_Normals_iter = m_Models_Polygon_Normals.begin();
			
			int polygon_Count = 0;
			for (auto& polygon : m_Pyramid_Model)
			{
				m_Textures[0]->Mapping_Texture(false);
				glBegin(GL_POLYGON);
				glNormal3f(m_Normals_iter->x, m_Normals_iter->y, m_Normals_iter->z);

				int vertex_Count = 0;
				for (auto& vertex : polygon)
				{
					if (vertex_Count == 0)
					{
						if (polygon_Count == 0) glTexCoord2f(1.0f, 1.0f);
						else glTexCoord2f(0.5f, 1.0f);
					}
					else if (vertex_Count == 1)
					{
						if (polygon_Count == 0) glTexCoord2f(1.0f, 0.0f);
						else glTexCoord2f(0.0f, 0.0f);
					}
					else if (vertex_Count == 2)
					{
						if (polygon_Count == 0) glTexCoord2f(0.0f, 0.0f);
						else glTexCoord2f(1.0f, 0.0f);
					}
					else if (vertex_Count == 3) glTexCoord2f(0.0f, 1.0f);
					
					glVertex3f(vertex.x, vertex.y, vertex.z);

					++vertex_Count;
				}
				glEnd();
				m_Textures[0]->Mapping_Off(false);
				++m_Normals_iter;
				++polygon_Count;
			}
		}
		else
		{
			for (auto& polygon : m_Pyramid_Model)
			{
				glBegin(GL_POLYGON);
				for (auto& vertex : polygon) glVertex3f(vertex.x, vertex.y, vertex.z);
				glEnd();
			}
		}		
		
		glPopMatrix();

		if (m_Pyramid_Particle != nullptr) m_Pyramid_Particle->Draw();
	}

	virtual void Collision_Procedure()
	{
		if (m_Pyramid_Particle->Get_is_Particle_Over())
		{
			m_Pyramid_Particle->Set_Random_Speeds(m_PP_Random_Speed, m_Random_SEED);
			m_Pyramid_Particle->Set_Random_Directions(m_PP_Random_Dir, m_Random_SEED);
		}
		m_Pyramid_Particle->Set_is_Particle_Over(false);
	}

	virtual void Update()
	{
		if (m_Pyramid_Particle != nullptr) m_Pyramid_Particle->Update();
	}
};