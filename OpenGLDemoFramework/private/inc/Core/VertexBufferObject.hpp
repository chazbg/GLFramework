#pragma once

#include <Core/IVertexBufferObject.hpp>

class VertexBufferObject : public IVertexBufferObject
{
public:
	VertexBufferObject();
	VertexBufferObject(const float* rawData, const int vertexCount, const int attributeSize);
	~VertexBufferObject();

	virtual int getId() const;
	virtual int getVertexCount() const;
	virtual int getAttributeSize() const;
private:
	int id;
	int vertexCount;
	int attributeSize;
};