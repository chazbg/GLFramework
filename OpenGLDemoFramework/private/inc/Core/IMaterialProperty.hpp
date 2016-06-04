#pragma once

#include <memory>

#include "Math/Vector.hpp"
#include "Math/Matrix.hpp"

template <typename T>
class IMaterialProperty
{
public:
    virtual ~IMaterialProperty() {}
};

typedef std::shared_ptr<IMaterialProperty<int>>          IntPropertySharedPtr;
typedef std::shared_ptr<IMaterialProperty<unsigned int>> UintPropertySharedPtr;
typedef std::shared_ptr<IMaterialProperty<float>>        FloatPropertySharedPtr;
typedef std::shared_ptr<IMaterialProperty<Vec2>>         Vec2PropertySharedPtr;
typedef std::shared_ptr<IMaterialProperty<Vec3>>         Vec3PropertySharedPtr;
typedef std::shared_ptr<IMaterialProperty<Matrix4>>      Mat4PropertySharedPtr;
