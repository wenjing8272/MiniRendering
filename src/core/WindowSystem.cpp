#include"WindowSystem.h"
#include<Logger.h>

namespace MiniRendering {
	Window::~Window()
	{
		MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::info, "destroy window");
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	Window::Window(Window&& other) noexcept
	{
		m_window = other.m_window;
		other.m_window = nullptr;
	}
	Window& Window::operator=(Window&& other) noexcept
	{
		if (this != &other)
		{
			glfwDestroyWindow(m_window);
			m_window = other.m_window;
			other.m_window = nullptr;
		}
		return *this;
	}
	void Window::windowInit(int width, int height)
	{
		m_width = width;
		m_height = height;
		createWindow();
	}
	void Window::createWindow()
	{
		if (!glfwInit())
		{
			MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "failed to init glfw");
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		m_window = glfwCreateWindow(m_width,m_height,"MiniRendering", nullptr, nullptr);
	}

	GLFWwindow* Window::getWindow() const
	{
		return m_window;
	}
	std::vector<const char*> Window::getGLFWExtension() const
	{
		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		return std::vector<const char*>(glfwExtensions, glfwExtensions + glfwExtensionCount);;
	}
}