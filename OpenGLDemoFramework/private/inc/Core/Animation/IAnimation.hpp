#pragma once

class IAnimation
{
public:
    virtual void update(const float delta) = 0;
};