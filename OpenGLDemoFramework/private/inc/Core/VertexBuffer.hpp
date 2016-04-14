#pragma once

#include "Core/IVertexBuffer.hpp"

class VertexBuffer : public IVertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const float* rawData, const int vertexCount, const int attributeSize);
	~VertexBuffer();

	virtual int getId() const;
	virtual int getVertexCount() const;
	virtual int getAttributeSize() const;
private:
	int id;
	int vertexCount;
	int attributeSize;
};