#include "Windowing/Variants/GLUTWrapper.hpp"
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
	SetKeyboardCallback();
	SetMouseCallback();
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

#include <iostream>
using namespace std;

void kbcb(unsigned char c, int x, int y)
{
	cout << c << " " << x << " " << y << endl;
}

void mscb(int button, int state, int x, int y)
{
	cout << button << " " << state << " " << x << " " << y << endl;
}

void GLUTWrapper::SetKeyboardCallback()
{
	glutKeyboardFunc(kbcb);
}

void GLUTWrapper::SetMouseCallback()
{
	glutMouseFunc(mscb);
}