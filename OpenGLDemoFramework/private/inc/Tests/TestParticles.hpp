#pragma once

#include "Core/Particles/SimpleEmitter.hpp"

void TestParticles1()
{
	SimpleEmitter emitter;

	for (int i = 0; i < 20; i++)
	{
		emitter.updateParticles(0.16);
	}
}