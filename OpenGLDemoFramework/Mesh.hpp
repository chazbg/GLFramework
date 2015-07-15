#pragma once
#include <GL/glew.h>
#include "Matrix.hpp"
#include <map>

using namespace std;

class Mesh
{
public:
	Mesh();
	virtual void Render();
	~Mesh();
	void SetViewMatrix(const Matrix4& view);
	void SetShaders(const string vertexShaderPath, const string fragmentShaderPath);
	void SetWireframeMode(const bool showWireframe);
	void SetVertexBuffer(const float* vertexBuffer, const unsigned int length);
	void SetNormalsBuffer(const float* normalsBuffer, const unsigned int length);
	void BindUniform(string uniform);
	void SetUniformValue(string uniform, const unsigned int v);
	void SetUniformValue(string uniform, const Vec3& v);
	void SetUniformValue(string uniform, const Vec4& v);
	void SetUniformValue(string uniform, const Matrix4& v);
protected:
	void generateNormals();
	void generateWireframe();
	unsigned int vertexCount;
	float* vertexBuffer;
	float* normalsBuffer;
	float* wireframeVertexBuffer;
	GLuint vertexBufferID;
	GLuint normalsBufferID;
	GLuint wireframeVertexBufferID;
	GLuint programID;
	Matrix4 view;
	bool showWireframe;
	map<string, GLuint> uniforms;
};

