#pragma once

#include "Core/IScene.hpp"

class Scene : public IScene
{
public:
    Scene();
    ~Scene();
    virtual void add(std::shared_ptr<INode> node);
    virtual void remove(const std::shared_ptr<INode> mesh);
    virtual void remove(const unsigned int index);
    virtual std::vector<std::shared_ptr<INode>>& getChildren();
    virtual void clear();

private:
    std::vector<std::shared_ptr<INode>> meshes;
};