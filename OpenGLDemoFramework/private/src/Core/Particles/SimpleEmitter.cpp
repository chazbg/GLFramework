#include "Core/Particles/SimpleEmitter.hpp"
#include "Core/Particles/GpuParticle2D.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;

SimpleEmitter::SimpleEmitter(
    IParticle2DRenderer& particleRenderer,
    const unsigned int particleCount,
    const float spawnInterval) :
    particleRenderer(particleRenderer),
    particleCount(particleCount),
    spawnInterval(spawnInterval),
    aliveParticles(0),
    nextSpawn(0.0f)
{
    for (unsigned int i = 0; i < particleCount; i++)
    {
        particles.push_back(new GpuParticle2D());
    }
}

SimpleEmitter::~SimpleEmitter()
{
    for (unsigned int i = 0; i < particleCount; i++)
    {
        delete particles[i];
    }
}

void SimpleEmitter::updateParticles(const float t)
{
    if (nextSpawn <= 0.0f && aliveParticles < particleCount)
    {
        IParticle2D* p = particles[aliveParticles];

        float r1 = static_cast<float>(rand() % 200 - 100) / 1000.0f;
        float r2 = static_cast<float>(rand() % 200 - 100) / 10000.0f;
        float r3 = static_cast<float>(rand() % 100) / 100.0f;
        float r4 = static_cast<float>(rand() % 100) / 100.0f;
        float r5 = static_cast<float>(rand() % 100) / 100.0f;
        float r6 = static_cast<float>(rand() % 100) / 100.0f;

        p->init(Vec2(0.0f),
            Vec2(0.0f + r1, 0.0f + r2),
            0.35 + 0.1 * r3,
            0.05 + 0.1 * r4,
            5.0f,
            Vec2(0.03f + 0.02f * r5),
            r6 * 6.28f);

        aliveParticles++;
        nextSpawn = spawnInterval;
        particleRenderer.particleSpawned(*p);
    }

    for (unsigned int i = 0; i < aliveParticles; i++)
    {
        IParticle2D* p = particles[i];

        p->update(t);

        if (p->getRemainingLife() < 0)
        {
            IParticle2D* lastAliveParticle = particles[aliveParticles - 1];
            particles[i] = lastAliveParticle;
            particles[aliveParticles - 1] = p;
            aliveParticles--;
            particleRenderer.particleDied(i);
        }
    }

    nextSpawn -= t;
}

const std::vector<IParticle2D*>& SimpleEmitter::getParticles()
{
    return particles;
}

unsigned int SimpleEmitter::getAliveParticlesCount()
{
    return aliveParticles;
}
