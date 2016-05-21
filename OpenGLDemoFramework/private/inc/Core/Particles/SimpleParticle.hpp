#pragma once

#include "Core/Particles/IParticle.hpp"
#include "Math/Vector.hpp"

class SimpleParticle : public IParticle
{
public:
	SimpleParticle();
	virtual void update(const float t);
    
    void init(
        const Vec2& emitterPos,
        const Vec2& particlePos,
        const float tangentAcceleration,
        const float radialAcceleration,
        const float duration,
		const Vec2 & scale);

    void deinit();

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