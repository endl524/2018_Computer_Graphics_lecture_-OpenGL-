#pragma once
#include "Object.h"

class Floor : Object
{
private:
	int m_Width, m_Height;
	int m_Piece_Width_Size, m_Piece_Height_Size;
	vector<vector<Vector2>> m_Pieces_Coord;
	vector<vector<Material>> m_Pieces_Material;

	vector<vector<Vector2>>::iterator m_Pieces_Coord_iter;
	vector<Vector2>::iterator m_One_Line_Coord_iter;
	vector<vector<Material>>::iterator m_Pieces_Material_iter;
	vector<Material>::iterator m_One_Line_Material_iter;

	// ==========================================================

	vector<float> m_Wheather_Particles_Speed;
	vector<Vector3> m_Wheather_Particles_Coord;
	vector<Material> m_Wheather_Particles_Matirial;
	Color4f m_Wheather_Particles_Color;

	vector<float>::iterator m_Wheather_Particles_Speed_iter;
	vector<Vector3>::iterator m_Wheather_Particles_Coord_iter;
	vector<Material>::iterator m_Wheather_Particles_Material_iter;

	uniform_int_distribution<> m_WP_Random_Coord_Width;
	uniform_int_distribution<> m_WP_Random_Coord_Height;
	uniform_real_distribution<> m_WP_Random_Speed;
	random_device m_Random_Device;
	mt19937_64 m_Random_SEED;

	bool m_is_Wheathering = false;
	bool m_is_Wheathering_Freeze = false;
	bool m_is_WP_Using_Material = false;
	int m_Wheather_Type = 0;

	// ==========================================================

public:

public:
	Floor(const int& width, const int& height, const int& piece_width_size, const int& piece_height_size)
		: Object()
	{
		mt19937_64 seed(m_Random_Device());
		m_Random_SEED = seed;
		uniform_int_distribution<> rand_WP_Pos_W(-width/2, width/2);
		m_WP_Random_Coord_Width = rand_WP_Pos_W;
		uniform_int_distribution<> rand_WP_Pos_H(-height/2, height/2);
		m_WP_Random_Coord_Height = rand_WP_Pos_H;
		uniform_real_distribution<> rand_WP_Speed(WP_FALLING_SPEED_MIN, WP_FALLING_SPEED_MAX);
		m_WP_Random_Speed = rand_WP_Speed;
		
		Floor_Initialize(width, height, piece_width_size, piece_height_size);
	}
	~Floor() 
	{
		for (auto& one_Line : m_Pieces_Coord) one_Line.clear();
		m_Pieces_Coord.clear();
		for (auto& one_Line : m_Pieces_Material) one_Line.clear();
		m_Pieces_Material.clear();
		m_Wheather_Particles_Coord.clear();
		m_Wheather_Particles_Matirial.clear();
	}

	void Floor_Initialize(const int& width, const int& height, const int& piece_width_size, const int& piece_height_size)
	{
		m_Width = width; m_Height = height;
		m_Piece_Width_Size = piece_width_size; m_Piece_Height_Size = piece_height_size;

		int w = width / 2;
		int h = height / 2;
		Vector2 temp_Coord;
		vector<Vector2> temp_Coord_Vector;
		Material temp_Material;
		vector<Material> temp_Material_Vector;

		for (auto& one_Line : m_Pieces_Coord) one_Line.clear();
		m_Pieces_Coord.clear();
		for (auto& one_Line : m_Pieces_Material) one_Line.clear();
		m_Pieces_Material.clear();

		for (int i = -width / 2; i <= width / 2; ++i)
		{
			for (int j = -height / 2; j <= height / 2; ++j)
			{
				temp_Coord.x = i * piece_width_size * 2;
				temp_Coord.y = j * piece_height_size * 2;
				temp_Coord_Vector.push_back(temp_Coord);

				temp_Material.Ambient[0] = 0.15f; temp_Material.Ambient[1] = 0.15f; temp_Material.Ambient[2] = 0.15f; temp_Material.Ambient[3] = 1.0f;
				temp_Material.Diffuse[0] = 1.0f; temp_Material.Diffuse[1] = 1.0f; temp_Material.Diffuse[2] = 1.0f; temp_Material.Diffuse[3] = 1.0f;
				temp_Material.Specular[0] = 1.0f; temp_Material.Specular[1] = 1.0f; temp_Material.Specular[2] = 1.0f; temp_Material.Specular[3] = 1.0f;
				temp_Material_Vector.push_back(temp_Material);
			}
			m_Pieces_Coord.push_back(temp_Coord_Vector);
			temp_Coord_Vector.clear();
			m_Pieces_Material.push_back(temp_Material_Vector);
			temp_Material_Vector.clear();
		}
	}

