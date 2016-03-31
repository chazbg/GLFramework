#pragma once

#include "Core/IMesh.hpp"
#include <vector>

class IScene
{
public:
	virtual ~IScene() {}
	virtual void add(IMesh* mesh) = 0;
	virtual void remove(const IMesh* mesh) = 0;
	virtual std::vector<IMesh*>& getChildren() = 0;
};