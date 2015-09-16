#pragma once
#include <vector>
#include <Core/Texture.hpp>

class IMaterial
{
public:
	virtual ~IMaterial() {}
	virtual int getId() const = 0;
	virtual std::vector<const Texture*> getTextures() const = 0; //TODO: Don't copy the vector.
};