#pragma once

#include "Core/IVertexBufferObject.hpp"

class VertexBufferObject : public IVertexBufferObject
{
public:
    VertexBufferObject();
	VertexBufferObject(const float* rawData, const int vertexCount, const int attributeSize);
	~VertexBufferObject();

	virtual int getId() const;
	virtual int getVertexCount() const;
	virtual int getAttributeSize() const;
    virtual float* getData() const;
private:
    static int nextId;
    static int getNextId();

	int id;
	int vertexCount;
	int attributeSize;
    float* data;
};