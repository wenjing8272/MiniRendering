#pragma once
#define GLFW_INCLUDE_VULKAN
#include<GLFW/glfw3.h>
#include<vulkan/vulkan_raii.hpp>

namespace MiniRendering {
	class Window
	{
	public:
		Window() = default;
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		Window(Window&&) noexcept;
		Window& operator=(Window&&) noexcept;

		GLFWwindow* getWindow() const;
		void windowInit(int width, int height);
		void createSurface(const vk::raii::Instance& instance);
	private:
		void createWindow();
		GLFWwindow* m_window{ nullptr };
		int m_width{ 0 };
		int m_height{ 0 };

		vk::raii::SurfaceKHR m_surface{ nullptr };
	};
}