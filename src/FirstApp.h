#pragma once

#include "ProjectWindow.h"
#include "ProjectDevice.h"
#include "ProjectGameObject.h"
#include "ProjectRenderer.h"

#include <memory>
#include <vector>

namespace lsmf
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;
		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp& operator=(const FirstApp&) = delete;

		void run();
	private:
		void LoadGameObjects();

		ProjectWindow m_window{ WIDTH, HEIGHT, "Vulkan" };
		ProjectDevice m_device{ m_window };
		ProjectRenderer m_renderer{ m_window, m_device };

		std::vector<ProjectGameObject> m_GameObjects;

	};
}

