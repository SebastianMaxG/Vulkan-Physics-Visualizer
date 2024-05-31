#include "FirstApp.h"
#include "SimpleRenderSystem.h"
#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lsmf
{

	FirstApp::FirstApp()
	{
		LoadGameObjects();
	}

	FirstApp::~FirstApp()
	{
	}
	void FirstApp::run()
	{
		SimpleRenderSystem simpleRenderSystem(m_device, m_renderer.getSwapChainRenderPass());
		while (!m_window.shouldClose())
		{
			glfwPollEvents();

			if (auto commandBuffer = m_renderer.beginFrame())
			{
				m_renderer.beginSwapChainRenderPass(commandBuffer);

				simpleRenderSystem.renderGameObjects(commandBuffer, m_GameObjects);

				m_renderer.endSwapChainRenderPass(commandBuffer);
				m_renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(m_device.device());
	}

	void FirstApp::LoadGameObjects()
	{
		std::vector<ProjectModel::Vertex> vertices
		{
			{{0.0f, -0.5f} ,{1.f,0.f,0.f}},
			{{0.5f, 0.5f},{0.f,1.f,0.f}},
			{{-0.5f, 0.5f},{0.f,0.f,1.0f}}
		};

		auto model = std::make_shared<ProjectModel>(m_device, vertices);

		auto triangle = ProjectGameObject::createGameObject();
		triangle.m_Model = model;
		triangle.m_Color = { 0.8f, 0.3f, 0.01f };
		triangle.m_Transform2D.translation = { 0.2f, 0.0f };
		triangle.m_Transform2D.scale = { 2.0f, 0.5f };
		triangle.m_Transform2D.rotation = glm::quarter_pi<float>();

		m_GameObjects.push_back(std::move(triangle));
	}
}