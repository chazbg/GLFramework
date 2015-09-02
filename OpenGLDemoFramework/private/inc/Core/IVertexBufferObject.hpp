#pragma once

class IVertexBufferObject
{
public:
	virtual ~IVertexBufferObject() {}
	virtual int getId() const = 0;
	virtual int getVertexCount() const = 0;
	virtual int getAttributeSize() const = 0;
};