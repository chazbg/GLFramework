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

std::vector<IMesh*>& Scene::getChildren()
{
	return meshes;
}
