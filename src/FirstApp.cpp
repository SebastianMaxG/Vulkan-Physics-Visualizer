#include "FirstApp.h"

#include "ProjectInputController.h"
#include  "ProjectCamera.h"
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

    std::unique_ptr<ProjectModel> createCubeModel(ProjectDevice& device, glm::vec3 offset)
	{
        ProjectModel::Builder modelBuilder{};

        modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };
        for (auto& v : modelBuilder.vertices) {
            v.position += offset;
        }

        modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

        return std::make_unique<ProjectModel>(device, modelBuilder);
    }
	void FirstApp::LoadGameObjects()
	{
		const std::shared_ptr<ProjectModel> cubeModel = createCubeModel(m_device, { 0.f, 0.f, 0.f });

        auto cube = ProjectGameObject::createGameObject();
        cube.m_Model = cubeModel;
        cube.m_Transform.translation = { 0.f, 0.f, -2.5f };
        cube.m_Transform.scale= { 0.5f, 0.5f, 0.5f };
        //cube.m_Color = { 0.8f, 0.3f, 0.1f };
        m_GameObjects.push_back(std::move(cube));

	}
}