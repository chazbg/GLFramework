#pragma once

#include "Core/IMaterialProperty.hpp"

template <typename T>
class OpenGLMaterialProperty : public IMaterialProperty<T>
{
public:
    std::string name;
    int location;
    T value;
};