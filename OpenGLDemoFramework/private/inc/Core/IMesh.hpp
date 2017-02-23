#pragma once

#include "Core/IVertexBuffer.hpp"
#include "Core/IIndexBuffer.hpp"
#include "Core/IMaterial.hpp"
#include "Math/Matrix.hpp"
#include <vector>
#include <memory>

class IMesh;

typedef std::shared_ptr<IMesh> IMeshSharedPtr;

class IMesh
{
public:
    virtual ~IMesh() {}
    virtual const IIndexBuffer* getIBO() = 0;
    virtual std::vector<const IVertexBuffer*>& getVBOs() = 0;
    virtual void setIndices(const IIndexBuffer& indices) = 0;
    virtual void setVertices(const IVertexBuffer& vertices) = 0;
    virtual void setNormals(const IVertexBuffer& normals) = 0;
    virtual void setTangents(const IVertexBuffer& tangents) = 0;
    virtual void setBitangents(const IVertexBuffer& bitangents) = 0;
    virtual void setTexCoords(const IVertexBuffer& texCoords) = 0;
    virtual void setMaterial(IMaterial* material) = 0;
    virtual IMaterial& getMaterial() const = 0;
    virtual std::shared_ptr<IMesh> clone() = 0;
};