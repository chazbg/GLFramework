#pragma once
#include <GL/glew.h>
#include "Math/Matrix.hpp"
#include "Core/IMesh.hpp"
#include <map>

using namespace std;

class Mesh : public IMesh
{
public:
    Mesh();
    ~Mesh();
    virtual const IIndexBuffer* getIBO();
    virtual std::vector<const IVertexBuffer*>& getVBOs();
    virtual void setModelMatrix(const Matrix4& model);
    virtual Matrix4 getModelMatrix() const;
    virtual void setWireframeMode(const bool showWireframe);
    virtual void setIndices(const IIndexBuffer& indices);
    virtual void setVertices(const IVertexBuffer& vertices);
    virtual void setNormals(const IVertexBuffer& normals);
    virtual void setTangents(const IVertexBuffer& tangents);
    virtual void setBitangents(const IVertexBuffer& bitangents);
    virtual void setTexCoords(const IVertexBuffer& texCoords);
    virtual void setMaterial(IMaterial* material);
    virtual IMaterial& getMaterial() const;
    virtual void addChild(IMesh* child);
    virtual std::vector<IMesh*>& getChildren();
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
    Vec3 getPosition();
protected:
    float* generateNormals(const float* vertexBuffer, const unsigned int vertexCount);
    float* generateUVs(const float* vertexBuffer, const unsigned int vertexCount);
    void generateWireframe();
    IMaterial* material;
    const IIndexBuffer* ibo;
    std::vector<const IVertexBuffer*> vbos;
    std::vector<IMesh*> children;
    float* wireframeVertexBuffer;
    GLuint wireframeVertexBufferID;
    Matrix4 model;
    bool showWireframe;
    bool castsShadow;
    bool receivesShadow;
};

