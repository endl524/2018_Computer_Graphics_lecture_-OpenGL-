#include "GL_Main.h"


GL_Main::GL_Main(int argc, char* argv[])
{
	m_instance = this;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(INIT_WINDOW_POSITION_X, INIT_WINDOW_POSITION_Y);
	glutInitWindowSize(INIT_WINDOW_SIZE_X, INIT_WINDOW_SIZE_Y);
	glutCreateWindow("Example");
	glutDisplayFunc(Draw_Scene);
	glutReshapeFunc(Reshape);


	m_Curr_Object_Count = 0;
	Init_Objects();

	glutMainLoop();
}

GL_Main::~GL_Main()
{
}

void GL_Main::Draw_Scene(void)
{
	m_instance->Draw_Background();
	m_instance->Draw_Objects();
	glFlush();
}

void GL_Main::Reshape(const int w, const int h) { glViewport(0, 0, w, h); glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1.0f, 1.0f); }

void GL_Main::Draw_Background() { glClearColor(0.0f, 0.0f, 1.0f, 1.0f); glClear(GL_COLOR_BUFFER_BIT); }

void GL_Main::Draw_Objects() 
{
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(-400.0f, 0.0f);
	glVertex2f(400.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex2f(0.0f, 300.0f);
	glVertex2f(0.0f, -300.0f);
	glEnd();

	for (auto& object : m_instance->m_Object_List) object.second->Draw(); 
	
	glColor3f(0.0f, 1.0f, 1.0f);
	glRectf(-400.0f, 200.0f, -300.0f, 300.0f); // 좌상단
	glColor3f(0.0f, 1.0f, 0.0f);
	glRectf(300.0f, 200.0f, 400.0f, 300.0f); // 우상단
	glColor3f(1.0f, 0.0f, 1.0f);
	glRectf(-400.0f, -300.0f, -300.0f, -200.0f); // 좌하단
	glColor3f(1.0f, 1.0f, 0.0f);
	glRectf(300.0f, -300.0f, 400.0f, -200.0f); // 우하단
}

void GL_Main::Init_Objects()
{
	Object* temp_Object;
	vector<Vertex2D> v_vertexs;
	Vertex2D temp_Vertex;

	temp_Vertex.point_x = 0.0f;
	temp_Vertex.point_y = 50.0f;
	temp_Vertex.color.R = 1.0f;
	temp_Vertex.color.G = 0.0f;
	temp_Vertex.color.B = 0.0f;
	temp_Vertex.color.A = 1.0f;
	v_vertexs.push_back(temp_Vertex);
	temp_Vertex.point_x = -50.0f;
	temp_Vertex.point_y = -50.0f;
	temp_Vertex.color.R = 1.0f;
	temp_Vertex.color.G = 0.0f;
	temp_Vertex.color.B = 0.0f;
	temp_Vertex.color.A = 1.0f;
	v_vertexs.push_back(temp_Vertex);
	temp_Vertex.point_x = 50.0f;
	temp_Vertex.point_y = -50.0f;
	temp_Vertex.color.R = 1.0f;
	temp_Vertex.color.G = 0.0f;
	temp_Vertex.color.B = 0.0f;
	temp_Vertex.color.A = 1.0f;
	v_vertexs.push_back(temp_Vertex);

	

	temp_Object = Make_Object(GL_TRIANGLES, v_vertexs);
	Insert_Object(temp_Object);
}

Object* GL_Main::Make_Object(const GLenum type, vector<Vertex2D>& v_vertexs)
{
	Object* temp_Object = new Object(type, v_vertexs, m_Curr_Object_Count);
	++m_Curr_Object_Count;
	return temp_Object;
}