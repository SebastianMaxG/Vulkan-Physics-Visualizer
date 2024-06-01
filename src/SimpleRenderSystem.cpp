#include "SimpleRenderSystem.h"
#include <stdexcept>
#include <array>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lsmf
{
	struct SimplePushConstantData
	{
		glm::mat4 transform{ 1.f };
		glm::mat4 normalMatrix{ 1.f };
	};

	SimpleRenderSystem::SimpleRenderSystem(ProjectDevice& device, VkRenderPass renderPass)
		: m_device{ device }
	{
		createPipelineLayout();
		createPipeline(renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
	}

	void SimpleRenderSystem::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create pipeline layout!");
		}

	}
	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		ProjectPipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = m_pipelineLayout;
		m_pipeline = std::make_unique<ProjectPipeline>
			(
				m_device,
				"shaders/simple_shader.vert.spv",
				"shaders/simple_shader.frag.spv",
				pipelineConfig
			);

	}


	void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<ProjectGameObject>& gameObjects, const ProjectCamera& camera)
	{
		m_pipeline->bind(commandBuffer);

		auto projectionView = camera.GetProjectionMatrix() * camera.GetViewMatrix();

		for (const auto& obj : gameObjects)
		{
			//obj.m_Transform.rotation.y = glm::mod(obj.m_Transform.rotation.y + 0.01f, glm::two_pi<float>());
			//obj.m_Transform.rotation.x = glm::mod(obj.m_Transform.rotation.x + 0.005f, glm::two_pi<float>());

			SimplePushConstantData push{};
			auto modelMatrix = obj.m_Transform.mat4();
			push.transform = projectionView * modelMatrix;
			push.normalMatrix = obj.m_Transform.normalMatrix();

			vkCmdPushConstants
			(
				commandBuffer,
				m_pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push
			);
			obj.m_Model->Bind(commandBuffer);
			obj.m_Model->Draw(commandBuffer);

			//Draw Physics Vectors
			if (obj.hasRigidBody() && !obj.isStatic() )
			{
				auto rigidBody = obj.getRigidBody();

				auto linearVelocity = rigidBody->getLinearVelocity();
				//draw verctor for linear velocity
				float linearScale = linearVelocity.length();
				if (linearScale > 0.05f)
				{
					TransformComponent linearTransform;
					linearTransform.translation = obj.m_Transform.translation;
					linearTransform.scale = glm::vec3{ 1.f, linearScale,1.f };
					glm::vec3 directionGLM(linearVelocity.x(), linearVelocity.y(), linearVelocity.z());

					// Normalize the direction vector
					directionGLM = glm::normalize(directionGLM);

					// Calculate the rotation angles in radians
					float pitch = glm::acos(directionGLM.y);
					float yaw = glm::atan(directionGLM.x, directionGLM.z);

					// Convert the rotation angles to degrees
					linearTransform.rotation.x = pitch;
					linearTransform.rotation.y = yaw;



					auto linearMatrix = linearTransform.mat4();
					push.transform = projectionView * linearMatrix;
					push.normalMatrix = obj.m_Transform.normalMatrix();


					vkCmdPushConstants
					(
						commandBuffer,
						m_pipelineLayout,
						VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
						0,
						sizeof(SimplePushConstantData),
						&push
					);

					m_VelocityVector->Bind(commandBuffer);
					m_VelocityVector->Draw(commandBuffer);
				}
				auto impulse = rigidBody->getPushVelocity();
				float impulseScale = impulse.length();
				if (impulseScale > 0.05f)
				{
					TransformComponent impulseTransform;
					impulseTransform.translation = obj.m_Transform.translation;
					impulseTransform.scale = glm::vec3{ 1.f, impulseScale,1.f };
					glm::vec3 directionGLM(impulse.x(), impulse.y(), impulse.z());

					// Normalize the direction vector
					directionGLM = glm::normalize(directionGLM);

					// Calculate the rotation angles in radians
					float pitch = glm::acos(directionGLM.y);
					float yaw = glm::atan(directionGLM.x, directionGLM.z);

					// Convert the rotation angles to degrees
					impulseTransform.rotation.x = pitch;
					impulseTransform.rotation.y = yaw;



					auto impulseMatrix = impulseTransform.mat4();
					push.transform = projectionView * impulseMatrix;
					push.normalMatrix = obj.m_Transform.normalMatrix();


					vkCmdPushConstants
					(
						commandBuffer,
						m_pipelineLayout,
						VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
						0,
						sizeof(SimplePushConstantData),
						&push
					);

					m_ExtraVector->Bind(commandBuffer);
					m_ExtraVector->Draw(commandBuffer);
				}

				auto force = rigidBody->getGravity();
				float forceScale = force.length();
				if (forceScale > 0.05f)
				{
					TransformComponent forceTransform;
					forceTransform.translation = obj.m_Transform.translation;
					forceTransform.scale = glm::vec3{ 1.f, forceScale,1.f };
					glm::vec3 directionGLM(force.x(), force.y(), force.z());

					// Normalize the direction vector
					directionGLM = glm::normalize(directionGLM);

					// Calculate the rotation angles in radians
					float pitch = glm::acos(directionGLM.y);
					float yaw = glm::atan(directionGLM.x, directionGLM.z);

					// Convert the rotation angles to degrees
					forceTransform.rotation.x = pitch;
					forceTransform.rotation.y = yaw;



					auto forceMatrix = forceTransform.mat4();
					push.transform = projectionView * forceMatrix;
					push.normalMatrix = obj.m_Transform.normalMatrix();


					vkCmdPushConstants
					(
						commandBuffer,
						m_pipelineLayout,
						VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
						0,
						sizeof(SimplePushConstantData),
						&push
					);

					m_ForceVector->Bind(commandBuffer);
					m_ForceVector->Draw(commandBuffer);
				}
			}
		}
	}
}