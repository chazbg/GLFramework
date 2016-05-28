#include "Core/Particles/GpuParticle2D.hpp"

GpuParticle2D::GpuParticle2D() :
    MAX_TANGENT_VELOCITY(0.07f),
    MAX_RADIAL_VELOCITY(0.03f)
{
}

void GpuParticle2D::update(const float t)
{
    remainingLife -= t;
}

float GpuParticle2D::getRemainingLife() const
{
    return remainingLife;
}

float GpuParticle2D::getDuration() const
{
    return duration;
}

void GpuParticle2D::setTangentialAcceleration(const float value)
{
    tangentAcceleration = value;
}

float GpuParticle2D::getTangentialAcceleration() const
{
    return tangentAcceleration;
}

float GpuParticle2D::getMaxTangentialVelocity() const
{
    return MAX_TANGENT_VELOCITY;
}

void GpuParticle2D::setRadialAcceleration(const float value)
{
    radialAcceleration = value;
}

float GpuParticle2D::getRadialAcceleration() const
{
    return radialAcceleration;
}

float GpuParticle2D::getMaxRadialVelocity() const
{
    return MAX_RADIAL_VELOCITY;
}

void GpuParticle2D::setScale(const Vec2 & scale)
{
    this->scale = scale;
}

Vec2 GpuParticle2D::getScale() const
{
    return scale;
}

void GpuParticle2D::init(
    const Vec2 & emitterPos, 
    const Vec2 & particlePos, 
    const float tangentAcceleration, 
    const float radialAcceleration, 
    const float duration, 
    const Vec2 & scale)
{
    this->tangentAcceleration = tangentAcceleration;
    this->radialAcceleration  = radialAcceleration;
    this->duration            = duration;
    this->scale               = scale;
    this->remainingLife       = duration;
}

void GpuParticle2D::deinit()
{
    tangentAcceleration = 0.0f;
    radialAcceleration  = 0.0f;
    duration            = 0.0f;
    scale               = Vec2(0.0f, 0.0f);
    remainingLife       = 0.0f;
}
