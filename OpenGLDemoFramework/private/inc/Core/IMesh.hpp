#pragma once

#include <vector>
#include <Core/IVertexBuffer.hpp>
#include <Core/IIndexBuffer.hpp>
#include <Core/IMaterial.hpp>
#include <Math/Matrix.hpp>

class IMesh
{
public:
	virtual ~IMesh() {}
    virtual IIndexBuffer* getIBO() = 0;
	virtual std::vector<IVertexBuffer*>& getVBOs() = 0;
	virtual void setModelMatrix(const Matrix4& model) = 0;
	virtual Matrix4 getModelMatrix() const = 0;
	virtual void setWireframeMode(const bool showWireframe) = 0;
    virtual void setIndices(const IIndexBuffer& indices) = 0;
	virtual void setVertices(const IVertexBuffer& vertices) = 0;
	virtual void setNormals(const IVertexBuffer& normals) = 0;
	virtual void setTangents(const IVertexBuffer& tangents) = 0;
	virtual void setBitangents(const IVertexBuffer& bitangents) = 0;
	virtual void setTexCoords(const IVertexBuffer& texCoords) = 0;
	virtual void setMaterial(IMaterial* material) = 0;
	virtual IMaterial& getMaterial() const = 0;
    virtual void addChild(IMesh* child) = 0;
    virtual std::vector<IMesh*>& getChildren() = 0;
    virtual void Scale(const float scaleX, const float scaleY, const float scaleZ) = 0;//TODO: Use Vec3
    virtual void Rotate(const float thetaX, const float thetaY, const float thetaZ) = 0;//TODO: Use Vec3
    virtual void Translate(const float transX, const float transY, const float transZ) = 0;//TODO: Use Vec3
};