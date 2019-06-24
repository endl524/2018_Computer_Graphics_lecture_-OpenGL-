#pragma once
#include "Scene.h"

class InGame_Scene : Scene
{
private:
	Robot* m_Robot = nullptr;
	int m_Robot_HP = 3;

	Igloo* m_Igloo = nullptr;
	Floor* m_Floor;
	uniform_real_distribution<> m_Random_Position_X_Min;
	uniform_real_distribution<> m_Random_Position_X_Max;
	uniform_real_distribution<> m_Random_Position_Z_Min;
	uniform_real_distribution<> m_Random_Position_Z_Max;
	uniform_int_distribution<> m_Random_Min_or_Max;

	queue<Snow_Ball*> m_Snow_Ball_Pool;
	queue<Small_Enemy*> m_Small_Enemy_1_Pool;
	queue<Small_Enemy*> m_Small_Enemy_2_Pool;
	
	float m_Small_Enemy_Spawn_Time = 6.0f;
	float m_Curr_Small_Enemy_1_Spawn_Time = 0.0f;
	float m_Curr_Small_Enemy_2_Spawn_Time = 0.0f;


	UI* m_Fade_Image = nullptr;
	UI* m_Fog_Image = nullptr;
	UI* m_Gauge_Bar_Image = nullptr;
	UI* m_Gauge_Image = nullptr;
	UI* m_Heart_Image[PLAYER_MAX_HP]{nullptr, };
	float m_Gauge_Value = 0.0f;

	UI* m_Igloo_HP_Bar_Image = nullptr;
	UI* m_Igloo_HP_Gauge_Image = nullptr;
	float m_Igloo_HP_Gauge_Value = 0.0f;

	bool m_Fade_Prod_is_Over = false;
	bool m_Fade_In_Prod = true;
	bool m_is_Game_Started = false;
	bool m_is_Cursor_Free = false;
	float m_Mouse_Response = 0.1f;


public:

