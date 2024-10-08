#pragma once
#include "ProjectModel.h"
#include <memory>

#include <glm/gtc/matrix_transform.hpp>


#include "BulletDynamics/Dynamics/btRigidBody.h"

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
		
		static ProjectGameObject createGameObject(btRigidBody* rigidBody);

		~ProjectGameObject();

		ProjectGameObject(const ProjectGameObject&) = delete;
		ProjectGameObject& operator=(const ProjectGameObject&) = delete;
		ProjectGameObject(ProjectGameObject&&) = default;
		ProjectGameObject& operator=(ProjectGameObject&&) = default;

		id_t getId() const { return m_id; }
		void Update();

		std::shared_ptr<ProjectModel> m_Model{};
		glm::vec3 m_Color{ 1.f, 1.f, 1.f };
		TransformComponent m_Transform{};


		btRigidBody* getRigidBody() const { return m_RigidBody; }

		bool isStatic() const { return m_IsStatic; }
		bool hasRigidBody() const { return m_HasRigidBody; }

			std::string m_Name{"Object"};
	private:
			ProjectGameObject(id_t id) : m_id{ id } {}
			ProjectGameObject(id_t id, btRigidBody* rigidBody);
			id_t m_id;

			btRigidBody* m_RigidBody = nullptr;
			bool m_HasRigidBody{ false };
			bool m_IsStatic{ false };


	};
}
