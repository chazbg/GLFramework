#pragma once

#include <Core/IScene.hpp>

class Scene : public IScene
{
public:
	Scene();
	~Scene();
	virtual void add(IMesh* mesh);
	virtual std::vector<IMesh*>& getChildren();
private:
	std::vector<IMesh*> meshes;
};