#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<vector>
#include<optional>
#include<QueueIndex.h>
#include"Validation.h"

namespace MiniRendering {

	class Context
	{
	public:
		Context() = default;
		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;
		~Context();
		void initInstance(std::vector<const char*>& instanceExtensions);
		void createDevice(const MiniRendering::QueueIndex& queueIndex);
		void pickPhysicalDevice(const vk::raii::SurfaceKHR& surface);
		vk::raii::Instance& getInstance();
		vk::raii::PhysicalDevice& getPhysicalDevice();
		vk::raii::Device& getDevice();
		vk::raii::Queue& getGraphicsQueue();
		vk::raii::Queue& getTransferQueue();
		vk::raii::Queue& getComputeQueue();
	private:
		void createInstance();
		bool isDeviceSuitable(const vk::raii::PhysicalDevice& physicalDevice, const vk::raii::SurfaceKHR& surface);
		std::vector<const char*> instanceExtensions;
		vk::raii::Context m_context;
		vk::raii::Instance m_instance{ nullptr };
#ifndef NDEBUG
		vk::raii::DebugUtilsMessengerEXT m_debugMessenger{ nullptr };
#endif
		vk::raii::PhysicalDevice m_physicalDevice{ nullptr };
		vk::raii::Device m_device{ nullptr };
		vk::raii::Queue m_graphicsQueueAndPresent{ nullptr };
		vk::raii::Queue m_transferQueue{ nullptr };
		vk::raii::Queue m_computeQueue{ nullptr };
		std::vector<const char*> requiredDeviceExtensions = { vk::KHRSwapchainExtensionName };
	};
} // namespace MiniRendering
