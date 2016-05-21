#include "Core/Particles/SimpleEmitter.hpp"
#include <iostream>
#include <stdlib.h>

using namespace std;

SimpleEmitter::SimpleEmitter(const unsigned int particleCount,
    const float spawnInterval) :
    particleCount(particleCount),
    spawnInterval(spawnInterval),
    aliveParticles(0),
    nextSpawn(0.0f)
{
	for (unsigned int i = 0; i < particleCount; i++)
	{
		particles.push_back(new SimpleParticle());
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
	if (nextSpawn <= 0.0f)
	{
		SimpleParticle* p = particles[aliveParticles];

		float r1 = static_cast<float>(rand() % 200 - 100) / 1000.0f;
		float r2 = static_cast<float>(rand() % 200 - 100) / 10000.0f;
		float r3 = static_cast<float>(rand() % 100)       / 10000.0f;
		float r4 = static_cast<float>(rand() % 100)       / 1000000.0f;
		float r5 = static_cast<float>(rand() % 200 - 100) / 1000.0f;

        p->init(Vec2(0.0f, 0.0f),
            Vec2(0.0f + r1, 0.0f + r2),
            0.1f + r3,
			0.0005f + r4,
            20.0f,
			Vec2(r5, r5));

		aliveParticles++;
        nextSpawn = spawnInterval;
	}

	for (unsigned int i = 0; i < aliveParticles; i++)
	{
		SimpleParticle* p = particles[i];

		p->update(t);
		//cout << "Pos(" << i << "): " << p->particlePos.toString() << endl;

		if (p->remainingLife < 0)
		{
            p->deinit();
            SimpleParticle* lastAliveParticle = particles[aliveParticles - 1];
            particles[i] = lastAliveParticle;
            particles[aliveParticles - 1] = p;
            aliveParticles--;
		}
	}

    nextSpawn -= t;
}

const std::vector<SimpleParticle*>& SimpleEmitter::getParticles()
{
    return particles;
}

unsigned int SimpleEmitter::getAliveParticlesCount()
{
    return aliveParticles;
}
