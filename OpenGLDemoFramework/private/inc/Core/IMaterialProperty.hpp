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

typedef IMaterialProperty<int>          IntProperty;
typedef IMaterialProperty<unsigned int> UintProperty;
typedef IMaterialProperty<float>        FloatProperty;
typedef IMaterialProperty<Vec2>         Vec2Property;
typedef IMaterialProperty<Vec3>         Vec3Property;
typedef IMaterialProperty<Matrix4>      Mat4Property;

typedef std::shared_ptr<IntProperty>   IntPropertySharedPtr;
typedef std::shared_ptr<UintProperty>  UintPropertySharedPtr;
typedef std::shared_ptr<FloatProperty> FloatPropertySharedPtr;
typedef std::shared_ptr<Vec2Property>  Vec2PropertySharedPtr;
typedef std::shared_ptr<Vec3Property>  Vec3PropertySharedPtr;
typedef std::shared_ptr<Mat4Property>  Mat4PropertySharedPtr;
