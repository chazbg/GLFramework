#pragma once

class IEmitter
{
public:
	virtual ~IEmitter() {}
	virtual void updateParticles(const float t) = 0;
};