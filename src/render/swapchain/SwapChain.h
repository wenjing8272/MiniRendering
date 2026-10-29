#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<vector>
#include"core/DeviceContext.h"
#include"render/RenderConfig.h"

namespace MiniRendering {

	class SwapChain {
	public:
		SwapChain() = default;
		~SwapChain();
		SwapChain(const SwapChain& other) = delete;
		SwapChain& operator=(const SwapChain& other) = delete;
		const vk::raii::SwapchainKHR& getSwapChain()const;
		const vk::Extent2D& getSwapChainExtent()const;
		const vk::SurfaceFormatKHR& getSwapChainSurfaceFormat()const;
		const std::vector<vk::Image>& getSwapChainImages()const;
		const std::vector<vk::raii::ImageView>& getSwapChainImageViews()const;
		const std::vector<vk::raii::Semaphore>& getRenderingFinishedSemaphores()const;
		void createSwapChain(const DeviceContext& deviceContext,const vk::raii::SurfaceKHR& surface, vk::Extent2D extent,vk::raii::SwapchainKHR oldSwapChain = nullptr);
		void createRenderingSemaphore(const vk::raii::Device& device);
		void recreateSwapChain(const DeviceContext& deviceContext,const vk::raii::SurfaceKHR& surface,vk::Extent2D extent);
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
		std::vector<vk::raii::Semaphore> m_renderingFinishedSemaphores;
	};
}