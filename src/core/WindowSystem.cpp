#include"WindowSystem.h"
#include<Logger.h>

namespace MiniRendering {
	Window::~Window()
	{
		MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::info, "destroy window");
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	void Window::windowInit(int width, int height)
	{
		m_width = width;
		m_height = height;
		createWindow();
	}
	GLFWwindow* Window::getWindow() const
	{
		return m_window;
	}
	vk::raii::SurfaceKHR& Window::getSurface()
	{
		return m_surface;
	}
	std::vector<const char*> Window::getGLFWExtension() const
	{
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		return std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);;
	}
	void Window::createSurface(const vk::raii::Instance& instance)
	{
		VkSurfaceKHR _surface;
		if (glfwCreateWindowSurface(*instance, m_window, nullptr, &_surface) != VK_SUCCESS)
		{
			MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "failed to create surface");
		}
		m_surface = vk::raii::SurfaceKHR(instance, _surface);
	}
	void Window::createWindow()
	{
		if (!glfwInit())
		{
			MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "failed to init glfw");
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		m_window = glfwCreateWindow(m_width, m_height, "MiniRendering", nullptr, nullptr);
		if (!m_window)
		{
			MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "failed to create glfw window");
		}
	}
}