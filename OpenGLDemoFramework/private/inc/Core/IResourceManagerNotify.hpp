#pragma once

#include "Core/IMaterial.hpp"

class IResourceManagerNotify
{
public:
    virtual ~IResourceManagerNotify() {}
    virtual void materialCreated(IMaterial& material) = 0;
    virtual void materialDestroyed(IMaterial& material) = 0;
};