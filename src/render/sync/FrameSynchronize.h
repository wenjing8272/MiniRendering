#pragma once
#include"render/sync/FrameResource.h"
#include"render/RenderConfig.h"
#include<cstdint>
#include<vulkan/vulkan_raii.hpp>
#include<array>
#include<utility>
#include<optional>

namespace MiniRendering {
	class SwapChain;
		
	class FrameSynchronize {
	public:
		FrameSynchronize() = default;
		~FrameSynchronize();
		FrameSynchronize(const FrameSynchronize& other) = delete;
		FrameSynchronize& operator=(const FrameSynchronize& other) = delete;
		void init(const vk::raii::Device& device,const vk::raii::Queue& graphicsQueue);
		std::optional<FrameResource> beginFrame(const SwapChain& swapChain);
		bool endFrame(const FrameResource& frame,const SwapChain& swapChain);
	private:
		bool waitTimeline(std::uint64_t value);
		struct Slot {
			std::uint64_t retireValue = 0;
			vk::raii::Semaphore acquire{ nullptr };
		};
		const vk::raii::Device* m_device = nullptr;
		const vk::raii::Queue* m_graphicsQueue = nullptr;
		vk::raii::Semaphore m_graphicsSemaphore{ nullptr };
		std::array<Slot, MAX_FRAME> m_slots;
		std::uint64_t m_timelineValue = 0;
		std::uint32_t m_frameIndex = 0;
	};
}