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
	virtual void setProjectionMatrix(const Matrix4& projection);
	virtual void setViewMatrix(const Matrix4& view);
	virtual void setModelMatrix(const Matrix4& model);
	virtual Matrix4 getProjectionMatrix() const;
	virtual Matrix4 getViewMatrix() const;
	virtual Matrix4 getModelMatrix() const;
	virtual void setWireframeMode(const bool showWireframe);
	virtual void setVertices(const IVertexBufferObject& vertices);
	virtual void setNormals(const IVertexBufferObject& normals);
	virtual void setTexCoords(const IVertexBufferObject& texCoords);
	virtual int getVertexCount() const;
	virtual void setMaterial(IMaterial* material);
	virtual IMaterial& getMaterial() const;
	void BindUniform(string uniform);
	void SetUniformValue(string uniform, const int v);
	void SetUniformValue(string uniform, const unsigned int v);
	void SetUniformValue(string uniform, const Vec3& v);
	void SetUniformValue(string uniform, const Vec4& v);
	void SetUniformValue(string uniform, const Matrix4& v);
	void SetCastsShadow(const bool castsShadow);
	void SetReceivesShadow(const bool receivesShadow);
	void SetTexture(const Texture& tex);
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
	IMaterial* material;
	std::vector<IVertexBufferObject*> vbos;
	float* wireframeVertexBuffer;
	GLuint wireframeVertexBufferID;
	Matrix4 projection;
	Matrix4 view;
	Matrix4 model;
	bool showWireframe;
	map<string, GLuint> uniforms;
	bool castsShadow;
	bool receivesShadow;
};

