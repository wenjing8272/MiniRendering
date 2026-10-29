#pragma once
#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#include<vulkan/vulkan_raii.hpp>
#include<vector>

namespace MiniRendering {
	class Window
	{
	public:
		Window() = default;
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		GLFWwindow* getWindow() const;
		vk::raii::SurfaceKHR& getSurface();
		void windowInit(int width, int height);
		void createSurface(const vk::raii::Instance& instance);
		std::vector<const char*> getGLFWExtension() const;
		vk::Extent2D getFramebufferExtent();
		void waitEvents()const;
		void closeWindowEvent();
		bool windowShouldClose()const;
		void pollEvent();
		bool consumeFrameResize();
	private:
		void createWindow();
		static void framebufferResizedCallback(GLFWwindow* window, int width, int height)
		{
			auto app = static_cast<Window*>(glfwGetWindowUserPointer(window));
			if (app != nullptr)app->framebufferResized = true;
		}
		GLFWwindow* m_window{ nullptr };
		bool m_windowInit = false;
		int m_width{ 0 };
		int m_height{ 0 };
		vk::raii::SurfaceKHR m_surface{ nullptr };
		bool framebufferResized = false;
	};
}