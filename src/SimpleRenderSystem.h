#pragma once

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

		void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<ProjectGameObject> &gameObjects );
	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);


		ProjectDevice& m_device;

		std::unique_ptr<ProjectPipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;

	};
}

