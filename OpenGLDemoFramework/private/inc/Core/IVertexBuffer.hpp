#pragma once

class IVertexBuffer
{
public:
	virtual ~IVertexBuffer() {}
	virtual int getId() const = 0;
	virtual int getVertexCount() const = 0;
	virtual int getAttributeSize() const = 0;
};