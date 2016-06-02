#include "Core/Particles/SimpleParticle.hpp"
#include <algorithm>

SimpleParticle::SimpleParticle() :
    MAX_TANGENT_VELOCITY(0.07f),
    MAX_RADIAL_VELOCITY(0.03f)
{

}

void SimpleParticle::update(const float t)
{
    remainingLife -= t;
    Vec2 radialDir = (particlePos - emitterPos).normalize();
    Vec2 tangentDir(radialDir.y, -radialDir.x);
    tangentVelocity = std::min(MAX_TANGENT_VELOCITY, tangentVelocity + tangentAcceleration);
    radialVelocity = std::min(MAX_RADIAL_VELOCITY, radialVelocity + radialAcceleration);
    particlePos += tangentVelocity * tangentDir * t + radialVelocity * radialDir * t;
    alpha = remainingLife / duration;    
}

float SimpleParticle::getRemainingLife() const
{
    return remainingLife;
}

float SimpleParticle::getDuration() const
{
    return duration;
}

void SimpleParticle::setTangentialAcceleration(const float value)
{
    tangentAcceleration = value;
}

float SimpleParticle::getTangentialAcceleration() const
{
    return tangentAcceleration;
}

float SimpleParticle::getMaxTangentialVelocity() const
{
    return MAX_TANGENT_VELOCITY;
}

void SimpleParticle::setRadialAcceleration(const float value)
{
    radialAcceleration = value;
}

float SimpleParticle::getRadialAcceleration() const
{
    return radialAcceleration;
}

float SimpleParticle::getMaxRadialVelocity() const
{
    return MAX_RADIAL_VELOCITY;
}

void SimpleParticle::setScale(const Vec2 & scale)
{
    this->scale = scale;
}

Vec2 SimpleParticle::getScale() const
{
    return scale;
}

void SimpleParticle::setPhase(const float phase)
{
}

float SimpleParticle::getPhase() const
{
    return 0.0f;
}

void SimpleParticle::init(
    const Vec2 & emitterPos, 
    const Vec2 & particlePos, 
    const float tangentAcceleration, 
    const float radialAcceleration, 
    const float duration,
    const Vec2 & scale,
    const float phase)
{
    this->emitterPos          = emitterPos;
    this->particlePos         = particlePos;
    this->tangentAcceleration = tangentAcceleration;
    this->radialAcceleration  = radialAcceleration;
    this->duration            = duration;
    this->scale               = scale;
    this->remainingLife       = duration;
    this->alpha               = 1.0f;
    this->tangentVelocity     = 0.0f;
    this->radialVelocity      = 0.0f;
}

void SimpleParticle::deinit()
{
    emitterPos          = Vec2(0.0f, 0.0f);
    particlePos         = Vec2(0.0f, 0.0f);
    tangentAcceleration = 0.0f;
    radialAcceleration  = 0.0f;
    duration            = 0.0f;
    scale               = Vec2(0.0f, 0.0f);
    remainingLife       = 0.0f;
    alpha               = 0.0f;
    tangentVelocity     = 0.0f;
    radialVelocity      = 0.0f;
}
