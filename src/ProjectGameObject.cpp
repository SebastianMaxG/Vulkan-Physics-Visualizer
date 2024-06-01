#include "ProjectGameObject.h"

#include "LinearMath/btTransform.h"

namespace lsmf
{
	glm::mat4 TransformComponent::mat4() const
	{
		auto transform = glm::translate(glm::mat4{ 1.f }, translation);
		transform = glm::rotate(transform, rotation.y, { 0.f, 1.f, 0.f });
		transform = glm::rotate(transform, rotation.x, { 1.f, 0.f, 0.f });
		transform = glm::rotate(transform, rotation.z, { 0.f, 0.f, 1.f });
		transform = glm::scale(transform, scale);
		// set the transform matrix to the calculated matrix
		 m_TransformMatrix = transform;
		return transform;
	}
	glm::mat3 TransformComponent::normalMatrix() const
	{
		return glm::mat3(glm::transpose(glm::inverse(m_TransformMatrix)));
	}

	ProjectGameObject ProjectGameObject::createGameObject(btRigidBody* rigidBody)
	{
		static id_t currentId = 0;
		return ProjectGameObject{ currentId++ , rigidBody};
	}

	ProjectGameObject::~ProjectGameObject()
	{
	}

	void ProjectGameObject::Update()
	{
		if (!m_HasRigidBody)
		{
			return;
		}


		btTransform transform = m_RigidBody->getWorldTransform();

		// set the transform matrix to the calculated matrix
		m_Transform.translation = { transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ() };
		transform.getRotation().getEulerZYX(m_Transform.rotation.z, m_Transform.rotation.y, m_Transform.rotation.x);
		

	}

	ProjectGameObject::ProjectGameObject(id_t id, btRigidBody* rigidBody)
		: m_id{ id }, m_RigidBody{ rigidBody }
	{
		btTransform transform = m_RigidBody->getWorldTransform();
		if (m_RigidBody->isStaticObject())
		{
			m_IsStatic = true;
		}
		// set the transform matrix to the calculated matrix
		m_Transform.translation = { transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ() };
		transform.getRotation().getEulerZYX(m_Transform.rotation.z, m_Transform.rotation.y, m_Transform.rotation.x);
		m_HasRigidBody = true;
	}
}
