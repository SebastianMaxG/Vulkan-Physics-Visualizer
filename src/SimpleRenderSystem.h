#pragma once

#include "ProjectCamera.h"
#include "ProjectPipeline.h"
#include "ProjectDevice.h"
#include "ProjectGameObject.h"

#include <memory>
#include <vector>

namespace lsmf
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(ProjectDevice &device, VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<ProjectGameObject> &gameObjects , const ProjectCamera &camera);
	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);


		ProjectDevice& m_device;

		std::unique_ptr<ProjectPipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;

		const std::shared_ptr<ProjectModel> m_VelocityVector = ProjectModel::CreateModelFromFile(m_device, "../models/speedVector.obj");
		const std::shared_ptr<ProjectModel> m_ForceVector = ProjectModel::CreateModelFromFile(m_device, "../models/forceVector.obj");
		const std::shared_ptr<ProjectModel> m_ExtraVector = ProjectModel::CreateModelFromFile(m_device, "../models/extraVector.obj");

	};
}

