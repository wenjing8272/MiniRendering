#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<cstdint>

namespace MiniRendering {
	struct FrameResource {
		std::uint32_t frameIndex = 0;
		std::uint32_t imageIndex = 0;
		std::uint64_t signalValue = 0;
		vk::Semaphore acquireSemaphore{};
		vk::Semaphore presentSemaphore{};
		vk::CommandBuffer command{};
	};
}