#pragma once
#include "Scene.h"

class Main_Scene : Scene
{
private:
	UI* m_Title_Logo_Image = nullptr;
	UI* m_Fog_Image = nullptr;
	UI* m_Fade_Image = nullptr;
	UI* m_Game_Start_Button = nullptr;
	UI* m_Title_Exit_Button = nullptr;
	uniform_real_distribution<> m_Random_Position_X;
	uniform_real_distribution<> m_Random_Position_Z;

	bool m_Fade_In_Prod = false;
	bool m_Fade_Out_Prod = false;
	bool m_Fade_Prod_is_Over = false;
	bool m_is_Clicked_Game_Start = false;
	
	bool m_Title_Logo_Prod = false;
	bool m_Title_Logo_Shake_Prod = false;
	int m_Title_Shake_Sign = 1;
	int m_Title_Shake_Count = 0;

public:
	virtual void Set_Random_Device()
	{
		mt19937_64 seed(m_Random_Device());
		m_Random_SEED = seed;

		uniform_real_distribution<> rand_Pos_x((-FLOOR_WIDTH + 3) * FLOOR_PIECE_WIDTH_SIZE, (FLOOR_WIDTH - 3) * FLOOR_PIECE_WIDTH_SIZE);
		m_Random_Position_X = rand_Pos_x;

		uniform_real_distribution<> rand_Pos_z((-FLOOR_HEIGHT + 3) * FLOOR_PIECE_HEIGHT_SIZE, (FLOOR_HEIGHT - 3) * FLOOR_PIECE_HEIGHT_SIZE);
		m_Random_Position_Z = rand_Pos_z;
	}

public:
	Main_Scene() : Scene() { glutSetCursor(GLUT_CURSOR_INHERIT); }
	~Main_Scene() { Destroy_Scene(); }

	virtual void Init_Camera()
	{
		if (m_Camera != nullptr)
		{
			delete m_Camera;
			m_Camera = nullptr;
		}
		m_Camera = new Camera();
		m_Camera->Free_View();
		m_Camera->Set_is_FPS(true);
		m_Camera->Set_Position(0.0f, 500.0f, 800.0f);
		m_Camera->Set_Look_At(0.0f, 0.0f, 0.0f);

		glutTimerFunc(OBJECT_TIMER_ELAPSED_TIME, s_Scene_instance->Set_Production_Update_Timer, -333);
	}

	virtual void Init_Object()
	{
		Object* temp_Object;
		int num = 0;

		num = Build_Object(FLOOR_OBJECT);
		Object_Activate(num); temp_Object = Get_Object(num);
		reinterpret_cast<Floor*>(temp_Object)->Wheather_Initialize(WHEATHER_TYPE_SNOW, 400, false);
		reinterpret_cast<Floor*>(temp_Object)->Switching_Wheathering();

		num = Build_Object(DIRECTIONAL_LIGHT_OBJECT);

		for (int i = 0; i < 100; ++i) { Build_Object(ROCK_OBSTACLE); }
		for (int i = 0; i < 100; ++i) { Build_Object(TREE_OBSTACLE); }
		for (int i = 0; i < 100; ++i) { Build_Object(SNOW_OBJECT); }

		num = Build_Object(IGLOO_OBJECT);
		Object_Activate(num);
	}

