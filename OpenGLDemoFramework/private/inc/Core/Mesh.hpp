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
    virtual IIndexBufferObject* getIBO();
	virtual std::vector<IVertexBufferObject*>& getVBOs();
	virtual void setModelMatrix(const Matrix4& model);
	virtual Matrix4 getModelMatrix() const;
	virtual void setWireframeMode(const bool showWireframe);
    virtual void setIndices(const IIndexBufferObject& indices);
	virtual void setVertices(const IVertexBufferObject& vertices);
	virtual void setNormals(const IVertexBufferObject& normals);
	virtual void setTexCoords(const IVertexBufferObject& texCoords);
	virtual void setMaterial(IMaterial* material);
	virtual IMaterial& getMaterial() const;
    virtual void addChild(IMesh* child);
    virtual std::vector<IMesh*>& getChildren();
    virtual void SetScale(const Vec3& scale);
    virtual void SetPosition(const Vec3& position);
    virtual void SetRotation(const float thetaX, const float thetaY, const float thetaZ); //TODO: Use Vec3
    virtual void Scale(const float scaleX, const float scaleY, const float scaleZ);//TODO: Use Vec3
    virtual void Rotate(const float thetaX, const float thetaY, const float thetaZ);//TODO: Use Vec3
    virtual void Translate(const float transX, const float transY, const float transZ);//TODO: Use Vec3
	void SetUniformValue(string uniform, const int v);
	void SetUniformValue(string uniform, const unsigned int v);
	void SetUniformValue(string uniform, const Vec3& v);
	void SetUniformValue(string uniform, const Vec4& v);
	void SetUniformValue(string uniform, const Matrix4& v);
	void SetCastsShadow(const bool castsShadow);
	void SetReceivesShadow(const bool receivesShadow);
protected:
    float* generateNormals(const float* vertexBuffer, const unsigned int vertexCount);
    float* generateUVs(const float* vertexBuffer, const unsigned int vertexCount);
	void generateWireframe();
	IMaterial* material;
    IIndexBufferObject* ibo;
	std::vector<IVertexBufferObject*> vbos;
    std::vector<IMesh*> children;
	float* wireframeVertexBuffer;
	GLuint wireframeVertexBufferID;
	Matrix4 scale;
    Matrix4 rotation;
    Matrix4 translation;
	bool showWireframe;
	bool castsShadow;
	bool receivesShadow;
};

