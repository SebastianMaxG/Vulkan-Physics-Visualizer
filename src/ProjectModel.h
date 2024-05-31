#pragma once
#include "ProjectDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <memory>

namespace lsmf
{

	class ProjectModel
	{
	public:
		struct Vertex
		{
			glm::vec3 position;
			glm::vec3 color;
			glm::vec3 normal;
			glm::vec2 texCoord;


			static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		};

		struct Builder
		{
			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;

			void LoadModel(const std::string &filepath);
		};


		ProjectModel(ProjectDevice& device, const std::vector<Vertex>& vertices);
		ProjectModel(ProjectDevice& device, const Builder &builder);
		~ProjectModel();

		ProjectModel(const ProjectModel&) = delete;
		ProjectModel& operator=(const ProjectModel&) = delete;

		static std::unique_ptr<ProjectModel> CreateModelFromFile(ProjectDevice& device, const std::string& filepath);

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		void CreateVertexBuffers(const std::vector<Vertex>& vertices);
		void CreateIndexBuffers(const std::vector<uint32_t>& indices);

		ProjectDevice& m_device;

		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		uint32_t m_vertexCount{ 0 };

		bool hasIndexBuffer{ false };
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;
		uint32_t m_indexCount{ 0 };
	};
}
