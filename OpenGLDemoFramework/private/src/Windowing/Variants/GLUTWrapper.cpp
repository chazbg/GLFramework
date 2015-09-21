#include "Windowing/Window.hpp"
#include <cstring>

#include <iostream>
#include <map>
#include <GL/freeglut.h>
using namespace std;

static map<int, Window*> windows;
bool Window::init = false;

void Window::render()
{
	int i = glutGetWindow();
	windows[i]->onRender();
}

void Window::mouseEvent(int button, int state, int x, int y)
{
	int i = glutGetWindow();
	windows[i]->onMouseEvent(button, state, x, y);
}

void Window::keyboardEvent(unsigned char c, int x, int y)
{
	int i = glutGetWindow();
	windows[i]->onKeyboardEvent(c, x, y);
}

void Window::mouseMove(int x, int y)
{
    int i = glutGetWindow();
    windows[i]->onMouseMove(x, y);
}

Window::Window(const WindowParameters& params, IApplication& app) : params(params), app(app)
{
	if (!init)
	{
		int argc = 1;
		char* argv = _strdup("Application");
		
		glutInit(&argc, &argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

		init = true;
	}

	glutInitWindowSize(params.width, params.height);
	glutInitWindowPosition(params.posX, params.posY);
	window = glutCreateWindow(params.name.c_str());
	windows[window] = this; 
	glutMouseFunc(Window::mouseEvent);
	glutKeyboardFunc(Window::keyboardEvent);
	glutDisplayFunc(Window::render);
    glutPassiveMotionFunc(Window::mouseMove);
}

Window::~Window()
{
	glutDestroyWindow(window);
}

void Window::startRenderLoop()
{
	app.onInit();
	glutMainLoop();
	app.onDestroy();	
}

void Window::onRender()
{
	//TODO: calculate delta
	app.onUpdate(0);
	app.onRender(0);
	glutSwapBuffers();
	glutPostRedisplay();
}

void Window::onMouseEvent(int button, int state, int x, int y)
{
	app.onMouseEvent(button, state, x, y);
}

void Window::onKeyboardEvent(unsigned char c, int x, int y)
{
	app.onKeyboardEvent(c, x, y);
}

void Window::onMouseMove(int x, int y)
{
    app.onMouseMove(x, y);
}