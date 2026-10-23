#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<vector>
#include<DeviceContext.h>
#include<MR_Global.h>

namespace MiniRendering {

	class SwapChain {
	public:
		SwapChain() = default;
		~SwapChain();
		SwapChain(const SwapChain& other) = delete;
		SwapChain& operator=(const SwapChain& other) = delete;
		vk::Extent2D& getSwapChainExtent();
		vk::SurfaceFormatKHR& getSwapChainSurfaceFormat();
		std::vector<vk::Image>& getSwapChainImages();
		std::vector<vk::raii::ImageView>& getSwapChainImageViews();
		std::vector<vk::raii::Semaphore>& getRenderingFinishedSemaphores();
		std::vector<vk::raii::Semaphore>& getPresentCompleteSemaphores();
		void createSwapChain(const DeviceContext& deviceContext,const vk::raii::SurfaceKHR& surface, vk::Extent2D extent);
		void createRenderingAndPresentSemaphore(const vk::raii::Device& device);
	private:
		vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities,vk::Extent2D extent);
		uint32_t swapMinImageCount(const vk::SurfaceCapabilitiesKHR& capabilities);
		vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats);
		vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availableMode);
		void createSwapChainViews(const vk::raii::Device& device);
		vk::raii::SwapchainKHR m_swapChain{ nullptr };
		vk::Extent2D m_swapChainExtent;
		vk::SurfaceFormatKHR m_swapChainSurfaceFormat;
		std::vector<vk::Image> m_swapChainImages;
		std::vector<vk::raii::ImageView> m_swapChainImageViews;
		std::vector<vk::raii::Semaphore> m_presentCompleteSemaphores;
		std::vector<vk::raii::Semaphore> m_renderingFinishedSemaphores;
	};
}