#pragma once
#include "ProjectModel.h"
#include <memory>

namespace lsmf 
{
	struct Transform2DComponent
	{
		glm::vec2 translation{};
		glm::vec2 scale{ 1.f, 1.f };
		float rotation{ 0.f };

		glm::mat2 mat2() const
		{
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			const glm::mat2 rotationMat = glm::mat2({ c, s }, { -s, c });
			const glm::mat2 scaleMat = glm::mat2({ scale.x,0 }, { 0, scale.y });
			return rotationMat * scaleMat;
		}
	};


	class ProjectGameObject
	{
	public:
		using id_t = unsigned int;

		static ProjectGameObject createGameObject()
		{
			static id_t currentId = 0;
			return ProjectGameObject{ currentId++ };
		}

		ProjectGameObject(const ProjectGameObject&) = delete;
		ProjectGameObject& operator=(const ProjectGameObject&) = delete;
		ProjectGameObject(ProjectGameObject&&) = default;
		ProjectGameObject& operator=(ProjectGameObject&&) = default;

		id_t getId() const { return m_id; }

		std::shared_ptr<ProjectModel> m_Model{};
		glm::vec3 m_Color{ 1.f, 1.f, 1.f };
		Transform2DComponent m_Transform2D{};

	private:
			ProjectGameObject(id_t id) : m_id{ id } {}
			id_t m_id;
	};
}