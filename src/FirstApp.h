#pragma once

#include "ProjectWindow.h"
#include "ProjectPipeline.h"
#include "ProjectDevice.h"
#include "ProjectSwapChain.h"
#include "ProjectModel.h"
#include <memory>


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
		void LoadModels();
		void createPipelineLayout();
		void createPipeline();
		void createCommandBuffers();
		void freeCommandBuffers();
		void drawFrame();

		void recreateSwapChain();
		void recordCommandBuffer(int imageIndex);

		ProjectWindow m_window{ WIDTH, HEIGHT, "Vulkan" };
		ProjectDevice m_device{ m_window };
		std::unique_ptr<ProjectSwapChain> m_swapChain;
		//ProjectPipeline m_pipeline{ m_device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv" , ProjectPipeline::defaultPipelineConfigInfo(m_width,m_height) };
		std::unique_ptr<ProjectPipeline> m_pipeline;
		VkPipelineLayout m_pipelineLayout;
		std::vector<VkCommandBuffer> m_commandBuffers;
		std::unique_ptr<ProjectModel> m_model;

	};
}

