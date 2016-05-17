#pragma once

#include "Core/Particles/IParticle.hpp"
#include "Math/Vector.hpp"

class SimpleParticle : public IParticle
{
public:
	SimpleParticle(
		const Vec2& emitterPos, 
		const Vec2& particlePos,
		const float tangentAcceleration,
		const float radialAcceleration,
		const float remainingLife);

	virtual void update(const float t);

public:
	Vec2 emitterPos;
	Vec2 particlePos;
	float tangentAcceleration;
	float radialAcceleration;
	float remainingLife;
};