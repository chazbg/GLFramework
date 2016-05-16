#include <Core/Scene.hpp>

Scene::Scene()
{
}

Scene::~Scene()
{
}

void Scene::add(IMesh * mesh)
{
    meshes.push_back(mesh);
}

void Scene::remove(const IMesh* mesh)
{
    std::vector<IMesh*>::iterator it;
    it = std::find(meshes.begin(), meshes.end(), mesh);
    meshes.erase(it);
}

std::vector<IMesh*>& Scene::getChildren()
{
    return meshes;
}
