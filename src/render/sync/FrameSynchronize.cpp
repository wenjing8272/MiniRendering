#include"render/sync/FrameSynchronize.h"
#include"utils/Logger.h"
#include"render/swapchain/SwapChain.h"

namespace MiniRendering {
	FrameSynchronize::~FrameSynchronize()
	{
		MR_LOG_INFO("destory frame synchronize");
	}
	void FrameSynchronize::init(const vk::raii::Device& device,const vk::raii::Queue& graphicsQueue)
	{
		m_device = &device;
		m_graphicsQueue = &graphicsQueue;
		vk::SemaphoreTypeCreateInfo semaphoreTypeInfo{
			.semaphoreType = vk::SemaphoreType::eTimeline,
			.initialValue = 0
		};
		vk::SemaphoreCreateInfo timelineSemaphoreInfo{ .pNext = &semaphoreTypeInfo };
		m_graphicsSemaphore = vk::raii::Semaphore(device, timelineSemaphoreInfo);
		for (size_t i = 0;i < MAX_FRAME;++i)
		{
			m_slots[i].acquire = vk::raii::Semaphore(device, vk::SemaphoreCreateInfo{});
		}
	}
	bool FrameSynchronize::waitTimeline(std::uint64_t value)
	{
		const std::uint64_t v = value;
		vk::SemaphoreWaitInfo waitInfo{
			.semaphoreCount = 1,
			.pSemaphores = &*m_graphicsSemaphore,
			.pValues = &v
		};
		const auto result = m_device->waitSemaphores(waitInfo,1'000'000'000ULL);
		return result == vk::Result::eSuccess;
	}
	std::optional<FrameResource> FrameSynchronize::beginFrame(const SwapChain& swapChain)
	{
		auto& slot = m_slots[m_frameIndex];
		if (slot.retireValue)
		{
			if (!waitTimeline(slot.retireValue))
			{
				MR_LOG_FATAL("slot {} retire timeout at {}", m_frameIndex, slot.retireValue);
			}
		}
		auto [result, imageIndex] = swapChain.getSwapChain().acquireNextImage(1'000'000'000ULL, slot.acquire, nullptr);
		if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)return std::nullopt;
		slot.retireValue = ++m_timelineValue;
		return FrameResource{
			.frameIndex = m_frameIndex,
			.imageIndex = imageIndex,
			.signalValue = m_timelineValue,
			.acquireSemaphore = *slot.acquire,
			.presentSemaphore = *swapChain.getRenderingFinishedSemaphores()[imageIndex],
		};
	}
	bool FrameSynchronize::endFrame(const FrameResource& frame,const SwapChain& swapChain)
	{
		vk::SemaphoreSubmitInfo waitInfo{
			.semaphore = frame.acquireSemaphore,
			.stageMask = vk::PipelineStageFlagBits2::eColorAttachmentOutput
		};
		vk::CommandBufferSubmitInfo commandInfo{
			.commandBuffer = frame.command
		};
		vk::SemaphoreSubmitInfo signalTimeline{
			.semaphore = *m_graphicsSemaphore,
			.value = frame.signalValue,
			.stageMask = vk::PipelineStageFlagBits2::eAllCommands
		};
		vk::SemaphoreSubmitInfo signalPresent{
			.semaphore = frame.presentSemaphore,
			.stageMask = vk::PipelineStageFlagBits2::eAllCommands
		};
		std::array<vk::SemaphoreSubmitInfo, 2>signalInfo{ {signalTimeline,signalPresent} };
		vk::SubmitInfo2 submitInfo{
			.waitSemaphoreInfoCount = 1,
			.pWaitSemaphoreInfos = &waitInfo,
			.commandBufferInfoCount = 1,
			.pCommandBufferInfos = &commandInfo,
			.signalSemaphoreInfoCount = static_cast<uint32_t>(signalInfo.size()),
			.pSignalSemaphoreInfos = signalInfo.data()
		};
		m_graphicsQueue->submit2(submitInfo);
		vk::PresentInfoKHR presentInfo{
			.waitSemaphoreCount = 1,
			.pWaitSemaphores = &frame.presentSemaphore,
			.swapchainCount = 1,
			.pSwapchains = &*swapChain.getSwapChain(),
			.pImageIndices = &frame.imageIndex
		};
		const auto result = m_graphicsQueue->presentKHR(presentInfo);
		m_frameIndex = (m_frameIndex + 1) % MAX_FRAME;
		return result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR;
	}
}