#include "Core/Particles/SimpleEmitter.hpp"
#include <iostream>

using namespace std;

SimpleEmitter::SimpleEmitter()
{
	int poolSize = 100;
	for (int i = 0; i < poolSize; i++)
	{
		deadParticles.push(new SimpleParticle(Vec2(0, 0), Vec2(0, 0), 0, 0, 0));
	}
}

SimpleEmitter::~SimpleEmitter()
{
	while (!aliveParticles.empty())
	{
		SimpleParticle* p = aliveParticles.front();
		delete p;
		aliveParticles.pop();
	}

	while (!deadParticles.empty())
	{
		SimpleParticle* p = deadParticles.front();
		delete p;
		deadParticles.pop();
	}
}

void SimpleEmitter::updateParticles(const float t)
{
	if (!deadParticles.empty())
	{
		SimpleParticle* p = deadParticles.front();
		deadParticles.pop();

		p->particlePos = Vec2(0, 0.1);
		p->radialAcceleration = 0.1;
		p->tangentAcceleration = 0.1;
		p->remainingLife = 20;

		aliveParticles.push(p);
	}

	for (int i = 0; i < aliveParticles.size(); i++)
	{
		SimpleParticle* p = aliveParticles.front();
		aliveParticles.pop();

		p->update(t);
		cout << "Pos(" << i << "): " << p->particlePos.toString() << endl;

		if (p->remainingLife < 0)
		{
			p->particlePos = Vec2(0, 0);
			p->radialAcceleration = 0;
			p->tangentAcceleration = 0;
			p->remainingLife = 0;
		}
		else
		{
			aliveParticles.push(p);
		}
	}
}