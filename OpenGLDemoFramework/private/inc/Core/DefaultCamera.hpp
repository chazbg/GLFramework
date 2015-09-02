#pragma once

#include <Core/ICamera.hpp>

class DefaultCamera : public ICamera
{
public:
	DefaultCamera();
	~DefaultCamera();

	virtual void setPosition(const Vec3& position);
	virtual Vec3 getPosition() const;
	virtual void setLookDirection(const Vec3& position);
	virtual Vec3 getLookDirection() const;
	virtual Matrix4 getViewMatrix() const;
};