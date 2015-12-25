#pragma once

#include <vector>
#include <Core/IVertexBufferObject.hpp>
#include <Core/IIndexBufferObject.hpp>
#include <Core/IMaterial.hpp>
#include <Math/Matrix.hpp>

class IMesh
{
public:
	virtual ~IMesh() {}
    virtual IIndexBufferObject* getIBO() = 0;
	virtual std::vector<IVertexBufferObject*>& getVBOs() = 0;
	virtual void setModelMatrix(const Matrix4& model) = 0;
	virtual Matrix4 getModelMatrix() const = 0;
	virtual void setWireframeMode(const bool showWireframe) = 0;
    virtual void setIndices(const IIndexBufferObject& indices) = 0;
	virtual void setVertices(const IVertexBufferObject& vertices) = 0;
	virtual void setNormals(const IVertexBufferObject& normals) = 0;
	virtual void setTexCoords(const IVertexBufferObject& texCoords) = 0;
	virtual void setMaterial(IMaterial* material) = 0;
	virtual IMaterial& getMaterial() const = 0;
    virtual void addChild(IMesh* child) = 0;
    virtual std::vector<IMesh*>& getChildren() = 0;
};