#include"SwapChain.h"
#include<Logger.h>

namespace MiniRendering {
	SwapChain::~SwapChain()
	{
		MR_LOG_INFO("destory swapchain");
	}
	vk::Extent2D SwapChain::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities,GLFWwindow* window)
	{
		if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return capabilities.currentExtent;
		}
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		return {
			std::clamp<uint32_t>(width,capabilities.minImageExtent.width,capabilities.maxImageExtent.width),
			std::clamp<uint32_t>(height,capabilities.minImageExtent.height,capabilities.maxImageExtent.height) };
	}
	uint32_t SwapChain::swapMinImageCount(const vk::SurfaceCapabilitiesKHR& capabilities)
	{
		auto minImageCount = std::max(3u, capabilities.minImageCount);
		if ((capabilities.maxImageCount > 0) && (minImageCount > capabilities.maxImageCount))//0表示无上限，在有上限，且超过maxImageCount时，把minImageCount clamp到maxImageCount
		{
			minImageCount = capabilities.maxImageCount;
		}
		return minImageCount;
	}
	vk::SurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats)
	{
		if (availableFormats.empty())
		{
			MR_LOG_FATAL("getSurfaceFormatsKHR returned an empty list");
		}
		const auto formatIt = std::ranges::find_if(availableFormats, [](const auto& format)
			{
				return format.format == vk::Format::eR8G8B8A8Srgb && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
			});
		return formatIt != availableFormats.end() ? *formatIt : availableFormats[0];//一定非空
	}
	vk::PresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<vk::PresentModeKHR>& availableModes)
	{
		if (!std::ranges::any_of(availableModes, [](auto presentMode) { return presentMode == vk::PresentModeKHR::eFifo; }))
		{
			MR_LOG_FATAL("availablePresentModes no hava eFifo mode");                 //保证后续一定可以选择使用垂直同步
		}
		return std::ranges::any_of(availableModes,
			[](const vk::PresentModeKHR value) { return vk::PresentModeKHR::eMailbox == value; }) ?   //如果找到eMailbox就使用eMailbox，否则使用eFifo
			vk::PresentModeKHR::eMailbox :
			vk::PresentModeKHR::eFifo;
	}
	void SwapChain::createSwapChain(const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::Device& device, const vk::raii::SurfaceKHR& surface,GLFWwindow* window)
	{
		MR_LOG_INFO("create swap chain");
		auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(*surface);
		m_swapChainExtent = chooseSwapExtent(surfaceCapabilities,window);                    //选择适合的交换链大小
		auto minImageCount = swapMinImageCount(surfaceCapabilities);                   

		auto availableFormats = physicalDevice.getSurfaceFormatsKHR(*surface);   
		m_swapChainSurfaceFormat = chooseSwapSurfaceFormat(availableFormats);       //选择合适的交换链格式

		auto availablePresentModes = physicalDevice.getSurfacePresentModesKHR(*surface);
		auto presentMode = chooseSwapPresentMode(availablePresentModes);                  //选择合适的显示模式
		vk::SwapchainCreateInfoKHR swapChainCreateInfo{
			.surface = *surface,
			.minImageCount = minImageCount,
			.imageFormat = m_swapChainSurfaceFormat.format,
			.imageColorSpace = m_swapChainSurfaceFormat.colorSpace,
			.imageExtent = m_swapChainExtent,
			.imageArrayLayers = 1,
			.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
			.imageSharingMode = vk::SharingMode::eExclusive,
			.preTransform = surfaceCapabilities.currentTransform,
			.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
			.presentMode = presentMode,
			.clipped = true
		};
		m_swapChain = vk::raii::SwapchainKHR(device, swapChainCreateInfo);
		m_swapChainImages = m_swapChain.getImages();  //取出交换链的图片
	}
}