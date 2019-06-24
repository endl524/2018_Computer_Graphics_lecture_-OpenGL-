#pragma once
#include "Object.h"

class Roller_Coaster : Object
{
private:
	float m_Control_Points[19][3]{ 0.0f };
	float m_Control_Points_2[19][3]{ 0.0f };
	float m_Control_Points_3[19][3]{ 0.0f };

	int m_CP_Count = 0;
	bool m_is_CP_Making_Over = false;
	int m_Selected_CP = -1;
	vector<OBB*> m_OBBs;

	float m_Tunnel_Control_Points[3][3][3];
	float m_Tunnel_Floor_Control_Points[3][3][3];
	float m_Tunnel_Diffuse[4];


	float m_Train_Position[3][3];
	float m_Train_Rotation_Value[3];
	float m_Train_Rotation_LookAt[3];
	float m_Train_Rotation_Axis[3][3];
	float m_Train_Scale[3];
	int m_Train_CP_index[3];
	int m_Train_Curv_index[3];
	int m_Train_Speed = 150; // 값이 작을 수록 빠르다.
	Material m_Train_Material;
	float m_Train_Front_Bar_Diffuse[4];
	float m_Train_Side_Bar_Diffuse[4];
	float m_Train_Wheel_Diffuse[4];
	float m_Train_Human_Diffuse[4];

	Camera* m_Camera;

public:
	void Set_is_CP_Making_Over(const bool& b) { if (m_CP_Count >= 4) { m_is_CP_Making_Over = b; Set_CPs_OBB(); Set_Tunnel_CPs(); Set_Other_Lines(); } }
	bool const Get_is_CP_Making_Over() { return m_is_CP_Making_Over; }

	vector<OBB*> Get_OBBs() { return m_OBBs; }

	void CP_Unselect() { m_Selected_CP = -1; }

	void Set_Camera(Camera* c)
	{
		m_Camera = c;
		m_Camera->Set_Target(this);
	}

	void Release_Camera()
	{
		if (m_Camera != nullptr)
		{
			m_Camera->Release_Target();
			m_Camera = nullptr;
		}
	}

	virtual void Init_Head()
	{
		m_Head_Position[0] = m_Train_Position[0][0] + m_Look_Vector_For_Camera[0] * OBJECT_SIZE * m_Scale_X * 0.5f;
		m_Head_Position[1] = m_Train_Position[0][1] + OBJECT_SIZE * m_Scale_Y * 0.8f;
		m_Head_Position[2] = m_Train_Position[0][2] + m_Look_Vector_For_Camera[2] * OBJECT_SIZE * m_Scale_Z * 0.5f;
		m_Head_Position[3] = 1.0f;
	}

	virtual float const Get_Look_Vector(const int& num)
	{
		if (num >= 0 && num <= 2)
			return m_Look_Vector_For_Camera[num];
		return 0;
	}

public:
	Roller_Coaster(const float& x, const float& y, const float& z) : Object() { Roller_Coaster_Init( x, y, z); }
	~Roller_Coaster()
	{
		if (!m_OBBs.empty())
		{
			for (auto& obb : m_OBBs)
				delete obb;
			m_OBBs.clear();
		}
	}

	void Roller_Coaster_Init(const float& x, const float& y, const float& z)
	{
		m_Object_Type = ROLLER_COASTER;

		m_Position.x = x; m_Position.y = y; m_Position.z = z;
		m_Scale_X = 0.8f; m_Scale_Y = 0.8f; m_Scale_Z = 0.8f;
		m_Position.y = -OBJECT_SIZE + OBJECT_SIZE * m_Scale_Y * 0.5f;
		
		m_Material.Ambient[0] = 0.2f; m_Material.Ambient[1] = 0.2f; m_Material.Ambient[2] = 0.2f; m_Material.Ambient[3] = 1.0f;
		m_Material.Diffuse[0] = 0.7f; m_Material.Diffuse[1] = 0.5f; m_Material.Diffuse[2] = 0.5f; m_Material.Diffuse[3] = 1.0f;
		m_Material.Specular[0] = 1.0f; m_Material.Specular[1] = 1.0f; m_Material.Specular[2] = 1.0f; m_Material.Specular[3] = 1.0f;
		
		m_Forward_Vector[0] = 0.0f; m_Forward_Vector[1] = 0.0f; m_Forward_Vector[2] = 1.0f;
		m_Right_Vector[0] = 1.0f; m_Right_Vector[1] = 0.0f; m_Right_Vector[2] = 0.0f;
		m_Up_Vector[0] = 0.0f; m_Up_Vector[1] = 1.0f; m_Up_Vector[2] = 0.0f;

		m_Tunnel_Diffuse[0] = 0.3f; m_Tunnel_Diffuse[1] = 0.3f; m_Tunnel_Diffuse[2] = 0.3f; m_Tunnel_Diffuse[3] = 1.0f;

		Init_Train();
		Init_Head();
	}

	virtual void Draw()
	{
		Draw_CPs(); 
		Draw_Tunnel();
		Draw_Line(0); Draw_Line(1); Draw_Line(2);

		Draw_Train(0); 
		Draw_Train(1);
		Draw_Train(2);
	}

	void Draw_Rail()
	{
		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);


