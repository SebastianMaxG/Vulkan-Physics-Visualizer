#include "ProjectWindow.h"
#include <stdexcept>

namespace lsmf
{
	ProjectWindow::ProjectWindow(int w, int h, std::string name)
		: m_width(w), m_height(h), m_windowName(name)
	{
		initWindow();
	}

	ProjectWindow::~ProjectWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void ProjectWindow::run()
	{
	}

	void ProjectWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void ProjectWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto pWindow = static_cast<ProjectWindow*>(glfwGetWindowUserPointer(window));
		pWindow->m_resized = true;
		pWindow->m_width = width;
		pWindow->m_height = height;
	}

	void ProjectWindow::initWindow()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

	}
}
