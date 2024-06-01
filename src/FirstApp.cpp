#include "FirstApp.h"

#include "ProjectInputController.h"
#include "ProjectCamera.h"
#include "ProjectModel.h"
#include "SimpleRenderSystem.h"
#include <stdexcept>
#include <array>
#include <chrono>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <algorithm>
#include <execution>
namespace lsmf
{

	FirstApp::FirstApp()
	{
		// Initialize Bullet
		btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
		btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
		btBroadphaseInterface* broadphase = new btDbvtBroadphase();
		btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
		m_DynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher, broadphase, solver, collisionConfiguration);
		 

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
        //camera.SetViewTarget(glm::vec3{ -1.f, -2.f,2.f }, glm::vec3{ 0.f, 0.f, 2.5f });

        auto viewerObject = ProjectGameObject::createGameObject();

		// set the viewer object to the camera position using the camera's view matrix 
		 viewerObject.m_Transform.translation = {0.f,-1.f,-10.f};
		//rotate by 180 degrees around the y-axis
		viewerObject.m_Transform.rotation = { 0.f, glm::pi<float>(), 0.f };

		 
        ProjectInputController cameraController{};

		m_DynamicsWorld->setGravity(btVector3(0, 9.8f, 0.f));

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!m_window.shouldClose())
		{
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            frameTime = std::min(frameTime, 0.5f);
            currentTime = newTime;


			//ImGui_ImplVulkan_NewFrame();
			//ImGui_ImplGlfw_NewFrame();
			//ImGui::NewFrame();
			//
			////// Create an ImGui window
			//if (ImGui::Begin("Game Object Controls"))
			//{
			//	//add a slider for controlling the gravity
			//	static float gravity = 9.8f;
			//	ImGui::SliderFloat("Gravity", &gravity, 0.f, 20.f);
			//	m_DynamicsWorld->setGravity(btVector3(0, gravity, 0.f));
			//
			//	// add a field for giving a file path to a model
			//	static char modelPath[256] = "";
			//	ImGui::InputText("Model Path", modelPath, sizeof(modelPath));
			//
			//	static char objectName[256] = "";
			//	ImGui::InputText("Name", objectName, sizeof(modelPath));
			//
			//	// add an input for the spawn position
			//	static glm::vec3 spawnPosition{ 0.f, 0.f, 0.f };
			//	ImGui::InputFloat3("Spawn Position", &spawnPosition.x);
			//
			//
			//	//Add a button for spawning a new game object
			//	if (ImGui::Button("Spawn New Game Object"))
			//	{
			//		// Code to spawn a new game object...
			//	}
			//
			//	// Add a list box for selecting and deleting game objects
			//	static int selected = -1;
			//	std::vector<const char*> items;
			//	for (const auto& object : m_GameObjects)
			//	{
			//		items.push_back(object.m_Name.c_str());
			//	}
			//	ImGui::ListBox("Game Objects", &selected, items.data(), static_cast<int>(items.size()), 4);
			//	if (ImGui::Button("Delete Selected Game Object"))
			//	{
			//		//remove the selected game object
			//		if (selected >= 0 && selected < m_GameObjects.size())
			//		{
			//			m_GameObjects.erase(m_GameObjects.begin() + selected);
			//			selected = -1;
			//		}
			//	}
			//}
			//ImGui::End();

			cameraController.updateParameters(m_window.getWindow(), m_DynamicsWorld.get());
			// Update the physics world
			m_DynamicsWorld->applyGravity();
			m_DynamicsWorld->stepSimulation(1 / 60.f, 10);

			for (ProjectGameObject& object : m_GameObjects)
			{
				object.Update();
			}

            cameraController.MoveInPlaneXZ(m_window.getWindow(), frameTime, viewerObject);
            camera.SetViewXYZ(viewerObject.m_Transform.translation, viewerObject.m_Transform.rotation);

            float aspect = m_renderer.getAspectRatio();

            camera.SetPerspectiveProjection(glm::radians(50.f), m_renderer.getAspectRatio(), 0.1f, 100.f);

			if (const auto commandBuffer = m_renderer.beginFrame())
			{
				m_renderer.beginSwapChainRenderPass(commandBuffer);

				simpleRenderSystem.renderGameObjects(commandBuffer, m_GameObjects, camera);

				//// Render ImGui
				//ImGui::Render();
				//ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);


				m_renderer.endSwapChainRenderPass(commandBuffer);
				m_renderer.endFrame();
			}

			m_DynamicsWorld->clearForces();
		}

		vkDeviceWaitIdle(m_device.device());
	}

	void FirstApp::LoadGameObjects()
	{
	//	const std::shared_ptr<ProjectModel> cubeModel = ProjectModel::CreateModelFromFile(m_device, "../models/ball.obj");
	//	const std::shared_ptr<ProjectModel> coneModel = ProjectModel::CreateModelFromFile(m_device, "../models/cone.obj");

	//	auto body = ProjectModel::CreatePhysicsFromFile("../models/ball.obj", m_DynamicsWorld.get(), false);

 //       auto game_object = ProjectGameObject::createGameObject(body);
 //       game_object.m_Model = cubeModel;
 //       game_object.m_Transform.translation = { 0.f, 0.f, -2.5f };
 //       game_object.m_Transform.scale= glm::vec3{1};
	//	body->translate({ 0.f, 0.f, -2.5f });
 //       //cube.m_Color = { 0.8f, 0.3f, 0.1f };
 //       m_GameObjects.push_back(std::move(game_object));

	//	auto body2 = ProjectModel::CreatePhysicsFromFile("../models/cone.obj", m_DynamicsWorld.get(), false);

 //       auto game_object2 = ProjectGameObject::createGameObject(body2);
	//	game_object2.m_Model = coneModel;
	//	game_object2.m_Transform.translation = { 0.f, 0.f, -2.5f };
	//	game_object2.m_Transform.scale= glm::vec3{1};
	//	body2->translate({ 0.f, -10.f, -1.0f });
 //       //cube.m_Color = { 0.8f, 0.3f, 0.1f };
 //       m_GameObjects.push_back(std::move(game_object2));

	//	// add a ground plane
	//	auto groundBody = ProjectModel::CreatePhysicsFromFile("../models/tapered_plane.obj", m_DynamicsWorld.get(), true);
	//	const std::shared_ptr<ProjectModel> planeModel = ProjectModel::CreateModelFromFile(m_device, "../models/tapered_plane.obj");

	//	groundBody->translate({ 0.f,5.f,0.f });
	//	auto plane = ProjectGameObject::createGameObject(groundBody);
	//	plane.m_Model = planeModel;
	//	plane.m_Transform.scale = glm::vec3{ 1.f };
	//	m_GameObjects.push_back(std::move(plane));

		SpawnObject("../models/tapered_plane.obj", true, { 0,3.f,0 });
		SpawnObject("../models/ball.obj", false, { 0,-5.f,2.5f });
		SpawnObject("../models/cone.obj", false, { 4.f,-5.f,0 });
		SpawnObject("../models/cube.obj", false, { 0,-5.f,-3 });


	}

	void FirstApp::SpawnObject(std::string filename, bool isStatic, btVector3 pos)
	{
		SpawnObject(filename, filename, isStatic, pos);
	}

	void FirstApp::SpawnObject(std::string filename, std::string physicsFilename, bool isStatic, btVector3 pos)
	{
		const std::shared_ptr<ProjectModel> model = ProjectModel::CreateModelFromFile(m_device, filename);
		auto body = ProjectModel::CreatePhysicsFromFile(physicsFilename, m_DynamicsWorld.get(), isStatic);
		body->translate(pos);
		auto game_object = ProjectGameObject::createGameObject(body);
		game_object.m_Model = model;
		m_GameObjects.push_back(std::move(game_object));
	}

}
