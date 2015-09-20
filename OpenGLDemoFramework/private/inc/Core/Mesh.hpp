#pragma once
#include <GL/glew.h>
#include "Math/Matrix.hpp"
#include "Core/Texture.hpp"
#include "Core/IMesh.hpp"
#include <map>

using namespace std;

class Mesh : public IMesh
{
public:
	Mesh();
	~Mesh();
	virtual std::vector<IVertexBufferObject*>& getVBOs();
	virtual void setModelMatrix(const Matrix4& model);
	virtual Matrix4 getModelMatrix() const;
	virtual void setWireframeMode(const bool showWireframe);
	virtual void setVertices(const IVertexBufferObject& vertices);
	virtual void setNormals(const IVertexBufferObject& normals);
	virtual void setTexCoords(const IVertexBufferObject& texCoords);
	virtual void setMaterial(IMaterial* material);
	virtual IMaterial& getMaterial() const;
	void SetUniformValue(string uniform, const int v);
	void SetUniformValue(string uniform, const unsigned int v);
	void SetUniformValue(string uniform, const Vec3& v);
	void SetUniformValue(string uniform, const Vec4& v);
	void SetUniformValue(string uniform, const Matrix4& v);
	void SetCastsShadow(const bool castsShadow);
	void SetReceivesShadow(const bool receivesShadow);
	void SetPosition(const Vec3& position);
    void SetRotation(const float thetaX, const float thetaY, const float thetaZ);
protected:
    float* generateNormals(const float* vertexBuffer, const unsigned int vertexCount);
	void generateWireframe();
	IMaterial* material;
	std::vector<IVertexBufferObject*> vbos;
	float* wireframeVertexBuffer;
	GLuint wireframeVertexBufferID;
	Matrix4 model;
	bool showWireframe;
	bool castsShadow;
	bool receivesShadow;
};