	virtual void Draw()
	{
		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		glShadeModel(GL_SMOOTH);

		m_Pieces_Coord_iter = m_Pieces_Coord.begin();
		m_Pieces_Material_iter = m_Pieces_Material.begin();

		while (m_Pieces_Coord_iter != m_Pieces_Coord.end())
		{
			m_One_Line_Coord_iter = m_Pieces_Coord_iter->begin();
			m_One_Line_Material_iter = m_Pieces_Material_iter->begin();

			while (m_One_Line_Coord_iter != m_Pieces_Coord_iter->end())
			{
				glPushMatrix();

				glTranslatef(m_One_Line_Coord_iter->x, 0.0f, m_One_Line_Coord_iter->y);

				glMaterialfv(GL_FRONT, GL_AMBIENT, m_One_Line_Material_iter->Ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_One_Line_Material_iter->Diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_One_Line_Material_iter->Specular);
				glMateriali(GL_FRONT, GL_SHININESS, 20);

				glBegin(GL_QUADS);
				glNormal3f(0.0f, 1.0f, 0.0f);
				glVertex3f(-m_Piece_Width_Size, 0.0f, -m_Piece_Height_Size);
				glVertex3f(-m_Piece_Width_Size, 0.0f, m_Piece_Height_Size);
				glVertex3f(m_Piece_Width_Size, 0.0f, m_Piece_Height_Size);
				glVertex3f(m_Piece_Width_Size, 0.0f, -m_Piece_Height_Size);
				glEnd();

				glPopMatrix();

				++m_One_Line_Coord_iter;
				++m_One_Line_Material_iter;
			}

			++m_Pieces_Coord_iter;
			++m_Pieces_Material_iter;
		}

		if (m_is_Wheathering)
		{
			m_Wheather_Particles_Coord_iter = m_Wheather_Particles_Coord.begin();
			m_Wheather_Particles_Material_iter = m_Wheather_Particles_Matirial.begin();

			if (!m_is_WP_Using_Material) glDisable(GL_LIGHTING);
			while (m_Wheather_Particles_Coord_iter != m_Wheather_Particles_Coord.end())
			{
				glPushMatrix();

				glTranslatef(m_Wheather_Particles_Coord_iter->x, m_Wheather_Particles_Coord_iter->y, m_Wheather_Particles_Coord_iter->z);
				if (m_is_WP_Using_Material)
				{
					glMaterialfv(GL_FRONT, GL_AMBIENT, m_Wheather_Particles_Material_iter->Ambient);
					glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Wheather_Particles_Material_iter->Diffuse);
					glMaterialfv(GL_FRONT, GL_SPECULAR, m_Wheather_Particles_Material_iter->Specular);
					glMateriali(GL_FRONT, GL_SHININESS, 20);
					
					++m_Wheather_Particles_Material_iter;
				}
				else glColor4f(m_Wheather_Particles_Color.R, m_Wheather_Particles_Color.G, m_Wheather_Particles_Color.B, m_Wheather_Particles_Color.A);

				glutSolidSphere(OBJECT_SIZE * 0.05f, 5, 5);

				glPopMatrix();

				++m_Wheather_Particles_Coord_iter;	
			}
			if (!m_is_WP_Using_Material) glEnable(GL_LIGHTING);
		}

