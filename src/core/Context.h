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
		Context(Context&&) noexcept;
		Context& operator=(Context&&) noexcept;
		~Context();
		void initContext(std::vector<const char*>& instanceExtensions);
		void createDevice(const MiniRendering::QueueIndex& queueIndex);
		vk::raii::Instance& getInstance();
		vk::raii::PhysicalDevice& getPhysicalDevice();
		vk::raii::Device& getDevice();
	private:
		void createInstance();
		void pickPhysicalDevice();
		bool isDeviceSuitable(const vk::raii::PhysicalDevice& physicalDevice);
		std::vector<const char*> instanceExtensions;
		vk::raii::Context m_context;
		vk::raii::Instance m_instance{ nullptr };
#ifndef NDEBUG
		vk::raii::DebugUtilsMessengerEXT m_debugMessenger{ nullptr };
#endif
		vk::raii::PhysicalDevice m_physicalDevice{ nullptr };
		vk::raii::Device m_device{ nullptr };
		vk::raii::Queue m_graphicsQueue{ nullptr };
		vk::raii::Queue m_transferQueue{ nullptr };
		vk::raii::Queue m_computeQueue{ nullptr };
		std::vector<const char*> requiredDeviceExtensions = { vk::KHRSwapchainExtensionName };
	};
} // namespace MiniRendering
