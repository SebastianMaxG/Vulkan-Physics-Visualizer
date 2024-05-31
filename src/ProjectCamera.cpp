#include "ProjectCamera.h"
#include <cassert>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>


namespace lsmf
{
	void ProjectCamera::SetOrthographicProjection(float left, float right, float bottom, float top, float near,
		float far)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, near, far);
	}
	void ProjectCamera::SetPerspectiveProjection(float fov, float aspectRatio, float near, float far)
	{
		assert(glm::abs(aspectRatio - std::numeric_limits<float>::epsilon()) > 0.f);
		m_ProjectionMatrix = glm::perspective(fov, aspectRatio, near, far);
	}

	void ProjectCamera::SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
	{
		m_ViewMatrix = glm::lookAt(position, direction + position, up);

	}
	void ProjectCamera::SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up)
	{
		m_ViewMatrix = glm::lookAt(position, target, up);

	}
	void ProjectCamera::SetViewXYZ(glm::vec3 position, glm::vec3 rotation)
	{
		glm::mat4 transform{ 1.f };
		transform = glm::translate(transform, position);
		transform = glm::rotate(transform, rotation.y, { 0.f, 1.f, 0.f });
		transform = glm::rotate(transform, rotation.x, { 1.f, 0.f, 0.f });
		transform = glm::rotate(transform, rotation.z, { 0.f, 0.f, 1.f });
		m_ViewMatrix = glm::inverse(transform);
	}
}
