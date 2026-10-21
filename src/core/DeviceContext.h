#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<vk_mem_alloc.h>

namespace MiniRendering {
	struct DeviceContext {
		const vk::raii::PhysicalDevice& physicalDevice;
		const vk::raii::Device& device;
		const VmaAllocator allocator;
		const vk::raii::Queue& graphicsQueue;
		const vk::raii::Queue& transferQueue;
		const vk::raii::Queue& computeQueue;
	};
}