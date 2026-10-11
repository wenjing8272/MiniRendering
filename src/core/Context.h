#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<vector>

namespace MiniRendering {
	const std::vector<char const* >validationLayers{
	"VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
	constexpr bool enableValidationLayers = false;
#else
	constexpr bool enableValidationLayers = true;
#endif

	class Context
	{
	public:
		Context();
		Context(const Context&) = delete;
		Context& operator=(const Context&) = delete;
		Context(Context&&) noexcept;
		Context& operator=(Context&&) noexcept;
		~Context();
	private:
		void initContext();
		void createInstance();
		void pickPhysicalDevice();
		vk::raii::Context m_context;
		vk::raii::Instance m_instance{ nullptr };
#ifndef NDEBUG
		vk::raii::DebugUtilsMessengerEXT m_debugMessenger{ nullptr };
#endif
		vk::raii::PhysicalDevice m_physicalDevice{ nullptr };
	};
} // namespace MiniRendering