		glPopMatrix();
	}

	void Switching_Wheathering() { m_is_Wheathering = m_is_Wheathering == true ? false : true; }
	void Switching_Wheathering_Freeze() { m_is_Wheathering_Freeze = m_is_Wheathering_Freeze == true ? false : true; }

	void Wheather_Initialize(const int& wheather_type, const int& particle_count, const bool& using_material)
	{
		m_Wheather_Type = wheather_type;
		m_is_WP_Using_Material = using_material;

		m_Wheather_Particles_Coord.clear();
		m_Wheather_Particles_Matirial.clear();

		Vector3 temp_Coord;

		for (int i = 0; i < particle_count; ++i)
		{
			temp_Coord.x = m_WP_Random_Coord_Width(m_Random_SEED) * FLOOR_PIECE_WIDTH_SIZE * 2;
			temp_Coord.y = WP_INIT_POSITION_Y - m_Position.y; // 맵 최대 높이로 설정할 것.
			temp_Coord.z = m_WP_Random_Coord_Height(m_Random_SEED) * FLOOR_PIECE_HEIGHT_SIZE * 2;

			m_Wheather_Particles_Coord.push_back(temp_Coord);
			m_Wheather_Particles_Speed.push_back(m_WP_Random_Speed(m_Random_SEED));
		}

		// 모델링?
		/*
		switch (wheather_type)
		{
		case WHEATHER_TYPE_SNOW: break;
		}
		*/


		if (using_material)
		{
			Material temp_Material;

			// 모델도 여기서 설정한다.
			switch (wheather_type)
			{
			case WHEATHER_TYPE_SNOW:
				for (int i = 0; i < particle_count; ++i)
				{
					temp_Material.Ambient[0] = 1.0f; temp_Material.Ambient[1] = 1.0f; temp_Material.Ambient[2] = 1.0f; temp_Material.Ambient[3] = 1.0f;
					temp_Material.Diffuse[0] = 1.0f; temp_Material.Diffuse[1] = 1.0f; temp_Material.Diffuse[2] = 1.0f; temp_Material.Diffuse[3] = 1.0f;
					temp_Material.Specular[0] = 1.0f; temp_Material.Specular[1] = 1.0f; temp_Material.Specular[2] = 1.0f; temp_Material.Specular[3] = 1.0f;

					m_Wheather_Particles_Matirial.push_back(temp_Material);
				}
				break;
			}

			m_Wheather_Particles_Material_iter = m_Wheather_Particles_Matirial.begin();
		}
		else
		{
			switch (wheather_type)
			{
			case WHEATHER_TYPE_SNOW:
				m_Wheather_Particles_Color.R = 1.0f; m_Wheather_Particles_Color.G = 1.0f; m_Wheather_Particles_Color.B = 1.0f; m_Wheather_Particles_Color.A = 1.0f; break;
			}
		}
	}

	void Wheater_Particles_Falling()
	{
		if (m_is_Wheathering && !m_is_Wheathering_Freeze)
		{
			m_Wheather_Particles_Coord_iter = m_Wheather_Particles_Coord.begin();
			m_Wheather_Particles_Speed_iter = m_Wheather_Particles_Speed.begin();

			int i = 0, j = 0;
			while (m_Wheather_Particles_Coord_iter != m_Wheather_Particles_Coord.end())
			{
				m_Wheather_Particles_Coord_iter->y += -*m_Wheather_Particles_Speed_iter;
				if (m_Wheather_Particles_Coord_iter->y < 0.0f)
				{
					i = m_Wheather_Particles_Coord_iter->x / (FLOOR_PIECE_WIDTH_SIZE * 2) + FLOOR_WIDTH / 2;
					j = m_Wheather_Particles_Coord_iter->z / (FLOOR_PIECE_HEIGHT_SIZE * 2) + FLOOR_HEIGHT / 2;

					m_Wheather_Particles_Coord_iter->y = WP_INIT_POSITION_Y - m_Position.y;
					m_Pieces_Material[i][j].Ambient[0] += 0.2f;
					m_Pieces_Material[i][j].Ambient[1] += 0.2f;
					m_Pieces_Material[i][j].Ambient[2] += 0.2f;
					m_Pieces_Material[i][j].Ambient[3] += 0.2f;
					m_Wheather_Particles_Coord_iter->x = m_WP_Random_Coord_Width(m_Random_SEED) * FLOOR_PIECE_WIDTH_SIZE * 2;
					m_Wheather_Particles_Coord_iter->z = m_WP_Random_Coord_Height(m_Random_SEED) * FLOOR_PIECE_HEIGHT_SIZE * 2;
					*m_Wheather_Particles_Speed_iter = m_WP_Random_Speed(m_Random_SEED);
				}
				
				++m_Wheather_Particles_Coord_iter;
				++m_Wheather_Particles_Speed_iter;
			}
		}
	}

	virtual void Update()
	{
		Wheater_Particles_Falling();
	}
};