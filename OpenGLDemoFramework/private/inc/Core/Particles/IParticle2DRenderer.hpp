#pragma once

#include "Core/Particles/IParticle2D.hpp"

class IParticle2DRenderer
{
public:
    virtual void particleSpawned(const IParticle2D& particle) = 0;
    virtual void particleDied(const unsigned int index) = 0;
};