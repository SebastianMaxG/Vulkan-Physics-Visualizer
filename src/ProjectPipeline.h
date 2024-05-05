#pragma once
#include "ProjectDevice.h"

#include <string>
#include <vector>

namespace lsmf
{
	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;

		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;

	};

	class ProjectPipeline
	{
	public:
		ProjectPipeline
		(
			ProjectDevice &device, 
			const std::string& vertFilepath, 
			const std::string& fragFilepath, 
			const PipelineConfigInfo& configInfo
		);
		~ProjectPipeline();

		ProjectPipeline(const ProjectPipeline&) = delete;
		ProjectPipeline& operator=(const ProjectPipeline&) = delete;

		void bind(VkCommandBuffer commandBuffer);


		static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

	private:
		static std::vector<char> readFile(const std::string& filepath);

		void createGraphicsPipeline
		(
			const std::string& vertFilepath, 
			const std::string& fragFilepathconst, 
			const PipelineConfigInfo& configInfo
		);
		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		ProjectDevice& m_device;
		VkPipeline m_graphicsPipeline;
		VkShaderModule m_vertShaderModule;
		VkShaderModule m_fragShaderModule;
	};

}

