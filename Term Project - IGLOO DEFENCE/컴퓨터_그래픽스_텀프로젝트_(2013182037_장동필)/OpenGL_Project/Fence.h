#pragma once
#include "Object.h"

class Fence : Object
{
private:
	Tree* m_Tree_Model;
	OBB* m_OBBs[4];

public:
	virtual OBB** const Get_OBBs() { return m_OBBs; }

public:
	Fence() : Object() { Init_Fence(); }
	~Fence()
	{
		if (m_Tree_Model != nullptr) delete m_Tree_Model;
		for(int i = 0; i < 4; ++i) if (m_OBBs[i] != nullptr) delete m_OBBs[i];
	}
	
	void Init_Fence()
	{
		m_Tree_Model = new Tree();

		m_Object_Type = FENCE_OBJECT;

		Vector3 temp_Pos;
		float temp_Scale[3];

		m_OBBs[0] = new OBB();
		temp_Pos.x = -FLOOR_WIDTH * FLOOR_PIECE_WIDTH_SIZE;
		temp_Pos.y = 0.0f; temp_Pos.z = 0.0f;
		temp_Scale[0] = 1.0f; temp_Scale[1] = 1.0f; temp_Scale[2] = (FLOOR_HEIGHT * FLOOR_PIECE_HEIGHT_SIZE) / OBJECT_SIZE;
		m_OBBs[0]->OBB_Init(temp_Pos, temp_Scale[0], temp_Scale[1], temp_Scale[2]);

		m_OBBs[1] = new OBB();
		temp_Pos.x = FLOOR_WIDTH* FLOOR_PIECE_WIDTH_SIZE;
		m_OBBs[1]->OBB_Init(temp_Pos, temp_Scale[0], temp_Scale[1], temp_Scale[2]);

		m_OBBs[2] = new OBB();
		temp_Pos.z = -FLOOR_HEIGHT * FLOOR_PIECE_HEIGHT_SIZE; 
		temp_Pos.y = 0.0f; temp_Pos.x = 0.0f;
		temp_Scale[0] = (FLOOR_WIDTH * FLOOR_PIECE_WIDTH_SIZE) / OBJECT_SIZE; temp_Scale[1] = 1.0f; temp_Scale[2] = 1.0f;
		m_OBBs[2]->OBB_Init(temp_Pos, temp_Scale[0], temp_Scale[1], temp_Scale[2]);

		m_OBBs[3] = new OBB();
		temp_Pos.z = FLOOR_HEIGHT * FLOOR_PIECE_HEIGHT_SIZE;
		m_OBBs[3]->OBB_Init(temp_Pos, temp_Scale[0], temp_Scale[1], temp_Scale[2]);
	}

	virtual void Draw()
	{
		glPushMatrix();

		glTranslatef(m_Position.x, m_Position.y, m_Position.z);
		glMultMatrixf(m_Rotate_Transform_Matrix);
		glScalef(m_Scale_X, m_Scale_Y, m_Scale_Z);

		for (int i = -FLOOR_WIDTH; i < FLOOR_WIDTH; i += 2)
		{
			glPushMatrix();
			glTranslatef(i * FLOOR_PIECE_WIDTH_SIZE, 0.0f, -FLOOR_HEIGHT * FLOOR_PIECE_HEIGHT_SIZE);
			m_Tree_Model->Draw();
			glPopMatrix();
			glPushMatrix();
			glTranslatef(i * FLOOR_PIECE_WIDTH_SIZE, 0.0f, FLOOR_HEIGHT * FLOOR_PIECE_HEIGHT_SIZE);
			m_Tree_Model->Draw();
			glPopMatrix();
		}

		for (int i = -FLOOR_HEIGHT; i < FLOOR_HEIGHT; i += 2)
		{
			glPushMatrix();
			glTranslatef(-FLOOR_WIDTH * FLOOR_PIECE_WIDTH_SIZE, 0.0f, i * FLOOR_PIECE_HEIGHT_SIZE);
			m_Tree_Model->Draw();
			glPopMatrix();
			glPushMatrix();
			glTranslatef(FLOOR_WIDTH * FLOOR_PIECE_WIDTH_SIZE, 0.0f, i * FLOOR_PIECE_HEIGHT_SIZE);
			m_Tree_Model->Draw();
			glPopMatrix();
		}

		glPopMatrix();
	}
};