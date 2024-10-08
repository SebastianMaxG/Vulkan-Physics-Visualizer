#pragma once
#include "ProjectDevice.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include <btBulletDynamicsCommon.h>


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

			bool operator==(const Vertex& other) const
			{
				return position == other.position
					&& color == other.color
					&& normal == other.normal
					&& texCoord == other.texCoord;
			}
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
		static btRigidBody* CreatePhysicsFromFile(const std::string& filepath, btDiscreteDynamicsWorld* world, bool isStatic);

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);

	private:
		void CreateVertexBuffers(const std::vector<Vertex>& vertices);
		void CreateIndexBuffers(const std::vector<uint32_t>& indices);

		static bool IsMeshConcave(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);

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
