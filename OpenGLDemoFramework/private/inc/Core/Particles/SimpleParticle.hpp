#pragma once

#include "Core/Particles/IParticle2D.hpp"
#include "Math/Vector.hpp"

class SimpleParticle : public IParticle2D
{
public:
    SimpleParticle();
    virtual void update(const float t);
    virtual float getRemainingLife() const;
    virtual float getDuration() const;
    virtual void  setTangentialAcceleration(const float value);
    virtual float getTangentialAcceleration() const;
    virtual float getMaxTangentialVelocity() const;
    virtual void  setRadialAcceleration(const float value);
    virtual float getRadialAcceleration() const;
    virtual float getMaxRadialVelocity() const;
    virtual void  setScale(const Vec2& scale);
    virtual Vec2  getScale() const;
    virtual void  setPhase(const float phase);
    virtual float getPhase() const;
    virtual void init(
        const Vec2& emitterPos,
        const Vec2& particlePos,
        const float tangentAcceleration,
        const float radialAcceleration,
        const float duration,
        const Vec2& scale,
        const float phase);
    virtual void deinit();

public:
    const float MAX_TANGENT_VELOCITY;
    const float MAX_RADIAL_VELOCITY;
    Vec2 emitterPos;
    Vec2 particlePos;
    float tangentAcceleration;
    float radialAcceleration;
    float duration;
    Vec2 scale;
    float remainingLife;
    float alpha;    
    float tangentVelocity;
    float radialVelocity;
};