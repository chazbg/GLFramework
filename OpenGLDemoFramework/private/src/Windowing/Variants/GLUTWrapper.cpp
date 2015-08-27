#include "Windowing/Window.hpp"
#include <cstring>

#include <iostream>
#include <map>
using namespace std;

static map<int, Window*> windows;
bool Window::init = false;

void Window::render()
{
	int i = glutGetWindow();
	windows[i]->onRender();
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
	glutDisplayFunc(Window::render);
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

//TODO: add callbacks
//void kbcb(unsigned char c, int x, int y)
//{
//	cout << c << " " << x << " " << y << endl;
//}
//
//void mscb(int button, int state, int x, int y)
//{
//	cout << button << " " << state << " " << x << " " << y << endl;
//}
//
//void GLUTWrapper::SetKeyboardCallback(void(*callback)(unsigned char, int, int))
//{
//	glutKeyboardFunc(callback);
//}
//
//void GLUTWrapper::SetMouseCallback(void(*callback)(int, int, int, int))
//{
//	glutMouseFunc(callback);
//}