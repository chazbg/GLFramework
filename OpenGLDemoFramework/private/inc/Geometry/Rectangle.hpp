#pragma once
#include "Core/Mesh.hpp"
#include "Math/Vector.hpp"
#include "Core/Texture.hpp"
#include <string>

class Rectangle : public Mesh
{
public:
	Rectangle();
	Rectangle(Vec2 topLeft, Vec2 bottomRight);
	~Rectangle();
	void Render();
	void attachTexture(const Texture& texture);
	virtual void  SetShaders(const std::string vertexShader, const std::string fragmentShader);
	void SetTime(GLuint time);
	GLuint GetTime();
	unsigned int GetTexId();
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