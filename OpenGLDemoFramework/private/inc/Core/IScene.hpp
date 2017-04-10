#pragma once

#include "Core/INode.hpp"
#include <vector>
#include <memory>

class IScene
{
public:
    virtual ~IScene() {}
    virtual void add(std::shared_ptr<INode> node)              = 0;
    virtual void remove(const std::shared_ptr<INode> mesh)     = 0;
    virtual void remove(const unsigned int index)              = 0;
    virtual std::vector<std::shared_ptr<INode>>& getChildren() = 0;
    virtual void clear()                                       = 0;
};