		glPopMatrix();
	}

	void Insert_CP(const float& cp_x, const float& cp_z, Object** obstacles)
	{
		if (!m_is_CP_Making_Over)
		{
			if (m_CP_Count >= 18) return;
			for (int i = 0; i < m_CP_Count; ++i)
			{
				if (m_Control_Points[i][0] + 100.0f >= cp_x && m_Control_Points[i][0] - 100.0f <= cp_x
					&& m_Control_Points[i][2] + 100.0f >= cp_z && m_Control_Points[i][2] - 100.0f <= cp_z)
					return;
			}
			if (cp_x < -FLOOR_PIECE_WIDTH_SIZE * FLOOR_WIDTH || cp_x > FLOOR_PIECE_WIDTH_SIZE * FLOOR_WIDTH
				|| cp_z < -FLOOR_PIECE_HEIGHT_SIZE * FLOOR_HEIGHT || cp_z > FLOOR_PIECE_HEIGHT_SIZE * FLOOR_HEIGHT)
				return;
				
			for (int i = 0; i < 10; ++i)
			{
				if (obstacles[i]->Get_Position_X() + 50.0f >= cp_x && obstacles[i]->Get_Position_X() - 50.0f <= cp_x
					&& obstacles[i]->Get_Position_Z() + 50.0f >= cp_z && obstacles[i]->Get_Position_Z() - 50.0f <= cp_z)
					return;
			}

			m_Control_Points[m_CP_Count][0] = cp_x;
			m_Control_Points[m_CP_Count][1] = 200.0f;
			m_Control_Points[m_CP_Count][2] = cp_z;


			++m_CP_Count;
		}
	}

	void Select_CP_TOP_VIEW(const float& cp_x, const float& cp_z)
	{
		if (!m_is_CP_Making_Over)
		{
			for (int i = 0; i < m_CP_Count; ++i)
			{
				if (m_Control_Points[i][0] + 10.0f >= cp_x && m_Control_Points[i][0] - 10.0f <= cp_x
					&& m_Control_Points[i][2] + 10.0f >= cp_z && m_Control_Points[i][2] - 10.0f <= cp_z)
					m_Selected_CP = i;
			}
		}
	}

	void Select_CP_FRONT_VIEW(const float& cp_x, const float& cp_y)
	{
		if (!m_is_CP_Making_Over)
		{
			for (int i = 0; i < m_CP_Count; ++i)
			{
				if (m_Control_Points[i][0] + 10.0f >= cp_x && m_Control_Points[i][0] - 10.0f <= cp_x
					&& m_Control_Points[i][1] + 10.0f >= cp_y && m_Control_Points[i][1] - 10.0f <= cp_y)
					m_Selected_CP = i;
			}
		}
	}

	void Control_CP(const float& re_x, const float& re_z)
	{
		if (!m_is_CP_Making_Over)
		{
			if (m_Selected_CP != -1)
			{
				m_Control_Points[m_Selected_CP][0] += re_x - m_Control_Points[m_Selected_CP][0];
				m_Control_Points[m_Selected_CP][2] += re_z - m_Control_Points[m_Selected_CP][2];
			}
		}
	}

	void Control_CP(const float& re_y)
	{
		if (!m_is_CP_Making_Over)
		{
			if (m_Selected_CP != -1)
			{
				if (re_y >= 200.0f)
				{
					m_Control_Points[m_Selected_CP][1] += re_y - m_Control_Points[m_Selected_CP][1];
				}
			}
		}
	}

	void Draw_CPs()
	{
		if (!m_is_CP_Making_Over)
		{
			for (int i = 0; i < m_CP_Count; ++i)
			{
				glPushMatrix();
				glTranslatef(m_Control_Points[i][0], m_Control_Points[i][1], m_Control_Points[i][2]);
				glPointSize(5.0f);
				glDisable(GL_LIGHTING);
				glColor3f(0.0f, 0.0f, 1.0f);
				glBegin(GL_POINTS);
				glVertex3f(0.0f, 0.0f, 0.0f);
				glEnd();
				glEnable(GL_LIGHTING);
				glPopMatrix();
			}
		}
		else
		{
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
			glMateriali(GL_FRONT, GL_SHININESS, 20);

			for (int i = 0; i < m_CP_Count; i += 2)
			{
				glPushMatrix();
				glTranslatef(m_Control_Points[i][0], m_Control_Points[i][1] * 0.3f, m_Control_Points[i][2]);
				glScalef(1.0f, m_Control_Points[i][1] / OBJECT_SIZE * 1.2f, 1.0f);
				glutSolidCube(OBJECT_SIZE);
				glTranslatef(0.0f, 20.0f, 0.0f);
				glScalef(0.2f, 0.15f, 0.2f);
				glutSolidCube(OBJECT_SIZE);
				glPopMatrix();
			}
		}
	}

	void Draw_Line(const int& num)
	{
		if (m_is_CP_Making_Over)
		{
			if (num == 0)
			{
				int i = 0;

				glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
				glMateriali(GL_FRONT, GL_SHININESS, 20);

				if (m_CP_Count % 2 == 0)
				{
					for (; i < m_CP_Count; i += 2)
					{
						glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, 3, &m_Control_Points[i][0]);
						glEnable(GL_MAP1_VERTEX_3);
						glMapGrid1f(10.0f, 0.0f, 1.0f);
						glEvalMesh1(GL_LINE, 0, 10);
						glDisable(GL_MAP1_VERTEX_3);
					}
					m_Control_Points[m_CP_Count][0] = m_Control_Points[0][0];
					m_Control_Points[m_CP_Count][1] = m_Control_Points[0][1];
					m_Control_Points[m_CP_Count][2] = m_Control_Points[0][2];
				}

				else
				{
					for (; i < m_CP_Count - 1; i += 2)
					{
						glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, 3, &m_Control_Points[i][0]);
						glEnable(GL_MAP1_VERTEX_3);
						glMapGrid1f(10.0f, 0.0f, 1.0f);
						glEvalMesh1(GL_LINE, 0, 10);
						glDisable(GL_MAP1_VERTEX_3);
					}
					m_Control_Points[m_CP_Count][0] = m_Control_Points[0][0];
					m_Control_Points[m_CP_Count][1] = m_Control_Points[0][1];
					m_Control_Points[m_CP_Count][2] = m_Control_Points[0][2];
					glPushMatrix();
					glTranslatef(m_Control_Points[m_CP_Count][0], m_Control_Points[m_CP_Count][1], m_Control_Points[m_CP_Count][2]);
					glColor3f(0.0f, 0.0f, 1.0f);
					glBegin(GL_LINE_STRIP);

					for (int i = 0; i <= 10; ++i)
						glVertex3f(
						(m_Control_Points[m_CP_Count - 1][0] - m_Control_Points[m_CP_Count][0]) * i / 10,
							(m_Control_Points[m_CP_Count - 1][1] - m_Control_Points[m_CP_Count][1]) * i / 10,
							(m_Control_Points[m_CP_Count - 1][2] - m_Control_Points[m_CP_Count][2]) * i / 10
						);
					glEnd();
					glDisable(GL_MAP1_VERTEX_3);
					glPopMatrix();
				}
			}
			else if (num == 1)
			{
				int i = 0;

				glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
				glMateriali(GL_FRONT, GL_SHININESS, 20);

				if (m_CP_Count % 2 == 0)
				{
					for (; i < m_CP_Count; i += 2)
					{
						glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, 3, &m_Control_Points_2[i][0]);
						glEnable(GL_MAP1_VERTEX_3);
						glMapGrid1f(10.0f, 0.0f, 1.0f);
						glEvalMesh1(GL_LINE, 0, 10);
						glDisable(GL_MAP1_VERTEX_3);
					}
					m_Control_Points_2[m_CP_Count][0] = m_Control_Points_2[0][0];
					m_Control_Points_2[m_CP_Count][1] = m_Control_Points_2[0][1];
					m_Control_Points_2[m_CP_Count][2] = m_Control_Points_2[0][2];
				}

				else
				{
					for (; i < m_CP_Count - 1; i += 2)
					{
						glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, 3, &m_Control_Points_2[i][0]);
						glEnable(GL_MAP1_VERTEX_3);
						glMapGrid1f(10.0f, 0.0f, 1.0f);
						glEvalMesh1(GL_LINE, 0, 10);
						glDisable(GL_MAP1_VERTEX_3);
					}
					m_Control_Points_2[m_CP_Count][0] = m_Control_Points_2[0][0];
					m_Control_Points_2[m_CP_Count][1] = m_Control_Points_2[0][1];
					m_Control_Points_2[m_CP_Count][2] = m_Control_Points_2[0][2];
					glPushMatrix();
					glTranslatef(m_Control_Points_2[m_CP_Count][0], m_Control_Points_2[m_CP_Count][1], m_Control_Points_2[m_CP_Count][2]);
					glColor3f(0.0f, 0.0f, 1.0f);
					glBegin(GL_LINE_STRIP);

					for (int i = 0; i <= 10; ++i)
						glVertex3f(
						(m_Control_Points_2[m_CP_Count - 1][0] - m_Control_Points_2[m_CP_Count][0]) * i / 10,
							(m_Control_Points_2[m_CP_Count - 1][1] - m_Control_Points_2[m_CP_Count][1]) * i / 10,
							(m_Control_Points_2[m_CP_Count - 1][2] - m_Control_Points_2[m_CP_Count][2]) * i / 10
						);
					glEnd();
					glDisable(GL_MAP1_VERTEX_3);
					glPopMatrix();
				}
			}
			else if (num == 2)
			{
				int i = 0;

				glMaterialfv(GL_FRONT, GL_AMBIENT, m_Material.Ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Material.Diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_Material.Specular);
				glMateriali(GL_FRONT, GL_SHININESS, 20);

				if (m_CP_Count % 2 == 0)
				{
					for (; i < m_CP_Count; i += 2)
					{
						glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, 3, &m_Control_Points_3[i][0]);
						glEnable(GL_MAP1_VERTEX_3);
						glMapGrid1f(10.0f, 0.0f, 1.0f);
						glEvalMesh1(GL_LINE, 0, 10);
						glDisable(GL_MAP1_VERTEX_3);
					}
					m_Control_Points_3[m_CP_Count][0] = m_Control_Points_3[0][0];
					m_Control_Points_3[m_CP_Count][1] = m_Control_Points_3[0][1];
					m_Control_Points_3[m_CP_Count][2] = m_Control_Points_3[0][2];
				}

				else
				{
					for (; i < m_CP_Count - 1; i += 2)
					{
						glMap1f(GL_MAP1_VERTEX_3, 0.0f, 1.0f, 3, 3, &m_Control_Points_3[i][0]);
						glEnable(GL_MAP1_VERTEX_3);
						glMapGrid1f(10.0f, 0.0f, 1.0f);
						glEvalMesh1(GL_LINE, 0, 10);
						glDisable(GL_MAP1_VERTEX_3);
					}
					m_Control_Points_3[m_CP_Count][0] = m_Control_Points_3[0][0];
					m_Control_Points_3[m_CP_Count][1] = m_Control_Points_3[0][1];
					m_Control_Points_3[m_CP_Count][2] = m_Control_Points_3[0][2];
					glPushMatrix();
					glTranslatef(m_Control_Points_3[m_CP_Count][0], m_Control_Points_3[m_CP_Count][1], m_Control_Points_3[m_CP_Count][2]);
					glColor3f(0.0f, 0.0f, 1.0f);
					glBegin(GL_LINE_STRIP);

					for (int i = 0; i <= 10; ++i)
						glVertex3f(
						(m_Control_Points_3[m_CP_Count - 1][0] - m_Control_Points_3[m_CP_Count][0]) * i / 10,
							(m_Control_Points_3[m_CP_Count - 1][1] - m_Control_Points_3[m_CP_Count][1]) * i / 10,
							(m_Control_Points_3[m_CP_Count - 1][2] - m_Control_Points_3[m_CP_Count][2]) * i / 10
						);
					glEnd();
					glDisable(GL_MAP1_VERTEX_3);
					glPopMatrix();
				}
			}
		}
	}

	void Draw_Tunnel()
	{
		if (m_is_CP_Making_Over)
		{
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Tunnel_Diffuse);
			glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, 3, 3, 0.0f, 1.0f, 9, 3, &m_Tunnel_Control_Points[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			glMapGrid2f(10, 0.0f, 1.0f, 10, 0.0f, 1.0f);
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glDisable(GL_MAP2_VERTEX_3);
			glMap2f(GL_MAP2_VERTEX_3, 0.0f, 1.0f, 3, 3, 0.0f, 1.0f, 9, 3, &m_Tunnel_Floor_Control_Points[0][0][0]);
			glEnable(GL_MAP2_VERTEX_3);
			glMapGrid2f(10, 0.0f, 1.0f, 10, 0.0f, 1.0f);
			glEvalMesh2(GL_FILL, 0, 10, 0, 10);
			glDisable(GL_MAP2_VERTEX_3);
			glPopMatrix();
		}
	}

	void Set_Tunnel_CPs()
	{
		m_Tunnel_Control_Points[0][0][0] = m_Control_Points[0][0];
		m_Tunnel_Control_Points[0][0][1] = m_Control_Points[0][1];
		m_Tunnel_Control_Points[0][0][2] = m_Control_Points[0][2];
		m_Tunnel_Control_Points[0][1][0] = m_Control_Points[1][0];
		m_Tunnel_Control_Points[0][1][1] = m_Control_Points[1][1];
		m_Tunnel_Control_Points[0][1][2] = m_Control_Points[1][2];
		m_Tunnel_Control_Points[0][2][0] = m_Control_Points[2][0];
		m_Tunnel_Control_Points[0][2][1] = m_Control_Points[2][1];
		m_Tunnel_Control_Points[0][2][2] = m_Control_Points[2][2];

		float t = 0.0f;
		float p0[3], p1[3], p2[3];
		float dir_vector[3]; // 접선 벡터의 방향.
		float dir_vector_size; // 접선 벡터의 크기.
		float v_1[3], v_2[3];
		float offset[3];

		p0[0] = m_Tunnel_Control_Points[0][0][0]; p0[1] = m_Tunnel_Control_Points[0][0][1]; p0[2] = m_Tunnel_Control_Points[0][0][2];
		p1[0] = m_Tunnel_Control_Points[0][1][0]; p1[1] = m_Tunnel_Control_Points[0][1][1]; p1[2] = m_Tunnel_Control_Points[0][1][2];
		p2[0] = m_Tunnel_Control_Points[0][2][0]; p2[1] = m_Tunnel_Control_Points[0][2][1]; p2[2] = m_Tunnel_Control_Points[0][2][2];

		t = 0.0f;
		
		v_1[0] = (1 - t) * p0[0] + t * p1[0]; v_1[1] = (1 - t) * p0[1] + t * p1[1]; v_1[2] = (1 - t) * p0[2] + t * p1[2];
		v_2[0] = (1 - t) * p1[0] + t * p2[0]; v_2[1] = (1 - t) * p1[1] + t * p2[1]; v_2[2] = (1 - t) * p1[2] + t * p2[2];

		dir_vector[0] = v_2[0] - v_1[0]; dir_vector[1] = v_2[1] - v_1[1]; dir_vector[2] = v_2[2] - v_1[2];
		dir_vector_size = sqrtf(dir_vector[0] * dir_vector[0] + dir_vector[1] * dir_vector[1] + dir_vector[2] * dir_vector[2]);
		dir_vector[0] /= dir_vector_size; dir_vector[1] /= dir_vector_size; dir_vector[2] /= dir_vector_size;

		// 외적을 하여 offset 방향을 찾는다.
		Vector_CrossProduct(offset, m_Up_Vector, dir_vector);

		m_Tunnel_Control_Points[1][0][0] = m_Tunnel_Control_Points[0][0][0];
		m_Tunnel_Control_Points[1][0][1] = m_Tunnel_Control_Points[0][0][1] + OBJECT_SIZE * 5.0f;
		m_Tunnel_Control_Points[1][0][2] = m_Tunnel_Control_Points[0][0][2];
		m_Tunnel_Control_Points[2][0][0] = m_Tunnel_Control_Points[0][0][0] - offset[0] * OBJECT_SIZE * 1.5f;
		m_Tunnel_Control_Points[2][0][1] = m_Tunnel_Control_Points[0][0][1] - OBJECT_SIZE * 0.3f;
		m_Tunnel_Control_Points[2][0][2] = m_Tunnel_Control_Points[0][0][2] - offset[2] * OBJECT_SIZE * 1.5f;

		m_Tunnel_Control_Points[0][0][0] = m_Tunnel_Control_Points[1][0][0] + offset[0] * OBJECT_SIZE * 1.5f;
		m_Tunnel_Control_Points[0][0][1] = m_Tunnel_Control_Points[2][0][1];
		m_Tunnel_Control_Points[0][0][2] = m_Tunnel_Control_Points[1][0][2] + offset[2] * OBJECT_SIZE * 1.5f;
		// ================================

		t = 0.5f;
		v_1[0] = (1 - t) * p0[0] + t * p1[0]; v_1[1] = (1 - t) * p0[1] + t * p1[1]; v_1[2] = (1 - t) * p0[2] + t * p1[2];
		v_2[0] = (1 - t) * p1[0] + t * p2[0]; v_2[1] = (1 - t) * p1[1] + t * p2[1]; v_2[2] = (1 - t) * p1[2] + t * p2[2];

		dir_vector[0] = v_2[0] - v_1[0]; dir_vector[1] = v_2[1] - v_1[1]; dir_vector[2] = v_2[2] - v_1[2];
		dir_vector_size = sqrtf(dir_vector[0] * dir_vector[0] + dir_vector[1] * dir_vector[1] + dir_vector[2] * dir_vector[2]);
		dir_vector[0] /= dir_vector_size; dir_vector[1] /= dir_vector_size; dir_vector[2] /= dir_vector_size;

		// 외적을 하여 offset 방향을 찾는다.
		Vector_CrossProduct(offset, m_Up_Vector, dir_vector);

		m_Tunnel_Control_Points[1][1][0] = m_Tunnel_Control_Points[0][1][0];
		m_Tunnel_Control_Points[1][1][1] = m_Tunnel_Control_Points[0][1][1] + OBJECT_SIZE * 5.0f;
		m_Tunnel_Control_Points[1][1][2] = m_Tunnel_Control_Points[0][1][2];
		m_Tunnel_Control_Points[2][1][0] = m_Tunnel_Control_Points[0][1][0] - offset[0] * OBJECT_SIZE * 1.5f;
		m_Tunnel_Control_Points[2][1][1] = m_Tunnel_Control_Points[0][1][1] - OBJECT_SIZE * 0.3f;
		m_Tunnel_Control_Points[2][1][2] = m_Tunnel_Control_Points[0][1][2] - offset[2] * OBJECT_SIZE * 1.5f;

		m_Tunnel_Control_Points[0][1][0] = m_Tunnel_Control_Points[1][1][0] + offset[0] * OBJECT_SIZE * 1.5f;
		m_Tunnel_Control_Points[0][1][1] = m_Tunnel_Control_Points[2][1][1];
		m_Tunnel_Control_Points[0][1][2] = m_Tunnel_Control_Points[1][1][2] + offset[2] * OBJECT_SIZE * 1.5f;
		// ================================

		t = 1.0f;
		v_1[0] = (1 - t) * p0[0] + t * p1[0]; v_1[1] = (1 - t) * p0[1] + t * p1[1]; v_1[2] = (1 - t) * p0[2] + t * p1[2];
		v_2[0] = (1 - t) * p1[0] + t * p2[0]; v_2[1] = (1 - t) * p1[1] + t * p2[1]; v_2[2] = (1 - t) * p1[2] + t * p2[2];

		dir_vector[0] = v_2[0] - v_1[0]; dir_vector[1] = v_2[1] - v_1[1]; dir_vector[2] = v_2[2] - v_1[2];
		dir_vector_size = sqrtf(dir_vector[0] * dir_vector[0] + dir_vector[1] * dir_vector[1] + dir_vector[2] * dir_vector[2]);
		dir_vector[0] /= dir_vector_size; dir_vector[1] /= dir_vector_size; dir_vector[2] /= dir_vector_size;

		// 외적을 하여 offset 방향을 찾는다.
		Vector_CrossProduct(offset, m_Up_Vector, dir_vector);

		m_Tunnel_Control_Points[1][2][0] = m_Tunnel_Control_Points[0][2][0];
		m_Tunnel_Control_Points[1][2][1] = m_Tunnel_Control_Points[0][2][1] + OBJECT_SIZE * 5.0f;
		m_Tunnel_Control_Points[1][2][2] = m_Tunnel_Control_Points[0][2][2];
		m_Tunnel_Control_Points[2][2][0] = m_Tunnel_Control_Points[0][2][0] - offset[0] * OBJECT_SIZE * 1.5f;
		m_Tunnel_Control_Points[2][2][1] = m_Tunnel_Control_Points[0][2][1] - OBJECT_SIZE * 0.3f;
		m_Tunnel_Control_Points[2][2][2] = m_Tunnel_Control_Points[0][2][2] - offset[2] * OBJECT_SIZE * 1.5f;

		m_Tunnel_Control_Points[0][2][0] = m_Tunnel_Control_Points[1][2][0] + offset[0] * OBJECT_SIZE * 1.5f;
		m_Tunnel_Control_Points[0][2][1] = m_Tunnel_Control_Points[2][2][1];
		m_Tunnel_Control_Points[0][2][2] = m_Tunnel_Control_Points[1][2][2] + offset[2] * OBJECT_SIZE * 1.5f;
		// ================================


		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
			{
				m_Tunnel_Floor_Control_Points[i][j][0] = m_Tunnel_Control_Points[i][j][0];
				m_Tunnel_Floor_Control_Points[i][j][1] = m_Tunnel_Control_Points[0][j][1];
				m_Tunnel_Floor_Control_Points[i][j][2] = m_Tunnel_Control_Points[i][j][2];
			}
	}

	void Set_Other_Lines()
	{
		float t = 0.0f;
		float p0[3], p1[3], p2[3];
		float dir_vector[3]; // 접선 벡터의 방향.
		float dir_vector_size; // 접선 벡터의 크기.
		float v_1[3], v_2[3];
		float offset[3];

		for (int i = 0; i < m_CP_Count; i += 2)
		{
			t = 0.0f;
			p0[0] = m_Control_Points[i][0]; p0[1] = m_Control_Points[i][1]; p0[2] = m_Control_Points[i][2];
			p1[0] = m_Control_Points[i + 1][0]; p1[1] = m_Control_Points[i + 1][1]; p1[2] = m_Control_Points[i + 1][2];

			if (i + 2 <= m_CP_Count) // 곡선일 경우
			{
				// 곡선 상의 지점을 찾는다.
				p2[0] = m_Control_Points[i + 2][0]; p2[1] = m_Control_Points[i + 2][1]; p2[2] = m_Control_Points[i + 2][2];
				
				v_1[0] = (1 - t) * p0[0] + t * p1[0]; v_1[1] = (1 - t) * p0[1] + t * p1[1]; v_1[2] = (1 - t) * p0[2] + t * p1[2];
				v_2[0] = (1 - t) * p1[0] + t * p2[0]; v_2[1] = (1 - t) * p1[1] + t * p2[1]; v_2[2] = (1 - t) * p1[2] + t * p2[2];

				dir_vector[0] = v_2[0] - v_1[0]; dir_vector[1] = v_2[1] - v_1[1]; dir_vector[2] = v_2[2] - v_1[2];
				dir_vector_size = sqrtf(dir_vector[0] * dir_vector[0] + dir_vector[1] * dir_vector[1] + dir_vector[2] * dir_vector[2]);
				dir_vector[0] /= dir_vector_size; dir_vector[1] /= dir_vector_size; dir_vector[2] /= dir_vector_size;
			}

			else // 마지막 곡선이 직선일 경우.
			{
				// 직선이므로 접선 = 직선이다.
				dir_vector[0] = p1[0] - p0[0]; dir_vector[1] = p1[1] - p0[1]; dir_vector[2] = p1[2] - p0[2];
				dir_vector_size = sqrtf(dir_vector[0] * dir_vector[0] + dir_vector[1] * dir_vector[1] + dir_vector[2] * dir_vector[2]);
				dir_vector[0] /= dir_vector_size; dir_vector[1] /= dir_vector_size; dir_vector[2] /= dir_vector_size;
			};

			// 외적을 하여 offset 방향을 찾는다.
			Vector_CrossProduct(offset, m_Up_Vector, dir_vector);

			m_Control_Points_2[i][0] = m_Control_Points[i][0] + offset[0] * OBJECT_SIZE * 0.5f;
			m_Control_Points_2[i][1] = m_Control_Points[i][1];
			m_Control_Points_2[i][2] = m_Control_Points[i][2] + offset[2] * OBJECT_SIZE * 0.5f;
			m_Control_Points_3[i][0] = m_Control_Points[i][0] - offset[0] * OBJECT_SIZE * 0.5f;
			m_Control_Points_3[i][1] = m_Control_Points[i][1];
			m_Control_Points_3[i][2] = m_Control_Points[i][2] - offset[2] * OBJECT_SIZE * 0.5f;

			// ===================================
			
			if (i + 2 <= m_CP_Count) // 곡선일 경우
			{
				t = 0.5f;
				p0[0] = m_Control_Points[i][0]; p0[1] = m_Control_Points[i][1]; p0[2] = m_Control_Points[i][2];
				p1[0] = m_Control_Points[i + 1][0]; p1[1] = m_Control_Points[i + 1][1]; p1[2] = m_Control_Points[i + 1][2];
				p2[0] = m_Control_Points[i + 2][0]; p2[1] = m_Control_Points[i + 2][1]; p2[2] = m_Control_Points[i + 2][2];

				v_1[0] = (1 - t) * p0[0] + t * p1[0]; v_1[1] = (1 - t) * p0[1] + t * p1[1]; v_1[2] = (1 - t) * p0[2] + t * p1[2];
				v_2[0] = (1 - t) * p1[0] + t * p2[0]; v_2[1] = (1 - t) * p1[1] + t * p2[1]; v_2[2] = (1 - t) * p1[2] + t * p2[2];

				dir_vector[0] = v_2[0] - v_1[0]; dir_vector[1] = v_2[1] - v_1[1]; dir_vector[2] = v_2[2] - v_1[2];
				dir_vector_size = sqrtf(dir_vector[0] * dir_vector[0] + dir_vector[1] * dir_vector[1] + dir_vector[2] * dir_vector[2]);
				dir_vector[0] /= dir_vector_size; dir_vector[1] /= dir_vector_size; dir_vector[2] /= dir_vector_size;
			}

			// 외적을 하여 offset 방향을 찾는다.
			Vector_CrossProduct(offset, m_Up_Vector, dir_vector);

			m_Control_Points_2[i + 1][0] = m_Control_Points[i + 1][0] + offset[0] * OBJECT_SIZE * 0.5f;
			m_Control_Points_2[i + 1][1] = m_Control_Points[i + 1][1];
			m_Control_Points_2[i + 1][2] = m_Control_Points[i + 1][2] + offset[2] * OBJECT_SIZE * 0.5f;
			m_Control_Points_3[i + 1][0] = m_Control_Points[i + 1][0] - offset[0] * OBJECT_SIZE * 0.5f;
			m_Control_Points_3[i + 1][1] = m_Control_Points[i + 1][1];
			m_Control_Points_3[i + 1][2] = m_Control_Points[i + 1][2] - offset[2] * OBJECT_SIZE * 0.5f;

			// ===================================

			t = 1.0f;
			p0[0] = m_Control_Points[i][0]; p0[1] = m_Control_Points[i][1]; p0[2] = m_Control_Points[i][2];
			p1[0] = m_Control_Points[i + 1][0]; p1[1] = m_Control_Points[i + 1][1]; p1[2] = m_Control_Points[i + 1][2];

			if (i + 2 <= m_CP_Count) // 곡선일 경우
			{
				// 곡선 상의 지점을 찾는다.
				p2[0] = m_Control_Points[i + 2][0]; p2[1] = m_Control_Points[i + 2][1]; p2[2] = m_Control_Points[i + 2][2];

				v_1[0] = (1 - t) * p0[0] + t * p1[0]; v_1[1] = (1 - t) * p0[1] + t * p1[1]; v_1[2] = (1 - t) * p0[2] + t * p1[2];
				v_2[0] = (1 - t) * p1[0] + t * p2[0]; v_2[1] = (1 - t) * p1[1] + t * p2[1]; v_2[2] = (1 - t) * p1[2] + t * p2[2];

				dir_vector[0] = v_2[0] - v_1[0]; dir_vector[1] = v_2[1] - v_1[1]; dir_vector[2] = v_2[2] - v_1[2];
				dir_vector_size = sqrtf(dir_vector[0] * dir_vector[0] + dir_vector[1] * dir_vector[1] + dir_vector[2] * dir_vector[2]);
				dir_vector[0] /= dir_vector_size; dir_vector[1] /= dir_vector_size; dir_vector[2] /= dir_vector_size;
			}

			else // 마지막 곡선이 직선일 경우.
			{
				// 직선이므로 접선 = 직선이다.
				dir_vector[0] = p1[0] - p0[0]; dir_vector[1] = p1[1] - p0[1]; dir_vector[2] = p1[2] - p0[2];
				dir_vector_size = sqrtf(dir_vector[0] * dir_vector[0] + dir_vector[1] * dir_vector[1] + dir_vector[2] * dir_vector[2]);
				dir_vector[0] /= dir_vector_size; dir_vector[1] /= dir_vector_size; dir_vector[2] /= dir_vector_size;
			};

			// 외적을 하여 offset 방향을 찾는다.
			Vector_CrossProduct(offset, m_Up_Vector, dir_vector);

			m_Control_Points_2[i + 2][0] = m_Control_Points[i + 2][0] + offset[0] * OBJECT_SIZE * 0.5f;
			m_Control_Points_2[i + 2][1] = m_Control_Points[i + 2][1];
			m_Control_Points_2[i + 2][2] = m_Control_Points[i + 2][2] + offset[2] * OBJECT_SIZE * 0.5f;
			m_Control_Points_3[i + 2][0] = m_Control_Points[i + 2][0] - offset[0] * OBJECT_SIZE * 0.5f;
			m_Control_Points_3[i + 2][1] = m_Control_Points[i + 2][1];
			m_Control_Points_3[i + 2][2] = m_Control_Points[i + 2][2] - offset[2] * OBJECT_SIZE * 0.5f;
		}
	}

	void Set_CPs_OBB()
	{
		OBB* temp_OBB;
		Vector3 temp_V;
		for (int i = 0; i < m_CP_Count; i += 2)
		{
			temp_OBB = new OBB();
			temp_V.x = m_Control_Points[i][0]; temp_V.y = m_Control_Points[i][1]; temp_V.z = m_Control_Points[i][2];
			temp_OBB->OBB_Init(temp_V, 0.5f, m_Control_Points[i][1] / OBJECT_SIZE, 0.5f);
			m_OBBs.push_back(temp_OBB);
		}
	}

	void Draw_Train(const int& train_num)
	{
		if (m_is_CP_Making_Over)
		{
			glPushMatrix();
			Set_Pos_and_Rot_On_the_Curve(m_Train_CP_index[train_num], m_Train_Curv_index[train_num], m_Train_Speed, m_Train_Position[train_num], m_Train_Rotation_Value[train_num], m_Train_Rotation_LookAt, m_Train_Rotation_Axis[train_num], train_num);
			glTranslatef(m_Train_Position[train_num][0], m_Train_Position[train_num][1], m_Train_Position[train_num][2]);
			glRotatef(m_Train_Rotation_Value[train_num], m_Train_Rotation_Axis[train_num][0], m_Train_Rotation_Axis[train_num][1], m_Train_Rotation_Axis[train_num][2]);
			
			glMaterialfv(GL_FRONT, GL_AMBIENT, m_Train_Material.Ambient);
			glMaterialfv(GL_FRONT, GL_SPECULAR, m_Train_Material.Specular);
			glMateriali(GL_FRONT, GL_SHININESS, 20);
			
			{
				glPushMatrix(); // 몸체 그리기
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Train_Material.Diffuse);

				glPushMatrix(); // 차체
				glScalef(m_Train_Scale[0], m_Train_Scale[1], m_Train_Scale[2]);
				glutSolidCube(OBJECT_SIZE);
				glPopMatrix();

				glPushMatrix(); // 의자
				glTranslatef(0.0f, OBJECT_SIZE * m_Train_Scale[1] * 0.3f, OBJECT_SIZE * m_Train_Scale[2] * 0.3f);
				glScalef(m_Train_Scale[0], m_Train_Scale[1], m_Train_Scale[2] * 0.3f);
				glutSolidCube(OBJECT_SIZE);
				glPopMatrix();

				glPushMatrix(); // 사람
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Train_Human_Diffuse);
				{
					glPushMatrix();
					glTranslatef(0.0f, OBJECT_SIZE * m_Train_Scale[1] * 0.3f, OBJECT_SIZE * m_Train_Scale[2] * 0.1f);
					glScalef(m_Train_Scale[0] * 0.5f, m_Train_Scale[1], m_Train_Scale[2] * 0.15f);
					glutSolidCube(OBJECT_SIZE); // 몸통
					glPopMatrix();
				}
				{
					glPushMatrix();
					glTranslatef(0.0f, OBJECT_SIZE * m_Train_Scale[1], OBJECT_SIZE * m_Train_Scale[2] * 0.1f);
					glScalef(m_Train_Scale[0] * 0.25f, m_Train_Scale[1] * 0.25f, m_Train_Scale[2] * 0.25f);
					glutSolidSphere(OBJECT_SIZE * 0.5f, 20, 20); // 머리
					glPopMatrix();
				}
				{
					glPushMatrix();
					glTranslatef(-OBJECT_SIZE * m_Train_Scale[0] * 0.35f, OBJECT_SIZE * m_Train_Scale[1] * 0.9f, OBJECT_SIZE * m_Train_Scale[2] * 0.05f);
					glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
					glScalef(m_Train_Scale[0] * 0.05f, m_Train_Scale[1] * 0.4f, m_Train_Scale[2] * 0.05f);
					glutSolidCube(OBJECT_SIZE); // 왼팔
					glPopMatrix();
				}
				{
					glPushMatrix();
					glTranslatef(OBJECT_SIZE * m_Train_Scale[0] * 0.35f, OBJECT_SIZE * m_Train_Scale[1] * 0.9f, OBJECT_SIZE * m_Train_Scale[2] * 0.05f);
					glRotatef(-45.0f, 0.0f, 0.0f, 1.0f);
					glScalef(m_Train_Scale[0] * 0.05f, m_Train_Scale[1] * 0.4f, m_Train_Scale[2] * 0.05f);
					glutSolidCube(OBJECT_SIZE); // 오른팔
					glPopMatrix();
				}
				glPopMatrix();

				glPopMatrix();
			}
			{
				glPushMatrix(); // 범퍼 그리기
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Train_Front_Bar_Diffuse);

				glPushMatrix();
				glTranslatef(0.0f, 0.0f, -OBJECT_SIZE * m_Train_Scale[2] * 0.5f);
				glScalef(m_Train_Scale[0], m_Train_Scale[1] * 0.4f, m_Train_Scale[2] * 0.2f);
				glutSolidCube(OBJECT_SIZE); // 앞범퍼
				glPopMatrix();

				glPushMatrix();
				glTranslatef(0.0f, 0.0f, OBJECT_SIZE * m_Train_Scale[2] * 0.5f);
				glScalef(m_Train_Scale[0], m_Train_Scale[1] * 0.2f, m_Train_Scale[2] * 0.2f);
				glutSolidCube(OBJECT_SIZE); // 뒷범퍼 1
				glPopMatrix();

				glPushMatrix();
				glTranslatef(0.0f, OBJECT_SIZE * m_Train_Scale[1] * 0.4f, OBJECT_SIZE * m_Train_Scale[2] * 0.5f);
				glScalef(m_Train_Scale[0], m_Train_Scale[1] * 0.2f, m_Train_Scale[2] * 0.2f);
				glutSolidCube(OBJECT_SIZE); // 뒷범퍼 2
				glPopMatrix();

				glPopMatrix();
			}
			{
				glPushMatrix(); // 사이드바 그리기
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Train_Side_Bar_Diffuse);

				glPushMatrix();
				glTranslatef(-OBJECT_SIZE * m_Train_Scale[0] * 0.5f, OBJECT_SIZE * m_Train_Scale[1] * 0.25f, 0.0f);
				glScalef(m_Train_Scale[0] * 0.2f, m_Train_Scale[1] * 0.2f, m_Train_Scale[2]);
				glutSolidCube(OBJECT_SIZE); // 왼쪽
				glPopMatrix();

				glPushMatrix();
				glTranslatef(OBJECT_SIZE * m_Train_Scale[0] * 0.5f, OBJECT_SIZE * m_Train_Scale[1] * 0.25f, 0.0f);
				glScalef(m_Train_Scale[0] * 0.2f, m_Train_Scale[1] * 0.2f, m_Train_Scale[2]);
				glutSolidCube(OBJECT_SIZE); // 오른쪽
				glPopMatrix();

				glPopMatrix();
			}
			{
				glPushMatrix(); // 바퀴 그리기
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Train_Wheel_Diffuse);

				glPushMatrix();
				glTranslatef(-OBJECT_SIZE * m_Train_Scale[0] * 0.5f, -OBJECT_SIZE * m_Train_Scale[1] * 0.2f, -OBJECT_SIZE * m_Train_Scale[2] * 0.3f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				glScalef(m_Train_Scale[0] * 0.3f, m_Train_Scale[1] * 0.3f, m_Train_Scale[2] * 0.3f);
				
				glutSolidTorus(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 20, 20); // 왼쪽 1
				glPopMatrix();

				glPushMatrix();
				glTranslatef(-OBJECT_SIZE * m_Train_Scale[0] * 0.5f, -OBJECT_SIZE * m_Train_Scale[1] * 0.2f, OBJECT_SIZE * m_Train_Scale[2] * 0.4f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				glScalef(m_Train_Scale[0] * 0.3f, m_Train_Scale[1] * 0.3f, m_Train_Scale[2] * 0.3f);
				glMaterialfv(GL_FRONT, GL_AMBIENT, m_Train_Material.Ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Train_Wheel_Diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_Train_Material.Specular);
				glMateriali(GL_FRONT, GL_SHININESS, 20);
				glutSolidTorus(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 20, 20); // 왼쪽 2
				glPopMatrix();

				glPushMatrix();
				glTranslatef(OBJECT_SIZE * m_Train_Scale[0] * 0.5f, -OBJECT_SIZE * m_Train_Scale[1] * 0.2f, -OBJECT_SIZE * m_Train_Scale[2] * 0.3f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				glScalef(m_Train_Scale[0] * 0.3f, m_Train_Scale[1] * 0.3f, m_Train_Scale[2] * 0.3f);
				glMaterialfv(GL_FRONT, GL_AMBIENT, m_Train_Material.Ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Train_Wheel_Diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_Train_Material.Specular);
				glMateriali(GL_FRONT, GL_SHININESS, 20);
				glutSolidTorus(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 20, 20); // 오른쪽 1
				glPopMatrix();

				glPushMatrix();
				glTranslatef(OBJECT_SIZE * m_Train_Scale[0] * 0.5f, -OBJECT_SIZE * m_Train_Scale[1] * 0.2f, OBJECT_SIZE * m_Train_Scale[2] * 0.4f);
				glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
				glScalef(m_Train_Scale[0] * 0.3f, m_Train_Scale[1] * 0.3f, m_Train_Scale[2] * 0.3f);
				glMaterialfv(GL_FRONT, GL_AMBIENT, m_Train_Material.Ambient);
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Train_Wheel_Diffuse);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_Train_Material.Specular);
				glMateriali(GL_FRONT, GL_SHININESS, 20);
				glutSolidTorus(OBJECT_SIZE * 0.5f, OBJECT_SIZE, 20, 20); // 오른쪽 2
				glPopMatrix();

				glPopMatrix();
			}
			glPopMatrix();
		}
	}

	void Init_Train()
	{
		m_Train_Position[0][0] = 0.0f; m_Train_Position[0][1] = 0.0f; m_Train_Position[0][2] = 0.0f;
		m_Train_Rotation_Value[0] = 0.0f;
		m_Train_Rotation_Axis[0][0] = 0.0f; m_Train_Rotation_Axis[0][1] = 1.0f; m_Train_Rotation_Axis[0][2] = 0.0f;

		m_Train_Position[1][0] = 0.0f; m_Train_Position[1][1] = 0.0f; m_Train_Position[1][2] = 0.0f;
		m_Train_Rotation_Value[1] = 0.0f;
		m_Train_Rotation_Axis[1][0] = 0.0f; m_Train_Rotation_Axis[1][1] = 1.0f; m_Train_Rotation_Axis[1][2] = 0.0f;

		m_Train_Position[2][0] = 0.0f; m_Train_Position[2][1] = 0.0f; m_Train_Position[2][2] = 0.0f;
		m_Train_Rotation_Value[2] = 0.0f;
		m_Train_Rotation_Axis[2][0] = 0.0f; m_Train_Rotation_Axis[2][1] = 1.0f; m_Train_Rotation_Axis[2][2] = 0.0f;

		m_Train_CP_index[0] = 0; m_Train_CP_index[1] = 0; m_Train_CP_index[2] = 0;
		m_Train_Curv_index[0] = 20; m_Train_Curv_index[1] = 10; m_Train_Curv_index[2] = 0;

		m_Train_Rotation_LookAt[0] = 0.0f; m_Train_Rotation_LookAt[1] = 0.0f; m_Train_Rotation_LookAt[2] = -1.0f;

		m_Train_Scale[0] = 0.6f; m_Train_Scale[1] = 0.6f; m_Train_Scale[2] = 1.0f;

		m_Train_Material.Ambient[0] = 0.3f; m_Train_Material.Ambient[1] = 0.3f; m_Train_Material.Ambient[2] = 0.3f; m_Train_Material.Ambient[3] = 1.0f;
		m_Train_Material.Diffuse[0] = 0.3f; m_Train_Material.Diffuse[1] = 0.3f; m_Train_Material.Diffuse[2] = 0.9f; m_Train_Material.Diffuse[3] = 1.0f;
		m_Train_Material.Specular[0] = 1.0f; m_Train_Material.Specular[1] = 1.0f; m_Train_Material.Specular[2] = 1.0f; m_Train_Material.Specular[3] = 1.0f;
		
		m_Train_Front_Bar_Diffuse[0] = 0.3f; m_Train_Front_Bar_Diffuse[1] = 0.8f; m_Train_Front_Bar_Diffuse[2] = 0.3f; m_Train_Front_Bar_Diffuse[3] = 1.0f;
		m_Train_Side_Bar_Diffuse[0] = 0.8f; m_Train_Side_Bar_Diffuse[1] = 0.3f; m_Train_Side_Bar_Diffuse[2] = 0.3f; m_Train_Side_Bar_Diffuse[3] = 1.0f;
		m_Train_Wheel_Diffuse[0] = 0.6f; m_Train_Wheel_Diffuse[1] = 0.6f; m_Train_Wheel_Diffuse[2] = 0.6f; m_Train_Wheel_Diffuse[3] = 1.0f;
		m_Train_Human_Diffuse[0] = 0.9f; m_Train_Human_Diffuse[1] = 0.9f; m_Train_Human_Diffuse[2] = 0.6f; m_Train_Human_Diffuse[3] = 1.0f;

		m_Look_Vector_For_Camera[0] = 0.0f; m_Look_Vector_For_Camera[1] = 0.0f; m_Look_Vector_For_Camera[2] = -1.0f;
	}

	void Setting_Train_to_CP_Point(const int& train_num, const int& cp_index)
	{
		m_Train_Position[train_num][0] = m_Control_Points[cp_index][0];
		m_Train_Position[train_num][1] = m_Control_Points[cp_index][1];
		m_Train_Position[train_num][2] = m_Control_Points[cp_index][2];
	}

	void Set_Pos_and_Rot_On_the_Curve(int& start_cp_index, int& curv_divine_index, const int& curv_divine_num, float* pos, float& rot, float* look_at, float* rot_axis, const int& train_num)
	{
		if (curv_divine_index <= curv_divine_num)
		{
			float t = (float)curv_divine_index / (float)curv_divine_num;

			float p0[3], p1[3], p2[3];

			p0[0] = m_Control_Points[start_cp_index][0]; p0[1] = m_Control_Points[start_cp_index][1]; p0[2] = m_Control_Points[start_cp_index][2];
			p1[0] = m_Control_Points[start_cp_index + 1][0]; p1[1] = m_Control_Points[start_cp_index + 1][1]; p1[2] = m_Control_Points[start_cp_index + 1][2];

			float dir_vector[3]; // 접선 벡터의 방향.
			float dir_vector_size; // 접선 벡터의 크기.

			if (start_cp_index + 2 <= m_CP_Count) // 곡선일 경우
			{
				// 곡선 상의 지점을 찾는다.
				p2[0] = m_Control_Points[start_cp_index + 2][0]; p2[1] = m_Control_Points[start_cp_index + 2][1]; p2[2] = m_Control_Points[start_cp_index + 2][2];
				pos[0] = (1 - t) * (1 - t) * p0[0] + 2 * t * (1 - t) * p1[0] + t * t * p2[0];
				pos[1] = (1 - t) * (1 - t) * p0[1] + 2 * t * (1 - t) * p1[1] + t * t * p2[1];
				pos[2] = (1 - t) * (1 - t) * p0[2] + 2 * t * (1 - t) * p1[2] + t * t * p2[2];

				float v_1[3], v_2[3]; // 곡선에서 해당 지점의 접선 벡터를 구한다.
				v_1[0] = (1 - t) * p0[0] + t * p1[0]; v_1[1] = (1 - t) * p0[1] + t * p1[1]; v_1[2] = (1 - t) * p0[2] + t * p1[2];
				v_2[0] = (1 - t) * p1[0] + t * p2[0]; v_2[1] = (1 - t) * p1[1] + t * p2[1]; v_2[2] = (1 - t) * p1[2] + t * p2[2];

				dir_vector[0] = v_2[0] - v_1[0]; dir_vector[1] = v_2[1] - v_1[1]; dir_vector[2] = v_2[2] - v_1[2];
				dir_vector_size = sqrtf(dir_vector[0] * dir_vector[0] + dir_vector[1] * dir_vector[1] + dir_vector[2] * dir_vector[2]);
				dir_vector[0] /= dir_vector_size; dir_vector[1] /= dir_vector_size; dir_vector[2] /= dir_vector_size;
			}

			else // 마지막 곡선이 직선일 경우.
			{
				// 직선 상의 지점을 찾는다.
				pos[0] = (1 - t) * p0[0] + t * p1[0];
				pos[1] = (1 - t) * p0[1] + t * p1[1];
				pos[2] = (1 - t) * p0[2] + t * p1[2];

				// 직선이므로 접선 = 직선이다.
				dir_vector[0] = p1[0] - p0[0]; dir_vector[1] = p1[1] - p0[1]; dir_vector[2] = p1[2] - p0[2];
				dir_vector_size = sqrtf(dir_vector[0] * dir_vector[0] + dir_vector[1] * dir_vector[1] + dir_vector[2] * dir_vector[2]);
				dir_vector[0] /= dir_vector_size; dir_vector[1] /= dir_vector_size; dir_vector[2] /= dir_vector_size;
			}

			float dot; // 구한 방향 벡터와 현재 객체가 보고 있는 방향 벡터를 내적한다.
			Vector_DotProduct(dot, look_at, dir_vector, false);

			// 내적 값에 "arc cos"를 적용하여 각도를 구한다.
			rot = acosf(dot) * (180.0f / 3.1416f);

			// 이번에는 외적을 하여 축을 찾는다.
			Vector_CrossProduct(rot_axis, look_at, dir_vector);

			if (train_num == 0)
			{
				Init_Head();
				m_Look_Vector_For_Camera[0] = dir_vector[0]; m_Look_Vector_For_Camera[1] = dir_vector[1]; m_Look_Vector_For_Camera[2] = dir_vector[2];
			}

			++curv_divine_index;
			if (curv_divine_index > curv_divine_num)
			{
				curv_divine_index = 0;
				start_cp_index = start_cp_index + 2 < m_CP_Count ? start_cp_index + 2 : 0;
			}
		}
	}

	void Vector_DotProduct(float& result, const float* p1, const float* p2, const bool& is_it_Normal)
	{
		if (is_it_Normal)
		{
			result = (p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2]) /
				(sqrtf(p1[0] * p1[0] + p1[1] * p1[1] + p1[2] * p1[2]) * sqrtf(p2[0] * p2[0] + p2[1] * p2[1] + p2[2] * p2[2]));
		}
		else
		{
			result = (p1[0] * p2[0] + p1[1] * p2[1] + p1[2] * p2[2]);
		}
	}

	void Vector_CrossProduct(float* result, const float* p1, const float* p2)
	{
		result[0] = p1[1] * p2[2] - p1[2] * p2[1];
		result[1] = p1[2] * p2[0] - p1[0] * p2[2];
		result[2] = p1[0] * p2[1] - p1[1] * p2[0];
	}
};