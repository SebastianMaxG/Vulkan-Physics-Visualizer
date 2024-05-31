#pragma once
#include "ProjectModel.h"
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

namespace lsmf 
{
	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f ,1.f};
		glm::vec3 rotation{ };

		glm::mat4 mat4() const;

		glm::mat3 normalMatrix() const;
	private:
		mutable glm::mat4 m_TransformMatrix{ 1.f };
		
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
		TransformComponent m_Transform{};

	private:
			ProjectGameObject(id_t id) : m_id{ id } {}
			id_t m_id;
	};
}