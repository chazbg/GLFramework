#include "GLUTWrapper.hpp"
#include <GL/freeglut.h>
#include <cstring>

GLUTWrapper::GLUTWrapper()
{
}

GLUTWrapper::~GLUTWrapper()
{
}

void GLUTWrapper::InitWindow(void (*renderScene)())
{
	int argc = 1;
	char* argv = _strdup("Application");

	glutInit(&argc, &argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Bar");
	glutDisplayFunc(renderScene);
}

void GLUTWrapper::UpdateFrame()
{
	glutSwapBuffers();
}
void GLUTWrapper::RequestNewFrame()
{
	glutPostRedisplay();
}

void GLUTWrapper::RenderLoop()
{
	glutMainLoop();
}