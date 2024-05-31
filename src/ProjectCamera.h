#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace lsmf
{
	class ProjectCamera
	{
	public:
		ProjectCamera() = default;
		~ProjectCamera() = default;

		ProjectCamera(const ProjectCamera&) = delete;
		ProjectCamera& operator=(const ProjectCamera&) = delete;
		ProjectCamera(ProjectCamera&&) = default;
		ProjectCamera& operator=(ProjectCamera&&) = default;

		void SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far);
		void SetPerspectiveProjection(float fov, float aspectRatio, float near, float far);

		void SetViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.-1.0});
		void SetViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.-1.0});
		void SetViewXYZ(glm::vec3 position, glm::vec3 rotation);
		//void SetPosition(float x, float y, float z);
		//void SetRotation(float x, float y, float z);

		//glm::vec3 GetPosition() const;
		//glm::vec3 GetRotation() const;

		glm::mat4 GetViewMatrix() const { return m_ViewMatrix;  }
		glm::mat4 GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

	};
}
