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
	void SetTime(GLuint time);
	GLuint GetTime();
	void UseProgram();
private:
	Vec2 topLeft;
	Vec2 bottomRight;
	float* genVertices();
	float* genTexCoords();
	void initGL();
	float* texCoords;
	GLuint programID;
	GLuint timeID;
	GLuint texID;
	GLuint texUniform;
	GLuint vertexBufferID;
	GLuint texCoordsBufferID;
	unsigned int time;

};