	virtual int Build_Object(const int& type)
	{
		Object* temp_Object = nullptr;
		Floor* temp_Floor = nullptr;
		Light_Object* temp_Light = nullptr;
		Rock* temp_Rock = nullptr;
		Tree* temp_Tree = nullptr;
		Igloo* temp_Igloo = nullptr;

		switch (type)
		{
		case FLOOR_OBJECT:
			temp_Floor = new Floor(FLOOR_WIDTH, FLOOR_HEIGHT, FLOOR_PIECE_WIDTH_SIZE, FLOOR_PIECE_HEIGHT_SIZE);
			reinterpret_cast<Object*>(temp_Floor)->Set_Object_Type(FLOOR_OBJECT);
			reinterpret_cast<Object*>(temp_Floor)->Set_Translate(0.0f, -OBJECT_SIZE, 0.0f);
			reinterpret_cast<Object*>(temp_Floor)->Set_is_Static(true);
			temp_Object = reinterpret_cast<Object*>(temp_Floor);
			break;

		case DIRECTIONAL_LIGHT_OBJECT:
			temp_Light = new Light_Object(0);
			if (!temp_Light->Get_is_invalid_Light_Object())
			{
				temp_Object = reinterpret_cast<Object*>(temp_Light);
				temp_Object->Set_Object_Type(DIRECTIONAL_LIGHT_OBJECT);
				temp_Object->Set_Translate(0.0f, OBJECT_SIZE * 15.0f, 0.0f);
				temp_Object->Pitch(90.0f);
				temp_Light->Set_Material_Ambient_Color(0.3f, 0.1f, 0.3f, 1.0f);
				temp_Light->Set_Material_Diffuse_Color(1.0f, 1.0f, 1.0f, 1.0f);
				temp_Light->Set_Material_Specular_Color(1.0f, 1.0f, 1.0f, 1.0f);
				temp_Light->Set_Light_Position(0.0f, 0.0f, 0.0f, 0.5f);
				temp_Object->Set_is_Static(true);
				temp_Light->Light_On();
			}
			break;

		case ROCK_OBSTACLE:
			temp_Rock = new Rock();
			temp_Object = reinterpret_cast<Object*>(temp_Rock);
			temp_Object->Set_is_Static(false);
			temp_Object->Set_Position_X(m_Random_Position_X(m_Random_SEED));
			temp_Object->Set_Position_Z(m_Random_Position_Z(m_Random_SEED));
			temp_Object->Make_OBB();
			temp_Rock->Rock_OBB_Size_Rebuild();
			break;

		case TREE_OBSTACLE:
			temp_Tree = new Tree();
			temp_Object = reinterpret_cast<Object*>(temp_Tree);
			temp_Object->Set_is_Static(false);
			temp_Object->Set_Position_X(m_Random_Position_X(m_Random_SEED));
			temp_Object->Set_Position_Z(m_Random_Position_Z(m_Random_SEED));
			temp_Object->Make_OBB();
			break;

		case IGLOO_OBJECT:
			temp_Igloo = new Igloo();
			temp_Object = reinterpret_cast<Object*>(temp_Igloo);
			temp_Object->Set_is_Static(true);
			temp_Object->Set_Position_X(0.0f); temp_Object->Set_Position_Z(0.0f);
			temp_Object->Make_OBB();
			temp_Igloo->Igloo_OBB_Size_Rebuild();
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

		num = Build_UI(UI_GAME_START_BUTTON);
		m_Game_Start_Button = Get_UI(num);
		m_Game_Start_Button->Color_Dark();

		num = Build_UI(UI_TITLE_EXIT_BUTTON);
		m_Title_Exit_Button = Get_UI(num);
		m_Title_Exit_Button->Color_Dark();

		num = Build_UI(UI_TITLE_LOGO);
		m_Title_Logo_Image = Get_UI(num);
		m_Title_Logo_Prod = true;

		num = Build_UI(UI_FADE_IMAGE);
		m_Fade_Image = Get_UI(num);
		m_Fade_Prod_is_Over = false;
		m_Fade_In_Prod = true;

		num = Build_UI(UI_FOG_IMAGE);
		m_Fog_Image = Get_UI(num);
		m_Fog_Image->Set_Fade(0.0f);		
	}

	virtual int Build_UI(const int& type) 
	{ 
		UI* temp_UI = nullptr;

		switch (type)
		{
		case UI_TITLE_LOGO:
			temp_UI = new UI();
			temp_UI->Set_Texture("Resources/UI/Title_Logo.bmp", 512, 256);
			temp_UI->Set_Color(1.0f, 1.0f, 1.0f, 1.0f);
			temp_UI->Set_Position(0.0f, m_Curr_Window_Size_Y + 100.0f, -150.0f);
			break;

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

		case UI_GAME_START_BUTTON:
			temp_UI = new UI();
			temp_UI->Set_Texture("Resources/UI/Game_Start_Button.bmp", 256, 128);
			temp_UI->Set_Position(m_Curr_Window_Size_X * 0.5f - 150.0f, 200.0f - m_Curr_Window_Size_Y * 0.5f, -210.0f);
			temp_UI->Set_Scale(0.6f, 0.6f, 1.0f);
			break;

		case UI_TITLE_EXIT_BUTTON:
			temp_UI = new UI();
			temp_UI->Set_Texture("Resources/UI/Game_Leave_Button.bmp", 256, 128);
			temp_UI->Set_Position(m_Curr_Window_Size_X * 0.5f - 150.0f, 100.0f - m_Curr_Window_Size_Y * 0.5f, -210.0f);
			temp_UI->Set_Scale(0.6f, 0.6f, 1.0f);
			break;
		}

		if (temp_UI != nullptr)
		{
			Insert_UI(temp_UI);
			return temp_UI->Get_Index();
		}
		else return -1;
	}

	virtual void Render_Background() { glClearColor(1.0f, 1.0f, 1.0f, 1.0f); }

	virtual void Key_Manager(unsigned char key, const int& x, const int& y)
	{
		switch (key)
		{
		// =================================================
			// 시스템 명령어

		case 27: glutLeaveMainLoop(); break;
		}
	}


	virtual void Mouse_Manager(const int& button, const int& state, const int& x, const int& y)
	{
		if (m_Fade_Prod_is_Over)
		{
			int mouse_pos_x = x - m_Curr_Window_Size_X * 0.5f;
			int mouse_pos_y = m_Curr_Window_Size_Y * 0.5f - y;

			switch (button)
			{
			case GLUT_LEFT_BUTTON:
				if (state == GLUT_DOWN && !m_is_Left_Clicked)
				{
					if (m_Game_Start_Button->Get_Position_X() - m_Game_Start_Button->Get_Width() * 0.5f <= mouse_pos_x &&
						m_Game_Start_Button->Get_Position_X() + m_Game_Start_Button->Get_Width() * 0.5f >= mouse_pos_x &&
						m_Game_Start_Button->Get_Position_Y() - m_Game_Start_Button->Get_Height() * 0.5f <= mouse_pos_y &&
						m_Game_Start_Button->Get_Position_Y() + m_Game_Start_Button->Get_Height() * 0.5f >= mouse_pos_y)
					{
						if (m_Fade_Image != nullptr)
						{
							m_Fade_Out_Prod = true;
							m_Fade_Prod_is_Over = false;
							m_Fade_Image->Set_is_Activate(true);
							if (m_Fog_Image != nullptr) m_Fog_Image->Set_Fade(0.0f);
						}
						if (m_Title_Logo_Image != nullptr) m_Title_Logo_Image->Set_Position(2, -210.0f);
						m_is_Clicked_Game_Start = true;
					}

					else if (m_Title_Exit_Button->Get_Position_X() - m_Title_Exit_Button->Get_Width() * 0.5f <= mouse_pos_x &&
						m_Title_Exit_Button->Get_Position_X() + m_Title_Exit_Button->Get_Width() * 0.5f >= mouse_pos_x &&
						m_Title_Exit_Button->Get_Position_Y() - m_Title_Exit_Button->Get_Height() * 0.5f <= mouse_pos_y &&
						m_Title_Exit_Button->Get_Position_Y() + m_Title_Exit_Button->Get_Height() * 0.5f >= mouse_pos_y) glutLeaveMainLoop();

					m_is_Left_Clicked = true;
				}
				else if (state == GLUT_UP)
				{
					m_is_Left_Clicked = false;
				}
				break;
			}
		}
	}

	virtual void Mouse_Non_Clicked_Move_Manager(const int& x, const int& y)
	{
		if (m_Fade_Prod_is_Over)
		{
			int mouse_pos_x = x - m_Curr_Window_Size_X * 0.5f;
			int mouse_pos_y = m_Curr_Window_Size_Y * 0.5f - y;

			m_Game_Start_Button->Color_Dark();
			m_Title_Exit_Button->Color_Dark();


			if (m_Game_Start_Button->Get_Position_X() - m_Game_Start_Button->Get_Width() * 0.5f <= mouse_pos_x &&
				m_Game_Start_Button->Get_Position_X() + m_Game_Start_Button->Get_Width() * 0.5f >= mouse_pos_x &&
				m_Game_Start_Button->Get_Position_Y() - m_Game_Start_Button->Get_Height() * 0.5f <= mouse_pos_y &&
				m_Game_Start_Button->Get_Position_Y() + m_Game_Start_Button->Get_Height() * 0.5f >= mouse_pos_y) m_Game_Start_Button->Color_Bright();

			else if (m_Title_Exit_Button->Get_Position_X() - m_Title_Exit_Button->Get_Width() * 0.5f <= mouse_pos_x &&
				m_Title_Exit_Button->Get_Position_X() + m_Title_Exit_Button->Get_Width() * 0.5f >= mouse_pos_x &&
				m_Title_Exit_Button->Get_Position_Y() - m_Title_Exit_Button->Get_Height() * 0.5f <= mouse_pos_y &&
				m_Title_Exit_Button->Get_Position_Y() + m_Title_Exit_Button->Get_Height() * 0.5f >= mouse_pos_y) m_Title_Exit_Button->Color_Bright();
		}
	}

	virtual bool Production_Update()
	{
		if (m_Camera != nullptr)
		{
			if (m_Fade_Out_Prod) m_Camera->Set_Rotation_Value(1, 5.0f);
			else m_Camera->Set_Rotation_Value(1, 1.0f);
		}
		if (!m_Fade_Prod_is_Over)
		{
			if (m_Fade_In_Prod)
			{
				if (m_Fade_Image != nullptr)
				{
					if (m_Fade_Image->Get_Alpha() > 0.0f)
					{
						m_Fade_Image->Plus_Fade(-0.01f);
						if (m_Fog_Image != nullptr) if (m_Fog_Image->Get_Alpha() < 1.0f) m_Fog_Image->Plus_Fade(0.01f, 0.3f);
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
					}
				}
			}
			else if (m_Fade_Out_Prod)
			{
				if (m_Fade_Image != nullptr)
				{
					if (m_Fade_Image->Get_Alpha() < 1.0f) m_Fade_Image->Plus_Fade(0.01f);
					else
					{
						m_Fade_Image->Set_Fade(1.0f);
						m_Fade_Out_Prod = false;
						m_Fade_Prod_is_Over = true;
						
						if (m_is_Clicked_Game_Start) Change_Scene(1);
					}
				}
			}
		}
		
		if (m_Title_Logo_Prod)
		{
			if (!m_Fade_In_Prod)
			{
				if (!m_Title_Logo_Shake_Prod)
				{
					if (m_Title_Logo_Image->Get_Position(1) > m_Curr_Window_Size_Y * 0.5f - 150.0f) m_Title_Logo_Image->Plus_Position(1, -30.0f);
					else
					{
						m_Title_Logo_Shake_Prod = true;
						m_Title_Logo_Image->Set_Position(1, m_Curr_Window_Size_Y * 0.5f - 150.0f);
					}
				}
				else
				{
					if (m_Title_Logo_Image->Get_Position(1) > m_Curr_Window_Size_Y * 0.5f - 140.0f
						|| m_Title_Logo_Image->Get_Position(1) < m_Curr_Window_Size_Y * 0.5f - 150.0f)
					{
						m_Title_Shake_Sign *= -1;
						++m_Title_Shake_Count;

						if (m_Title_Shake_Count > 4 && m_Title_Logo_Image->Get_Position(1) <= m_Curr_Window_Size_Y * 0.5f - 150.0f)
						{
							m_Title_Logo_Image->Set_Position(1, m_Curr_Window_Size_Y * 0.5f - 150.0f);
							m_Title_Logo_Prod = false;
							m_Title_Logo_Shake_Prod = false;
							m_Title_Shake_Sign = 1;
							m_Title_Shake_Count = 0;
							return true;
						}
					}

					m_Title_Logo_Image->Plus_Position(1, 7.0f * m_Title_Shake_Sign);
				}
			}
		}

		return true;
	}



};
