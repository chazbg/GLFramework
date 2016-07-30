#pragma once
#include "Core/IMesh.hpp"

class Mesh : public IMesh
{
public:
    Mesh();
    Mesh(const Mesh& rhs);
    ~Mesh();
    virtual const IIndexBuffer* getIBO();
    virtual std::vector<const IVertexBuffer*>& getVBOs();
    virtual void setIndices(const IIndexBuffer& indices);
    virtual void setVertices(const IVertexBuffer& vertices);
    virtual void setNormals(const IVertexBuffer& normals);
    virtual void setTangents(const IVertexBuffer& tangents);
    virtual void setBitangents(const IVertexBuffer& bitangents);
    virtual void setTexCoords(const IVertexBuffer& texCoords);
    virtual void setMaterial(IMaterial* material);
    virtual IMaterial& getMaterial() const;
    virtual shared_ptr<IMesh> clone();
    void SetCastsShadow(const bool castsShadow);
    void SetReceivesShadow(const bool receivesShadow);
protected:
    float* generateNormals(const float* vertexBuffer, const unsigned int vertexCount);
    float* generateUVs(const float* vertexBuffer, const unsigned int vertexCount);
    IMaterial* material;
    const IIndexBuffer* ibo;
    std::vector<const IVertexBuffer*> vbos;
    bool castsShadow;
    bool receivesShadow;
};