	virtual void Set_Random_Device()
	{
		mt19937_64 seed(m_Random_Device());
		m_Random_SEED = seed;

		uniform_int_distribution<> rand_min_or_max(0, 1);
		m_Random_Min_or_Max = rand_min_or_max;

		uniform_real_distribution<> rand_Pos_x_min((-FLOOR_WIDTH + 3) * FLOOR_PIECE_WIDTH_SIZE, -5 * FLOOR_PIECE_WIDTH_SIZE);
		m_Random_Position_X_Min = rand_Pos_x_min;

		uniform_real_distribution<> rand_Pos_x_max(5 * FLOOR_PIECE_WIDTH_SIZE, (FLOOR_WIDTH - 3) * FLOOR_PIECE_WIDTH_SIZE);
		m_Random_Position_X_Max = rand_Pos_x_max;

		uniform_real_distribution<> rand_Pos_z_min((-FLOOR_HEIGHT + 3) * FLOOR_PIECE_HEIGHT_SIZE, -5 * FLOOR_PIECE_HEIGHT_SIZE);
		m_Random_Position_Z_Min = rand_Pos_z_min;

		uniform_real_distribution<> rand_Pos_z_max(5 * FLOOR_PIECE_HEIGHT_SIZE, (FLOOR_HEIGHT - 3) * FLOOR_PIECE_HEIGHT_SIZE);
		m_Random_Position_Z_Max = rand_Pos_z_max;
	}

public:
	InGame_Scene() : Scene() { m_is_Cursor_Free = false; glutSetCursor(GLUT_CURSOR_NONE); glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, s_Scene_instance->Set_Production_Update_Timer, -332); }
	~InGame_Scene() { Destroy_Scene(); }


	virtual void Init_Object()
	{
		Object* temp_Object;
		int num = 0;

		num = Build_Object(FLOOR_OBJECT);
		Object_Activate(num); temp_Object = Get_Object(num); m_Floor = reinterpret_cast<Floor*>(temp_Object);
		m_Floor->Wheather_Initialize(WHEATHER_TYPE_SNOW, 200, false);
		m_Floor->Switching_Wheathering();

		num = Build_Object(DIRECTIONAL_LIGHT_OBJECT);

		for (int i = 0; i < 30; ++i) { Build_Object(ROCK_OBSTACLE); }
		for (int i = 0; i < 30; ++i) { Build_Object(TREE_OBSTACLE); }
		for (int i = 0; i < 60; ++i) { Build_Object(SNOW_OBJECT); }

		Build_Object(FENCE_OBJECT);

		num = Build_Object(IGLOO_OBJECT);
		Object_Activate(num);
		m_Igloo = reinterpret_cast<Igloo*>(Get_Object(num));

		num = Build_Object(ROBOT_OBJECT);
		Object_Activate(num);
		m_Robot = reinterpret_cast<Robot*>(Get_Object(num));
		m_Robot->Init_Prev_Pos();
		m_Robot_HP = m_Robot->Get_HP();
		if (m_Camera != nullptr)
		{
			m_Robot->Set_Camera(m_Camera);
			m_Camera->Set_is_FPS(false);
			glutSetCursor(GLUT_CURSOR_NONE);
		}

		for (int i = 0; i < 20; ++i)
		{
			num = Build_Object(SNOW_BALL_OBJECT);
			Object_Activate(num);
			m_Snow_Ball_Pool.push(reinterpret_cast<Snow_Ball*>(Get_Object(num)));
		}

		for (int i = 0; i < 6; ++i)
		{
			num = Build_Object(SMALL_ENEMY_1);
			Object_Activate(num);
			m_Small_Enemy_1_Pool.push(reinterpret_cast<Small_Enemy*>(Get_Object(num)));
		}

		Small_Enemy* temp_Small_Enemy;
		while (!m_Small_Enemy_1_Pool.empty())
		{
			temp_Small_Enemy = m_Small_Enemy_1_Pool.front();
			temp_Small_Enemy->Set_Target(reinterpret_cast<Object*>(m_Robot));
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_X(m_Random_Position_X_Min(m_Random_SEED));
			else reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_X(m_Random_Position_X_Max(m_Random_SEED));
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_Z(m_Random_Position_Z_Min(m_Random_SEED));
			else reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_Z(m_Random_Position_Z_Max(m_Random_SEED));
			temp_Small_Enemy->Small_Enemy_Initialize();
			temp_Small_Enemy->Set_is_Alive(true);
			reinterpret_cast<Object*>(temp_Small_Enemy)->Set_is_Move(true);
			m_Small_Enemy_1_Pool.pop();
		}

		for (int i = 0; i < 10; ++i)
		{
			num = Build_Object(SMALL_ENEMY_2);
			Object_Activate(num);
			m_Small_Enemy_2_Pool.push(reinterpret_cast<Small_Enemy*>(Get_Object(num)));
		}

		while (!m_Small_Enemy_2_Pool.empty())
		{
			temp_Small_Enemy = m_Small_Enemy_2_Pool.front();
			temp_Small_Enemy->Set_Target(reinterpret_cast<Object*>(m_Igloo));
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_X(m_Random_Position_X_Min(m_Random_SEED));
			else reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_X(m_Random_Position_X_Max(m_Random_SEED));
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_Z(m_Random_Position_Z_Min(m_Random_SEED));
			else reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_Z(m_Random_Position_Z_Max(m_Random_SEED));
			temp_Small_Enemy->Small_Enemy_Initialize();
			temp_Small_Enemy->Set_is_Alive(true);
			reinterpret_cast<Object*>(temp_Small_Enemy)->Set_is_Move(true);
			m_Small_Enemy_2_Pool.pop();
		}
	}

	virtual int Build_Object(const int& type)
	{
		Object* temp_Object = nullptr;
		Floor* temp_Floor = nullptr;
		Light_Object* temp_Light = nullptr;
		Rock* temp_Rock = nullptr;
		Tree* temp_Tree = nullptr;
		Fence* temp_Fence = nullptr;
		Snows* temp_Snows = nullptr;
		Igloo* temp_Igloo = nullptr;
		Robot* temp_Robot = nullptr;
		Snow_Ball* temp_Snow_Ball = nullptr;
		Small_Enemy* temp_Small_Enemy = nullptr;

		switch (type)
		{
		case FLOOR_OBJECT:
			temp_Floor = new Floor(FLOOR_WIDTH, FLOOR_HEIGHT, FLOOR_PIECE_WIDTH_SIZE, FLOOR_PIECE_HEIGHT_SIZE);
			temp_Object = reinterpret_cast<Object*>(temp_Floor);
			temp_Object->Set_Object_Type(FLOOR_OBJECT);
			temp_Object->Set_Translate(0.0f, -OBJECT_SIZE, 0.0f);
			temp_Object->Set_is_Static(true);

			break;

		case DIRECTIONAL_LIGHT_OBJECT:
			temp_Light = new Light_Object(0);
			if (!temp_Light->Get_is_invalid_Light_Object())
			{
				temp_Object = reinterpret_cast<Object*>(temp_Light);
				temp_Object->Set_Object_Type(DIRECTIONAL_LIGHT_OBJECT);
				temp_Object->Set_Translate(0.0f, OBJECT_SIZE * 15.0f, 0.0f);
				temp_Object->Pitch(90.0f);
				temp_Light->Set_Material_Ambient_Color(0.15f, 0.1f, 0.1f, 1.0f);
				temp_Light->Set_Material_Diffuse_Color(0.9f, 0.8f, 0.9f, 1.0f);
				temp_Light->Set_Material_Specular_Color(0.6f, 0.6f, 0.8f, 1.0f);
				temp_Light->Set_Light_Position(0.0f, 0.0f, 0.0f, 0.5f);
				temp_Object->Set_is_Static(true);
				temp_Light->Light_On();
			}
			break;

		case ROCK_OBSTACLE:
			temp_Rock = new Rock();
			temp_Object = reinterpret_cast<Object*>(temp_Rock);
			temp_Object->Set_is_Static(false);
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				temp_Object->Set_Position_X(m_Random_Position_X_Min(m_Random_SEED));
			else temp_Object->Set_Position_X(m_Random_Position_X_Max(m_Random_SEED));
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				temp_Object->Set_Position_Z(m_Random_Position_Z_Min(m_Random_SEED));
			else temp_Object->Set_Position_Z(m_Random_Position_Z_Max(m_Random_SEED));
			temp_Object->Make_OBB();
			temp_Rock->Rock_OBB_Size_Rebuild();
			break;

		case TREE_OBSTACLE:
			temp_Tree = new Tree();
			temp_Object = reinterpret_cast<Object*>(temp_Tree);
			temp_Object->Set_is_Static(false);
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				temp_Object->Set_Position_X(m_Random_Position_X_Min(m_Random_SEED));
			else temp_Object->Set_Position_X(m_Random_Position_X_Max(m_Random_SEED));
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				temp_Object->Set_Position_Z(m_Random_Position_Z_Min(m_Random_SEED));
			else temp_Object->Set_Position_Z(m_Random_Position_Z_Max(m_Random_SEED));
			temp_Object->Make_OBB();
			break;

		case FENCE_OBJECT:
			temp_Fence = new Fence();
			temp_Object = reinterpret_cast<Object*>(temp_Fence);
			temp_Object->Set_is_Static(false);
			temp_Object->Set_Object_Type(FENCE_OBJECT);
			break;

		case SNOW_OBJECT:
			temp_Snows = new Snows();
			temp_Object = reinterpret_cast<Object*>(temp_Snows);
			temp_Object->Set_is_Static(true);
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				temp_Object->Set_Position_X(m_Random_Position_X_Min(m_Random_SEED));
			else temp_Object->Set_Position_X(m_Random_Position_X_Max(m_Random_SEED));
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				temp_Object->Set_Position_Z(m_Random_Position_Z_Min(m_Random_SEED));
			else temp_Object->Set_Position_Z(m_Random_Position_Z_Max(m_Random_SEED));
			break;

		case IGLOO_OBJECT:
			temp_Igloo = new Igloo();
			temp_Object = reinterpret_cast<Object*>(temp_Igloo);
			temp_Object->Set_is_Static(false);
			temp_Object->Set_Position_X(0.0f); temp_Object->Set_Position_Z(0.0f);
			temp_Object->Make_OBB();
			temp_Object->Set_Object_Type(IGLOO_OBJECT);
			temp_Igloo->Igloo_OBB_Size_Rebuild();
			break;

		case ROBOT_OBJECT:
			temp_Robot = new Robot();
			temp_Object = reinterpret_cast<Object*>(temp_Robot);
			temp_Object->Set_is_Static(false);
			if (m_Igloo != nullptr) temp_Object->Set_Position_Z(reinterpret_cast<Object*>(m_Igloo)->Get_Position_Z() + OBJECT_SIZE * 8.0f);
			temp_Robot->Robot_Initialize();
			temp_Robot->Init_Head();
			break;

		case SNOW_BALL_OBJECT:
			temp_Snow_Ball = new Snow_Ball();
			temp_Snow_Ball->Set_Pool_pointer(&m_Snow_Ball_Pool);
			temp_Object = reinterpret_cast<Object*>(temp_Snow_Ball);
			temp_Object->Get_OBB()->OBB_Init(temp_Object->Get_Position(), temp_Object->Get_Scale_X() * 0.25f, temp_Object->Get_Scale_Y() * 0.25f, temp_Object->Get_Scale_Z() * 0.25f);
			break;

		case SMALL_ENEMY_1: // 유닛공격
			temp_Small_Enemy = new Small_Enemy();
			temp_Small_Enemy->Set_Pool_pointer(&m_Small_Enemy_1_Pool);
			temp_Object = reinterpret_cast<Object*>(temp_Small_Enemy);
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				temp_Object->Set_Position_X(m_Random_Position_X_Min(m_Random_SEED));
			else temp_Object->Set_Position_X(m_Random_Position_X_Max(m_Random_SEED));
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				temp_Object->Set_Position_Z(m_Random_Position_Z_Min(m_Random_SEED));
			else temp_Object->Set_Position_Z(m_Random_Position_Z_Max(m_Random_SEED));
			temp_Object->Set_Object_Type(SMALL_ENEMY_1);
			temp_Small_Enemy->Small_Enemy_Initialize();
			break;

		case SMALL_ENEMY_2: // 공성
			temp_Small_Enemy = new Small_Enemy();
			temp_Small_Enemy->Set_Pool_pointer(&m_Small_Enemy_2_Pool);
			temp_Object = reinterpret_cast<Object*>(temp_Small_Enemy);
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				temp_Object->Set_Position_X(m_Random_Position_X_Min(m_Random_SEED));
			else temp_Object->Set_Position_X(m_Random_Position_X_Max(m_Random_SEED));
			if (m_Random_Min_or_Max(m_Random_SEED) == 0)
				temp_Object->Set_Position_Z(m_Random_Position_Z_Min(m_Random_SEED));
			else temp_Object->Set_Position_Z(m_Random_Position_Z_Max(m_Random_SEED));
			temp_Object->Set_Object_Type(SMALL_ENEMY_2);
			temp_Small_Enemy->Small_Enemy_Initialize();
			break;
		}

		if (temp_Object != nullptr)
		{
			Insert_Object(temp_Object);
			return temp_Object->Get_Index();
		}
		else return -1;
	}

	virtual void Init_UI()
	{
		int num = 0;

		num = Build_UI(UI_FADE_IMAGE);
		m_Fade_Image = Get_UI(num);
		m_Fade_Prod_is_Over = false;
		m_Fade_In_Prod = true;

		num = Build_UI(UI_FOG_IMAGE);
		m_Fog_Image = Get_UI(num);
		m_Fog_Image->Set_Fade(0.0f);

		num = Build_UI(UI_GAUGE_BAR_IMAGE);
		m_Gauge_Bar_Image = Get_UI(num);

		num = Build_UI(UI_GAUGE_IMAGE);
		m_Gauge_Image = Get_UI(num);

		for (int i = 1; i <= m_Robot_HP; ++i)
		{
			num = Build_UI(UI_HP_IMAGE);
			m_Heart_Image[i - 1] = Get_UI(num);
			m_Heart_Image[i - 1]->Set_Position(0, 70.0f * i - m_Curr_Window_Size_X * 0.5f);
			m_Heart_Image[i - 1]->Color_less_Dark();
		}

		num = Build_UI(UI_IGLOO_HP_BAR_IMAGE);
		m_Igloo_HP_Bar_Image = Get_UI(num);

		num = Build_UI(UI_IGLOO_HP_GAUGE_IMAGE);
		m_Igloo_HP_Gauge_Image = Get_UI(num);
	}

	virtual int Build_UI(const int& type)
	{
		UI* temp_UI = nullptr;

		switch (type)
		{
		case UI_FOG_IMAGE:
			temp_UI = new UI();
			temp_UI->Set_Texture("Resources/UI/Fog_Image.bmp", 512, 512);
			temp_UI->Set_Position(0.0f, 0.0f, -150.0f);
			temp_UI->Set_Scale(2.0f, 1.5f, 1.0f);
			temp_UI->Set_Color(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case UI_FADE_IMAGE:
			temp_UI = new UI();
			temp_UI->Set_Texture("Resources/UI/Fade_Image.bmp", 512, 512);
			temp_UI->Set_Position(0.0f, 0.0f, -200.0f);
			temp_UI->Set_Scale(2.0f, 1.5f, 1.0f);
			temp_UI->Set_is_Fade_UI(true);
			temp_UI->Set_Color(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case UI_GAUGE_BAR_IMAGE:
			temp_UI = new UI();
			temp_UI->Set_Texture("Resources/UI/Gauge_Bar.bmp", 256, 32);
			temp_UI->Set_Position(0.0f, 150.0f - m_Curr_Window_Size_Y * 0.5f, -141.0f);
			temp_UI->Set_Scale(2.0f, 1.5f, 1.0f);
			temp_UI->Set_Color(1.0f, 1.0f, 1.0f, 1.0f);
			temp_UI->Set_is_Activate(false);
			break;

		case UI_GAUGE_IMAGE:
			temp_UI = new UI();
			temp_UI->Set_Texture("Resources/UI/Gauge.bmp", 256, 32);
			temp_UI->Set_Position(0.0f, 150.0f - m_Curr_Window_Size_Y * 0.5f, -140.0f);
			temp_UI->Set_Scale(1.0f, 1.0f, 1.0f);
			temp_UI->Set_Color(1.0f, 1.0f, 1.0f, 1.0f);
			temp_UI->Set_is_Activate(false);
			break;

		case UI_HP_IMAGE:
			temp_UI = new UI();
			temp_UI->Set_Texture("Resources/UI/Heart.bmp", 512, 512);
			temp_UI->Set_Position(0.0f, 50.0f - m_Curr_Window_Size_Y * 0.5f, -142.0f);
			temp_UI->Set_Scale(0.1f, 0.1f, 1.0f);
			temp_UI->Set_Color(1.0f, 1.0f, 1.0f, 1.0f);
			break;

		case UI_IGLOO_HP_BAR_IMAGE:
			temp_UI = new UI();
			temp_UI->Set_Texture("Resources/UI/Gauge_Bar.bmp", 256, 32);
			temp_UI->Set_Position(0.0f, m_Curr_Window_Size_Y * 0.5f - 50.0f, -144.0f);
			temp_UI->Set_Scale(2.0f, 1.5f, 1.0f);
			temp_UI->Set_Color(0.5f, 0.5f, 1.0f, 1.0f);
			break;

		case UI_IGLOO_HP_GAUGE_IMAGE:
			temp_UI = new UI();
			temp_UI->Set_Texture("Resources/UI/Gauge.bmp", 256, 32);
			temp_UI->Set_Position(0.0f, m_Curr_Window_Size_Y * 0.5f - 50.0f, -143.0f);
			temp_UI->Set_Scale(1.0f, 1.0f, 1.0f);
			temp_UI->Set_Color(1.0f, 0.5f, 0.5f, 1.0f);
			break;
		}
		if (temp_UI != nullptr)
		{
			Insert_UI(temp_UI);
			return temp_UI->Get_Index();
		}
		else return -1;
	}

	virtual void Render_Background() { glClearColor(0.9f, 0.8f, 0.9f, 1.0f); }

	virtual void Key_Manager(unsigned char key, const int& x, const int& y)
	{
		if (m_is_Game_Started)
		{
			switch (key)
			{
			// =================================================
			// Player Controller
			case 'w': case 'W': if (!m_Robot->Get_is_Reloading()) reinterpret_cast<Object*>(m_Robot)->Forward_Move_On(); break;
			case 's': case 'S': if (!m_Robot->Get_is_Reloading()) reinterpret_cast<Object*>(m_Robot)->Backward_Move_On(); break;
			case 'a': case 'A': if (!m_Robot->Get_is_Reloading()) reinterpret_cast<Object*>(m_Robot)->Left_Move_On(); break;
			case 'd': case 'D': if (!m_Robot->Get_is_Reloading()) reinterpret_cast<Object*>(m_Robot)->Right_Move_On(); break;

			case 'r': case 'R':
				if (m_Robot != nullptr)
				{
					if (!m_Robot->Get_is_Reloading()) Gauge_Init_and_Activate();
					m_Robot->Reload();
				}
				break;

			case 'v': case 'V':
				if (m_Camera != nullptr)
				{
					if (m_Camera->Get_is_FPS())
					{
						m_Camera->Set_is_FPS(false);
						glutSetCursor(GLUT_CURSOR_NONE);
						if (!m_is_Cursor_Free) glutWarpPointer(m_Curr_Window_Size_X * 0.5f, m_Curr_Window_Size_Y * 0.5f);
					}
					else
					{
						m_Camera->Set_is_FPS(true);
						glutSetCursor(GLUT_CURSOR_CROSSHAIR);
						if (!m_is_Cursor_Free) glutWarpPointer(m_Curr_Window_Size_X * 0.5f, m_Curr_Window_Size_Y * 0.5f);
					}
					if (m_Robot != nullptr) m_Robot->Pitch_Correction();
				}
				break;

			// =================================================
				// 시스템 명령어
			case 'p': m_is_Cursor_Free = true; glutSetCursor(GLUT_CURSOR_INHERIT); break;

			case '[': if (m_Mouse_Response - 0.01f > 0.05f) m_Mouse_Response -= 0.01f; else m_Mouse_Response = 0.05f; break;
			case ']': if (m_Mouse_Response + 0.01f < 2.0f) m_Mouse_Response += 0.01f; else m_Mouse_Response = 2.0f; break;

			case 27: glutLeaveMainLoop(); break;
			}
		}
	}

	virtual void Key_Up_Manager(unsigned char key, const int& x, const int& y) 
	{
		if (m_is_Game_Started)
		{
			switch (key)
			{
				// =================================================
				// Player Controller
			case 'w': case 'W': reinterpret_cast<Object*>(m_Robot)->Forward_Move_Off(); m_Robot->Dash_Mode_Off(); break;
			case 's': case 'S': reinterpret_cast<Object*>(m_Robot)->Backward_Move_Off(); break;
			case 'a': case 'A': reinterpret_cast<Object*>(m_Robot)->Left_Move_Off(); break;
			case 'd': case 'D': reinterpret_cast<Object*>(m_Robot)->Right_Move_Off(); break;
			}
		}
	}

	virtual void Special_Key_Manager(const int& key, const int& x, const int& y) 
	{
		if (m_is_Game_Started)
		{
			switch (key)
			{
			case GLUT_KEY_SHIFT_L: if (m_Robot != nullptr) m_Robot->Dash_Mode_Activate(); break;
			}
		}
	}

	virtual void Special_Key_Up_Manager(const int& key, const int& x, const int& y)
	{
		if (m_is_Game_Started)
		{
			switch (key)
			{
			case GLUT_KEY_SHIFT_L: if (m_Robot != nullptr) m_Robot->Dash_Mode_Off(); break;
			}
		}
	}

	virtual void Mouse_Manager(const int& button, const int& state, const int& x, const int& y)
	{
		if (m_is_Game_Started)
		{
			switch (button)
			{
			case GLUT_LEFT_BUTTON:
				if (state == GLUT_DOWN && !m_is_Left_Clicked)
				{
					if (m_Robot != nullptr)
					{
						if (m_Robot->Get_Have_Snow_Ball() && !m_Snow_Ball_Pool.empty())
						{
							Snow_Ball* temp_Snow_Ball = reinterpret_cast<Snow_Ball*>(m_Robot->Get_Snow_Ball());
							temp_Snow_Ball->Set_is_Shot(true);
							m_Robot->Shoot_Process();
						}
					}
					if (m_Camera != nullptr)
					{
						if (m_Camera->Get_is_FPS()) glutSetCursor(GLUT_CURSOR_CROSSHAIR);
						else glutSetCursor(GLUT_CURSOR_NONE);
					}
					m_is_Cursor_Free = false;
					if (!m_is_Cursor_Free) glutWarpPointer(m_Curr_Window_Size_X * 0.5f, m_Curr_Window_Size_Y * 0.5f);

					m_is_Left_Clicked = true;
				}
				else if (state == GLUT_UP)
				{
					m_is_Left_Clicked = false;
				}
				break;

			case GLUT_RIGHT_BUTTON:
				if (state == GLUT_DOWN && !m_is_Right_Clicked)
				{
					if (m_Camera != nullptr)
					{
						if (m_Camera->Get_is_FPS()) glutSetCursor(GLUT_CURSOR_CROSSHAIR);
						else glutSetCursor(GLUT_CURSOR_NONE);
					}
					m_is_Cursor_Free = false;
					if (!m_is_Cursor_Free) glutWarpPointer(m_Curr_Window_Size_X * 0.5f, m_Curr_Window_Size_Y * 0.5f);
					m_is_Right_Clicked = true;
				}
				else if (state == GLUT_UP)
				{
					m_is_Right_Clicked = false;
				}
				break;
			}
		}
	}

	virtual void Mouse_Clicked_Move_Manager(const int& x, const int& y)
	{
		if (m_is_Game_Started)
		{
			if (!m_is_Cursor_Free)
			{
				if (m_Robot != nullptr)
				{
					m_Robot->Pitch(-(y - m_Curr_Window_Size_Y * 0.5f) * m_Mouse_Response);
					m_Robot->Yaw((x - m_Curr_Window_Size_X * 0.5f) * m_Mouse_Response);
				}
				glutWarpPointer(m_Curr_Window_Size_X * 0.5f, m_Curr_Window_Size_Y * 0.5f);
			}
		}
	}

	virtual void Mouse_Non_Clicked_Move_Manager(const int& x, const int& y)
	{
		if (m_is_Game_Started)
		{
			if (!m_is_Cursor_Free)
			{
				if (m_Robot != nullptr)
				{
					m_Robot->Pitch(-(y - m_Curr_Window_Size_Y * 0.5f) * m_Mouse_Response);
					m_Robot->Yaw((x - m_Curr_Window_Size_X * 0.5f) * m_Mouse_Response);
				}
				glutWarpPointer(m_Curr_Window_Size_X * 0.5f, m_Curr_Window_Size_Y * 0.5f);
			}
		}
	}

	virtual bool Production_Update()
	{
		if (!m_Fade_Prod_is_Over)
		{
			if (m_Fade_In_Prod)
			{
				if (m_Fade_Image != nullptr)
				{
					if (m_Fade_Image->Get_Alpha() > 0.0f)
					{
						m_Fade_Image->Plus_Fade(-0.02f);
						if (m_Fog_Image != nullptr) if (m_Fog_Image->Get_Alpha() < 1.0f) m_Fog_Image->Plus_Fade(0.02f, 0.3f);
					}
					else
					{
						m_Fade_Image->Set_Fade(0.0f);
						m_Fade_In_Prod = false;
						m_Fade_Prod_is_Over = true;
						m_Fade_Image->Set_is_Activate(false);
						if (m_Fog_Image != nullptr)
						{
							m_Fog_Image->Set_Color(1.0f, 1.0f, 1.0f, 1.0f);
							m_Fog_Image->Color_Dark();
						}
						m_is_Game_Started = true;
					}
				}
			}
		}

		if (m_Robot != nullptr)
		{
			if (m_Robot->Get_is_Reloading())
			{
				if (m_Gauge_Bar_Image != nullptr && m_Gauge_Image != nullptr)
				{
					m_Gauge_Value += 0.02f;
					m_Gauge_Image->Set_Scale(0, m_Gauge_Value);
					m_Gauge_Image->Set_Position(0, m_Gauge_Image->Get_Texture()->Get_Width() * 0.5f * m_Gauge_Image->Get_Scale_X() - m_Gauge_Image->Get_Texture()->Get_Width() * 0.5f * 1.95f);

					if (m_Gauge_Value >= 1.95f)
					{
						m_Robot->Make_Snow_Ball();
						m_Gauge_Bar_Image->Set_is_Activate(false);
						m_Gauge_Image->Set_is_Activate(false);

						Snow_Ball* temp_Snow_Ball = m_Snow_Ball_Pool.front();
						temp_Snow_Ball->Snow_Ball_Load(reinterpret_cast<Object*>(m_Robot));
						m_Robot->Set_Snow_Ball(reinterpret_cast<Object*>(temp_Snow_Ball));
						m_Snow_Ball_Pool.pop();
					}
				}
			}

			if (m_Robot->Get_HP() < m_Robot_HP && m_Robot_HP > 0)
			{
				for (int i = m_Robot_HP; i > m_Robot->Get_HP(); --i)
					m_Heart_Image[i - 1]->Set_Texture("Resources/UI/Broken_Heart.bmp", 512, 512);

				m_Robot_HP = m_Robot->Get_HP();
			}
			
			if (m_Robot->Get_HP() <= 0) Change_Scene(0);
		}

		if (!m_Small_Enemy_1_Pool.empty() && m_Robot != nullptr)
		{
			if (m_Curr_Small_Enemy_1_Spawn_Time >= m_Small_Enemy_Spawn_Time)
			{
				Small_Enemy* temp_Small_Enemy;
				while (!m_Small_Enemy_1_Pool.empty())
				{
					temp_Small_Enemy = m_Small_Enemy_1_Pool.front();
					temp_Small_Enemy->Set_Target(reinterpret_cast<Object*>(m_Robot));
					if (m_Random_Min_or_Max(m_Random_SEED) == 0)
						reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_X(m_Random_Position_X_Min(m_Random_SEED));
					else reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_X(m_Random_Position_X_Max(m_Random_SEED));
					if (m_Random_Min_or_Max(m_Random_SEED) == 0)
						reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_Z(m_Random_Position_Z_Min(m_Random_SEED));
					else reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_Z(m_Random_Position_Z_Max(m_Random_SEED));
					temp_Small_Enemy->Small_Enemy_Initialize();
					temp_Small_Enemy->Set_is_Alive(true);
					reinterpret_cast<Object*>(temp_Small_Enemy)->Set_is_Move(true);
					m_Small_Enemy_1_Pool.pop();
				}
				m_Curr_Small_Enemy_1_Spawn_Time = 0.0f;
			}
			else m_Curr_Small_Enemy_1_Spawn_Time += 0.01f;
		}

		if (!m_Small_Enemy_2_Pool.empty() && m_Igloo != nullptr)
		{
			if (m_Curr_Small_Enemy_2_Spawn_Time >= m_Small_Enemy_Spawn_Time)
			{
				Small_Enemy* temp_Small_Enemy;
				while (!m_Small_Enemy_2_Pool.empty())
				{
					temp_Small_Enemy = m_Small_Enemy_2_Pool.front();
					temp_Small_Enemy->Set_Target(reinterpret_cast<Object*>(m_Igloo));
					if (m_Random_Min_or_Max(m_Random_SEED) == 0)
						reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_X(m_Random_Position_X_Min(m_Random_SEED));
					else reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_X(m_Random_Position_X_Max(m_Random_SEED));
					if (m_Random_Min_or_Max(m_Random_SEED) == 0)
						reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_Z(m_Random_Position_Z_Min(m_Random_SEED));
					else reinterpret_cast<Object*>(temp_Small_Enemy)->Set_Position_Z(m_Random_Position_Z_Max(m_Random_SEED));
					temp_Small_Enemy->Small_Enemy_Initialize();
					temp_Small_Enemy->Set_is_Alive(true);
					reinterpret_cast<Object*>(temp_Small_Enemy)->Set_is_Move(true);
					m_Small_Enemy_2_Pool.pop();
				}
				m_Curr_Small_Enemy_2_Spawn_Time = 0.0f;
			}
			else m_Curr_Small_Enemy_2_Spawn_Time += 0.01f;
		}

		if (m_Igloo != nullptr)
		{
			if (m_Igloo_HP_Bar_Image != nullptr && m_Igloo_HP_Gauge_Image != nullptr)
			{
				m_Igloo_HP_Gauge_Value = (m_Igloo->Get_HP() / m_Igloo->Get_Max_HP()) * 1.95f;
				m_Igloo_HP_Gauge_Image->Set_Scale(0, m_Igloo_HP_Gauge_Value);
				m_Igloo_HP_Gauge_Image->Set_Position(0, m_Igloo_HP_Gauge_Image->Get_Texture()->Get_Width() * 0.5f * m_Igloo_HP_Gauge_Image->Get_Scale_X() - m_Igloo_HP_Gauge_Image->Get_Texture()->Get_Width() * 0.5f * 1.95f);

			}

			if (m_Igloo->Get_HP() <= 0) Change_Scene(0);
		}

		return true;
	}


	void Gauge_Init_and_Activate()
	{
		if (m_Gauge_Bar_Image != nullptr && m_Gauge_Image != nullptr)
		{
			m_Gauge_Bar_Image->Set_is_Activate(true);
			m_Gauge_Image->Set_is_Activate(true);
			m_Gauge_Value = 0.01f;
			m_Gauge_Image->Set_Scale(0, m_Gauge_Value);
			m_Gauge_Image->Set_Position(0, m_Gauge_Image->Get_Texture()->Get_Width() * 0.5f * m_Gauge_Image->Get_Scale_X() - m_Gauge_Image->Get_Texture()->Get_Width() * 0.5f * 1.95f);
		}
	}


};