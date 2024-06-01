#pragma once

#include <btBulletDynamicsCommon.h>
#include <memory>
#include <vector>

#include "ProjectDevice.h"
#include "ProjectGameObject.h"
#include "ProjectRenderer.h"
#include "ProjectWindow.h"


namespace lsmf
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 1600;
		static constexpr int HEIGHT = 1200;
		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();
	private:
		void LoadGameObjects();


		ProjectWindow m_window{ WIDTH, HEIGHT, "VulkanPhysicsVisualizer - Leander Flossie" };
		ProjectDevice m_device{ m_window };
		ProjectRenderer m_renderer{ m_window, m_device };

		std::vector<ProjectGameObject> m_GameObjects;

		// Bullet Physics

		std::unique_ptr<btDiscreteDynamicsWorld> m_DynamicsWorld;

		void SpawnObject(std::string filename, bool isStatic = false, btVector3 pos = {});
		void SpawnObject(std::string filename, std::string physicsFilename, bool isStatic = false, btVector3 pos = {});
	};
}

