#include "Core/Particles/SimpleParticle.hpp"

SimpleParticle::SimpleParticle()
{

}

void SimpleParticle::update(const float t)
{
	remainingLife -= t;
	Vec2 radialDir = (particlePos - emitterPos).normalize();
	Vec2 tangentDir(radialDir.y, -radialDir.x);
	particlePos += tangentAcceleration * tangentDir * t + radialAcceleration * radialDir * t;
    alpha = remainingLife / duration;
}

void SimpleParticle::init(
    const Vec2 & emitterPos, 
    const Vec2 & particlePos, 
    const float tangentAcceleration, 
    const float radialAcceleration, 
    const float duration)
{
    this->emitterPos          = emitterPos;
    this->particlePos         = particlePos;
    this->duration            = duration;
    this->tangentAcceleration = tangentAcceleration;
    this->radialAcceleration  = radialAcceleration;
    this->remainingLife       = duration;
    this->alpha               = 1.0f;
}

void SimpleParticle::deinit()
{
    emitterPos          = Vec2(0.0f, 0.0f);
    particlePos         = Vec2(0.0f, 0.0f);
    duration            = 0.0f;
    tangentAcceleration = 0.0f;
    radialAcceleration  = 0.0f;
    remainingLife       = 0.0f;
    alpha               = 0.0f;
}
