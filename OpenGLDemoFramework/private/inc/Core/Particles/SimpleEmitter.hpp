#pragma once

#include "Core/Particles/IEmitter.hpp"
#include "Core/Particles/SimpleParticle.hpp"
#include <queue>

class SimpleEmitter : public IEmitter
{
public:
	SimpleEmitter();
	~SimpleEmitter();
	virtual void updateParticles(const float t);
private:
	std::queue<SimpleParticle*> aliveParticles;
	std::queue<SimpleParticle*> deadParticles;
};