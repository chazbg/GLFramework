#pragma once

#include "Windowing/Window.hpp"
#include "Rendering/Renderer.hpp"
#include "Geometry/BlockMesh.hpp"
#include <Core/Scene.hpp>
#include <Core/PerspectiveCamera.hpp>
#include <Core/ShaderMaterial.hpp>

namespace CubeDemo
{
	class TestWindowApp : public IApplication
	{
	public:
		TestWindowApp() : renderer(0) {}
		~TestWindowApp() {}
		virtual void onInit() 
        {
            renderer = new Renderer();
            cube = new BlockMesh();
            material = new ShaderMaterial("Shaders/cube.vs", "Shaders/cube.fs");
            cube->setMaterial(material);
            scene.add(cube);
            camera.setPosition(Vec3(0, 15, 15));
            camera.setLookDirection(Vec3(0, 0, 0));
        }
		virtual void onUpdate(const unsigned int deltaTime) {}
		virtual void onRender(const unsigned int deltaTime)
		{
			renderer->clear(Vec4(0.0f, 0.0f, 0.2f, 0.0f));

            renderer->render(scene, camera);
		}

		virtual void onDestroy() { cout << "onDestroy" << endl; }
		virtual void onEvent(const unsigned int event) { cout << "onEvent: " << event << endl; }
		virtual void onMouseEvent(int button, int state, int x, int y) { cout << button << " " << state << " " << x << " " << y << endl; }
		virtual void onKeyboardEvent(unsigned char c, int x, int y) { cout << c << " " << x << " " << y << endl; }
	private:
		Renderer* renderer;
        Scene scene;
        PerspectiveCamera camera;
        BlockMesh* cube;
        ShaderMaterial* material;
	};

	void main()
	{
		WindowParameters params;
		params.width = 800;
		params.height = 800;
		params.posX = 100;
		params.posY = 100;
		params.name = "TestWindow";
		TestWindowApp app;
		Window window(params, app);
		window.startRenderLoop();
	}
}