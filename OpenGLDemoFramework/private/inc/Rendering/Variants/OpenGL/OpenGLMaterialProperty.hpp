#pragma once

#include "Core/IMaterialProperty.hpp"

template <typename T>
class OpenGLMaterialProperty : public IMaterialProperty<T>
{
public:
    std::string name;
    int location;
    T value;
    unsigned int internalLocation;
};

typedef std::shared_ptr<OpenGLMaterialProperty<int>>          OpenGLIntPropertySharedPtr;
typedef std::shared_ptr<OpenGLMaterialProperty<unsigned int>> OpenGLUintPropertySharedPtr;
typedef std::shared_ptr<OpenGLMaterialProperty<float>>        OpenGLFloatPropertySharedPtr;
typedef std::shared_ptr<OpenGLMaterialProperty<Vec2>>         OpenGLVec2PropertySharedPtr;
typedef std::shared_ptr<OpenGLMaterialProperty<Vec3>>         OpenGLVec3PropertySharedPtr;
typedef std::shared_ptr<OpenGLMaterialProperty<Matrix4>>      OpenGLMat4PropertySharedPtr;