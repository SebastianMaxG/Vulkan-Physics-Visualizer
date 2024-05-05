#pragma once
#include "ProjectDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace lsmf
{

	class ProjectModel
	{
	public:
		struct Vertex
		{
			glm::vec2 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		};

		ProjectModel(ProjectDevice& device, const std::vector<Vertex>& vertices);
		~ProjectModel();

		ProjectModel(const ProjectModel&) = delete;
		ProjectModel& operator=(const ProjectModel&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		void CreateVertexBuffers(const std::vector<Vertex>& vertices);

		ProjectDevice& m_device;
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		uint32_t m_vertexCount{ 0 };
	};
}
