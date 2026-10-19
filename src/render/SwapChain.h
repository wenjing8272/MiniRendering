#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<vector>
#include<GLFW/glfw3.h>

namespace MiniRendering {

	class SwapChain {
	public:
		SwapChain() = default;
		~SwapChain();
		SwapChain(const SwapChain& other) = delete;
		SwapChain& operator=(const SwapChain& other) = delete;
		void createSwapChain(const vk::raii::PhysicalDevice& physicalDevice,const vk::raii::Device& device,const vk::raii::SurfaceKHR& surface, GLFWwindow* window);
	private:
		vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities,GLFWwindow* window);
		uint32_t swapMinImageCount(const vk::SurfaceCapabilitiesKHR& capabilities);
		vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availableMode);
		vk::raii::SwapchainKHR m_swapChain{ nullptr };
		vk::Extent2D m_swapChainExtent;
		vk::SurfaceFormatKHR m_swapChainSurfaceFormat;
		std::vector<vk::Image> m_swapChainImages;
		std::vector<vk::raii::ImageView> m_swapChainImageViews;
	};
}