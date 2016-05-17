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
        const float duration);

    void deinit();

public:
	Vec2 emitterPos;
	Vec2 particlePos;
    float duration;
	float tangentAcceleration;
	float radialAcceleration;
	float remainingLife;
    float alpha;
};