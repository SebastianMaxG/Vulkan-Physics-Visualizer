#include "FirstApp.h"

#include "ProjectInputController.h"
#include "ProjectCamera.h"
#include "ProjectModel.h"
#include "SimpleRenderSystem.h"
#include <stdexcept>
#include <array>
#include <chrono>

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
        ProjectCamera camera{};
        //camera.SetViewDirection(glm::vec3{ 0.f }, glm::vec3{ 0.5f , 0.f, 1.f });
        camera.SetViewTarget(glm::vec3{ -1.f, -2.f,2.f }, glm::vec3{ 0.f, 0.f, 2.5f });

        auto viewerObject = ProjectGameObject::createGameObject();
        ProjectInputController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!m_window.shouldClose())
		{
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            frameTime = std::min(frameTime, 0.5f);
            currentTime = newTime;

            cameraController.MoveInPlaneXZ(m_window.getWindow(), frameTime, viewerObject);
            camera.SetViewXYZ(viewerObject.m_Transform.translation, viewerObject.m_Transform.rotation);

            float aspect = m_renderer.getAspectRatio();

            camera.SetPerspectiveProjection(glm::radians(50.f), m_renderer.getAspectRatio(), 0.1f, 10.f);

			if (const auto commandBuffer = m_renderer.beginFrame())
			{
				m_renderer.beginSwapChainRenderPass(commandBuffer);

				simpleRenderSystem.renderGameObjects(commandBuffer, m_GameObjects, camera);

				m_renderer.endSwapChainRenderPass(commandBuffer);
				m_renderer.endFrame();
			}

		}

		vkDeviceWaitIdle(m_device.device());
	}

	void FirstApp::LoadGameObjects()
	{
		const std::shared_ptr<ProjectModel> cubeModel = ProjectModel::CreateModelFromFile(m_device, "../models/smooth_vase.obj");

        auto game_object = ProjectGameObject::createGameObject();
        game_object.m_Model = cubeModel;
        game_object.m_Transform.translation = { 0.f, 0.f, -2.5f };
        game_object.m_Transform.scale= glm::vec3{3};
        //cube.m_Color = { 0.8f, 0.3f, 0.1f };
        m_GameObjects.push_back(std::move(game_object));

	}
}