#include "ProjectGameObject.h"

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
}
