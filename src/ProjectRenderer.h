#pragma once

#include "ProjectWindow.h"
#include "ProjectDevice.h"
#include "ProjectSwapChain.h"
#include "ProjectModel.h"

#include <memory>
#include <vector>
#include <cassert>

namespace lsmf
{
	class ProjectRenderer
	{
	public:
		ProjectRenderer(ProjectWindow &window, ProjectDevice &device );
		~ProjectRenderer();

		ProjectRenderer(const ProjectRenderer&) = delete;
		ProjectRenderer& operator=(const ProjectRenderer&) = delete;

		VkRenderPass getSwapChainRenderPass() const 
		{ 
			return m_swapChain->getRenderPass(); 
		}

		float getAspectRatio() const { return m_swapChain->extentAspectRatio(); }
		bool isFrameInProgress() const { return m_IsFrameStarted; }

		VkCommandBuffer getCurrentCommandBuffer() const 
		{ 
			assert(m_IsFrameStarted && "Cannot get command buffer when frame not in progress.");
			return m_commandBuffers[m_currentFrameIndex];
		}

		int getFrameIndex() const
		{
			assert(m_IsFrameStarted && "Cannot get frame index when frame not in progress.");
			return m_currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

	private:

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

		ProjectWindow& m_window;
		ProjectDevice& m_device;
		std::unique_ptr<ProjectSwapChain> m_swapChain;
		std::vector<VkCommandBuffer> m_commandBuffers;

		uint32_t m_currentImageIndex;
		int m_currentFrameIndex{ 0 };
		bool m_IsFrameStarted{ false };
	};
}


