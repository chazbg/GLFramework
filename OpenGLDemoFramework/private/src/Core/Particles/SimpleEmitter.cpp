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
		float r3 = static_cast<float>(rand() % 200)       / 10000.0f;
		float r4 = static_cast<float>(rand() % 200)       / 1000000.0f;
		float r5 = static_cast<float>(rand() % 200 - 100) / 1000.0f;

        p->init(Vec2(0.0f, 0.0f),
            Vec2(0.0f + r1, 0.0f + r2),
            0.05f + r3,
			0.0005f + r4,
            20.0f,
			Vec2(r5, r5));

		aliveParticles++;
        nextSpawn = spawnInterval;
        particleRenderer.particleSpawned(*p);
	}

	for (unsigned int i = 0; i < aliveParticles; i++)
	{
        IParticle2D* p = particles[i];

		p->update(t);
		//cout << "Pos(" << i << "): " << p->particlePos.toString() << endl;

		if (p->getRemainingLife() < 0)
		{
            p->deinit();
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
