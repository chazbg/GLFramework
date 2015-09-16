#pragma once
#include <GL/glew.h>
#include "Math/Matrix.hpp"
#include "Core/Texture.hpp"
#include <map>
#include <vector>

using namespace std;

class Mesh
{
public:
	Mesh();
	virtual void Render();
	virtual void RenderToTexture(const unsigned int fbo, const unsigned int texId);
	~Mesh();
	void SetProjectionMatrix(const Matrix4& projection);
	void SetViewMatrix(const Matrix4& view);
	void SetModelMatrix(const Matrix4& model);
	Matrix4 GetProjectionMatrix();
	Matrix4 GetViewMatrix();
	Matrix4 GetModelMatrix();
	virtual void SetShaders(const string vertexShaderPath, const string fragmentShaderPath);
	void SetWireframeMode(const bool showWireframe);
	void SetVertexBuffer(const float* vertexBuffer, const unsigned int length, const unsigned char vertexSize = 3);
	void SetNormalsBuffer(const float* normalsBuffer);
	void SetTexCoordsBuffer(const float* texCoordsBuffer);
	void BindUniform(string uniform);
	void SetUniformValue(string uniform, const int v);
	void SetUniformValue(string uniform, const unsigned int v);
	void SetUniformValue(string uniform, const Vec3& v);
	void SetUniformValue(string uniform, const Vec4& v);
	void SetUniformValue(string uniform, const Matrix4& v);
	void SetCastsShadow(const bool castsShadow);
	void SetReceivesShadow(const bool receivesShadow);
	void AddTexture(const Texture* tex);
	void RemoveTexture(const Texture* tex);
	void SetPosition(const Vec3& position);
protected:
	void generateNormals();
	void generateWireframe();
	void activateNormalsBuffer();
	void activateTexCoordsBuffer();
	void deactivateNormalsBuffer();
	void deactivateTexCoordsBuffer();
	unsigned int vertexCount;
	unsigned char vertexSize;
	float* vertexBuffer;
	float* normalsBuffer;
	float* texCoordsBuffer;
	float* wireframeVertexBuffer;
	GLuint vertexBufferID;
	GLuint normalsBufferID;
	GLuint texCoordsBufferID;
	GLuint wireframeVertexBufferID;
	GLuint programID;
	GLuint shadowTexID;
	GLuint texID;
	Matrix4 projection;
	Matrix4 view;
	Matrix4 model;
	bool showWireframe;
	map<string, GLuint> uniforms;
	bool castsShadow;
	bool receivesShadow;
	vector<const Texture*> textures;
};

