#pragma once

#include "Core/Particles/IParticle.hpp"
#include "Math/Vector.hpp"

class IParticle2D : public IParticle
{
public:
    virtual ~IParticle2D() {}
    virtual void update(const float t) = 0;
    virtual float getRemainingLife() const = 0;
    virtual float getDuration() const = 0;
    virtual void  setTangentialAcceleration(const float value) = 0;
    virtual float getTangentialAcceleration() const = 0;
    virtual float getMaxTangentialVelocity() const = 0;
    virtual void  setRadialAcceleration(const float value) = 0;
    virtual float getRadialAcceleration() const = 0;
    virtual float getMaxRadialVelocity() const = 0;
    virtual void  setScale(const Vec2& scale) = 0;
    virtual Vec2  getScale() const = 0;
    virtual void  setPhase(const float phase) = 0;
    virtual float getPhase() const = 0;
    virtual void init(
        const Vec2& emitterPos,
        const Vec2& particlePos,
        const float tangentAcceleration,
        const float radialAcceleration,
        const float duration,
        const Vec2 & scale,
        const float phase) = 0;
    virtual void deinit() = 0;
};