#pragma once

#include <vector>
#include <Core/IVertexBufferObject.hpp>
#include <Core/IMaterial.hpp>
#include <Math/Matrix.hpp>

class IMesh
{
public:
	virtual ~IMesh() {}
	virtual std::vector<IVertexBufferObject*>& getVBOs() = 0;
	virtual void setProjectionMatrix(const Matrix4& projection) = 0;
	virtual void setViewMatrix(const Matrix4& view) = 0;
	virtual void setModelMatrix(const Matrix4& model) = 0;
	virtual Matrix4 getProjectionMatrix() const = 0;
	virtual Matrix4 getViewMatrix() const = 0;
	virtual Matrix4 getModelMatrix() const = 0;
	virtual void setWireframeMode(const bool showWireframe) = 0;
	virtual void setVertices(const IVertexBufferObject& vertices) = 0;
	virtual void setNormals(const IVertexBufferObject& normals) = 0;
	virtual void setTexCoords(const IVertexBufferObject& texCoords) = 0;
	virtual int getVertexCount() const = 0;
	virtual void setMaterial(IMaterial* material) = 0;
	virtual IMaterial& getMaterial() const = 0;
};