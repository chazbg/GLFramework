#pragma once

#include "Core/Particles/IEmitter.hpp"
#include "Core/Particles/SimpleParticle.hpp"
#include <vector>

class SimpleEmitter : public IEmitter
{
public:
	SimpleEmitter(const unsigned int particleCount,
        const float spawnInterval);
	~SimpleEmitter();
	virtual void updateParticles(const float t);
    const std::vector<SimpleParticle*>& getParticles();
    unsigned int getAliveParticlesCount();
private:
	std::vector<SimpleParticle*> particles;
    const unsigned int particleCount;
    const float spawnInterval;
    unsigned int aliveParticles;
    float nextSpawn;
};