#include "Core/Scene.hpp"
#include <iostream>

using namespace std;

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::add(std::shared_ptr<INode> mesh)
{
    meshes.push_back(mesh);
}

void Scene::remove(const std::shared_ptr<INode> mesh)
{
    auto it = find(meshes.begin(), meshes.end(), mesh);
    if (it != meshes.end())
    {
        meshes.erase(it);
    }    
}

void Scene::remove(const unsigned int index)
{
    if (index < meshes.size())
    {
        meshes.erase(meshes.begin() + index);
    }
    else
    {
        cout << "Index out of range: " << index 
             << ". Scene has currently " << meshes.size() << "children." << endl;
    }
}

std::vector<std::shared_ptr<INode>>& Scene::getChildren()
{
    return meshes;
}
