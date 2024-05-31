#include "ProjectModel.h"
#include <cassert>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

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
			std::cout << warn << err << std::endl;

			throw std::runtime_error(warn + err);
		}

		vertices.clear();
		indices.clear();

		for (const auto& shape : shapes)
		{
			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};
				if (index.vertex_index >= 0)
				vertex.position =
				{
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

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

				auto colorIndex = 3 * index.vertex_index;
				if (colorIndex < attrib.colors.size())
				{
					vertex.color =
					{
						attrib.colors[colorIndex + 0],
						attrib.colors[colorIndex + 1],
						attrib.colors[colorIndex + 2]
					};
				}
				else
				{
					vertex.color = { 1.0f, 1.0f, 1.0f };
				}

				vertices.push_back(vertex);
				indices.push_back(static_cast<uint32_t>(indices.size()));
			}
		}
		std::cout << "Model loaded. Vertices: " << vertices.size() << " Indices: " << indices.size() << std::endl;
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
		std::cout << "Model loaded from file: " << filepath << std::endl;
		std::cout << "Model has " << builder.vertices.size() << " vertices\n";
		std::cout << "Model has " << builder.indices.size() << " indices\n";

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
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, position);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, color);
		return  attributeDescriptions;
	}

}
