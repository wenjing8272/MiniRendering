#include"WindowSystem.h"
#include<Logger.h>

namespace MiniRendering {
	Window::~Window()
	{
		MR_LOG_INFO("destroy window");
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
		return std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);
	}
	vk::Extent2D Window::getFramebufferExtent()const
	{
		int width, height;
		glfwGetFramebufferSize(m_window, &width, &height);
		return { static_cast<uint32_t>(width),static_cast<uint32_t>(height) };
	}
	void Window::createSurface(const vk::raii::Instance& instance)
	{
		VkSurfaceKHR _surface;
		if (glfwCreateWindowSurface(*instance, m_window, nullptr, &_surface) != VK_SUCCESS)
		{
			MR_LOG_FATAL("failed to create surface");
		}
		m_surface = vk::raii::SurfaceKHR(instance, _surface);
	}
	void Window::createWindow()
	{
		if (!glfwInit())
		{
			MR_LOG_FATAL("failed to init glfw");
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		m_window = glfwCreateWindow(m_width, m_height, "MiniRendering", nullptr, nullptr);
		if (!m_window)
		{
			MR_LOG_FATAL("failed to create glfw window");
		}
		glfwSetWindowUserPointer(m_window, this);
		glfwSetFramebufferSizeCallback(m_window, framebufferResizedCallback);
	}
}