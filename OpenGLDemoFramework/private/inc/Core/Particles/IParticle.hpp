#pragma once

class IParticle
{
public:
	virtual ~IParticle() {}
	virtual void update(const float t) = 0;
};