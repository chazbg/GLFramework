#pragma once

class IMaterial
{
public:
	virtual ~IMaterial() {}
	virtual int getId() const = 0;
};