#include "GL_Main.h"

void main(int argc, char* argv[])
{
	GL_Main* p_GL_Main = new GL_Main(argc, argv);
	delete p_GL_Main;
}