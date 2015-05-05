#pragma once
#include "Mesh.hpp"
#include "Vector.hpp"
#include <string>

class Rectangle : public Mesh
{
public:
	Rectangle();
	Rectangle(Vec2 topLeft, Vec2 bottomRight);
	~Rectangle();
	void Render();
	void attachTexture(GLuint width, GLuint height, void* data);
	void Rectangle::attachShaders(const std::string vertexShader, const std::string fragmentShader);
private:
	void initGL();
	void useProgram();
	GLfloat vertexBuffer[16]; //Vertices + texture coordinates
	GLuint programID;
	GLuint timeUniform;
	GLuint texID;
	GLuint texUniform;
	GLuint vertexBufferID;
	unsigned int time;

};