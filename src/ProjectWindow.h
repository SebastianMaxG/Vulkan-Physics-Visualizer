#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
namespace lsmf
{
	class ProjectWindow
	{
	public:
		ProjectWindow(int w, int h, std::string name);
		~ProjectWindow();

		ProjectWindow(const ProjectWindow&) = delete;
		ProjectWindow& operator=(const ProjectWindow&) = delete;

		void run();
		bool shouldClose() { return glfwWindowShouldClose(window);}
		VkExtent2D getExtent() { return { static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height) }; }
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		bool wasWindowResized() { return m_resized; }
		void resetWindowResizedFlag() { m_resized = false; }
		GLFWwindow* getWindow() { return window; }

	private:
		static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		
		void initWindow();

		int m_width = 800;
		int m_height = 600;
		bool m_resized = false;

		std::string m_windowName = "Vulkan";
		GLFWwindow* window = nullptr;/*
		void init();
		void loop();
		void cleanup();*/
	};
}