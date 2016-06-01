#pragma once

#include "Core/Particles/IEmitter.hpp"
#include "Core/Particles/IParticle2D.hpp"
#include "Core/Particles/IParticle2DRenderer.hpp"
#include <vector>

class SimpleEmitter : public IEmitter
{
public:
    SimpleEmitter(
        IParticle2DRenderer& particleRenderer,
        const unsigned int particleCount,
        const float spawnInterval);
    ~SimpleEmitter();
    virtual void updateParticles(const float t);
    const std::vector<IParticle2D*>& getParticles();
    unsigned int getAliveParticlesCount();
private:
    IParticle2DRenderer& particleRenderer;
    std::vector<IParticle2D*> particles;
    const unsigned int particleCount;
    const float spawnInterval;
    unsigned int aliveParticles;
    float nextSpawn;
};