#include "ProjectModel.h"
#include "ProjectUtils.h"
#include <cassert>
#include <unordered_map>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std
{
	template<>
	struct hash<lsmf::ProjectModel::Vertex>
	{
		size_t operator()(lsmf::ProjectModel::Vertex const& vertex) const
		{
			size_t seed = 0;
			lsmf::hash_combine(seed, vertex.position, vertex.color, vertex.normal, vertex.texCoord);
			return seed;
		}
	};
	
}

namespace lsmf
{
	void ProjectModel::Builder::LoadModel(const std::string& filepath)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn;
		std::string err;

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
		{
			throw std::runtime_error(warn + err);
		}

		vertices.clear();
		indices.clear();

		std::unordered_map<Vertex, uint32_t> uniqueVertices{}; // Map of unique vertices
		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};
				if (index.vertex_index >= 0)
				{
					vertex.position =
					{
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2]
					};

					vertex.color =
					{
						attrib.colors[3 * index.vertex_index + 0],
						attrib.colors[3 * index.vertex_index + 1],
						attrib.colors[3 * index.vertex_index + 2]
					};
				}
				

				if (index.normal_index >= 0)
					vertex.normal =
				{
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};

				if (index.texcoord_index >= 0)
				vertex.texCoord =
				{
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
				};

				

				//vertices.push_back(vertex);
				if (uniqueVertices.count(vertex) == 0)
				{
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}
				indices.push_back(uniqueVertices[vertex]);
			}
		}
	}

	ProjectModel::ProjectModel(ProjectDevice& device, const std::vector<Vertex>& vertices)
		: m_device(device)
	{
		CreateVertexBuffers(vertices);
	}

	ProjectModel::ProjectModel(ProjectDevice& device, const Builder& builder)
		: m_device(device)
	{
		CreateVertexBuffers(builder.vertices);
		CreateIndexBuffers(builder.indices);
	}

	ProjectModel::~ProjectModel()
	{
		vkDestroyBuffer(m_device.device(), m_vertexBuffer, nullptr);
		vkFreeMemory(m_device.device(), m_vertexBufferMemory, nullptr);

		if (hasIndexBuffer)
		{
			vkDestroyBuffer(m_device.device(), m_indexBuffer, nullptr);
			vkFreeMemory(m_device.device(), m_indexBufferMemory, nullptr);
		}
	}

	std::unique_ptr<ProjectModel> ProjectModel::CreateModelFromFile(ProjectDevice& device, const std::string& filepath)
	{
		Builder builder;
		builder.LoadModel(filepath);

		return std::make_unique<ProjectModel>(device, builder);
	}

	void ProjectModel::Bind(VkCommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { m_vertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

		if (hasIndexBuffer)
		{
			vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
		}
	}

	void ProjectModel::Draw(VkCommandBuffer commandBuffer)
	{
		if (hasIndexBuffer)
		{
			vkCmdDrawIndexed(commandBuffer, m_indexCount, 1, 0, 0, 0);
		}
		else
		{
			vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
		}
	}

	void ProjectModel::CreateVertexBuffers(const std::vector<Vertex>& vertices)
	{
		m_vertexCount = static_cast<uint32_t>(vertices.size());
		assert(m_vertexCount >= 3 && "Vertex count must be at least 3"); // We need at least 3 vertices to draw a triangle
		VkDeviceSize bufferSize = sizeof(vertices[0]) * m_vertexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		m_device.createBuffer
		(
			bufferSize, 
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* data;
		vkMapMemory(m_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_device.device(), stagingBufferMemory);


		m_device.createBuffer
		(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_vertexBuffer,
			m_vertexBufferMemory
		);
		m_device.copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

		vkDestroyBuffer(m_device.device(), stagingBuffer, nullptr);
		vkFreeMemory(m_device.device(), stagingBufferMemory, nullptr);
	}

	void ProjectModel::CreateIndexBuffers(const std::vector<uint32_t>& indices)
	{
		m_indexCount = static_cast<uint32_t>(indices.size());
		hasIndexBuffer = m_indexCount > 0;

		if (!hasIndexBuffer) { return; }

		VkDeviceSize bufferSize = sizeof(indices[0]) * m_indexCount;

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;

		m_device.createBuffer
		(
			bufferSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* data;
		vkMapMemory(m_device.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
		vkUnmapMemory(m_device.device(), stagingBufferMemory);


		m_device.createBuffer
		(
			bufferSize,
			VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_indexBuffer,
			m_indexBufferMemory
		);
		m_device.copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

		vkDestroyBuffer(m_device.device(), stagingBuffer, nullptr);
		vkFreeMemory(m_device.device(), stagingBufferMemory, nullptr);
	}

	std::vector<VkVertexInputBindingDescription> ProjectModel::Vertex::GetBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
		bindingDescriptions[0].binding = 0;
		bindingDescriptions[0].stride = sizeof(Vertex);
		bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		return bindingDescriptions;
	}

	std::vector<VkVertexInputAttributeDescription> ProjectModel::Vertex::GetAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

		attributeDescriptions.push_back({ 0,0,VK_FORMAT_R32G32B32_SFLOAT ,offsetof(Vertex, position) });
		attributeDescriptions.push_back({ 1,0,VK_FORMAT_R32G32B32_SFLOAT ,offsetof(Vertex, color) });
		attributeDescriptions.push_back({ 2,0,VK_FORMAT_R32G32B32_SFLOAT ,offsetof(Vertex, normal) });
		attributeDescriptions.push_back({ 3,0,VK_FORMAT_R32G32_SFLOAT	 ,offsetof(Vertex, texCoord) });
		return  attributeDescriptions;
	}

}
