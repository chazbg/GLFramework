#pragma once
#include "Mesh.hpp"
#include "Vector.hpp"
#include "Texture.hpp"
#include <string>

class Rectangle : public Mesh
{
public:
	Rectangle();
	Rectangle(Vec2 topLeft, Vec2 bottomRight);
	~Rectangle();
	void Render();
	void attachTexture(const Texture& texture);
	void Rectangle::attachShaders(const std::string vertexShader, const std::string fragmentShader);
	void SetTime(GLuint time);
	GLuint GetTime();
private:
	Vec2 topLeft;
	Vec2 bottomRight;
	float* genVertices();
	float* genTexCoords();
	void init();
	float* texCoords;
	GLuint texID;
	unsigned int time;

};