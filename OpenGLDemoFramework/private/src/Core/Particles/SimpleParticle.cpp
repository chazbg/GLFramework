#include "Core/Particles/SimpleParticle.hpp"

SimpleParticle::SimpleParticle(
	const Vec2& emitterPos,
	const Vec2& particlePos,
	const float tangentAcceleration,
	const float radialAcceleration,
	const float remainingLife) :
	emitterPos(emitterPos),
	particlePos(particlePos),
	tangentAcceleration(tangentAcceleration),
	radialAcceleration(radialAcceleration),
	remainingLife(remainingLife)
{

}

void SimpleParticle::update(const float t)
{
	remainingLife -= t;
	Vec2 radialDir = (particlePos - emitterPos).normalize();
	Vec2 tangentDir(radialDir.y, -radialDir.x);
	particlePos += tangentAcceleration * tangentDir * t + radialAcceleration * radialDir * t;
}