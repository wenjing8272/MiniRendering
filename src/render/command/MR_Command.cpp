#include"render/command/MR_Command.h"
#include"utils/Logger.h"

namespace MiniRendering {
	Command::~Command()
	{
		MR_LOG_INFO("destory command");
	}
	const std::vector<vk::raii::CommandBuffer>& Command::getCommandBuffers() const{ return m_CommandBuffers; }
	void Command::createCommandPool(const DeviceContext& deviceContext,const QueueIndex& queueIndex)
	{
		createGraphicsCommandPool(deviceContext.device, queueIndex.graphicsAndPresent->familyIndex);
		createTransferCommandPool(deviceContext.device, queueIndex.transfer->familyIndex);
		createComputeCommandPool(deviceContext.device, queueIndex.compute->familyIndex);
	}
	void Command::createGraphicsCommandPool(const vk::raii::Device& device, uint32_t graphicsIndex)
	{
		vk::CommandPoolCreateInfo graphicsPoolInfo{
			.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			.queueFamilyIndex = graphicsIndex
		};
		m_graphicsCommandPool = vk::raii::CommandPool(device, graphicsPoolInfo);
	}
	void Command::createCommandBuffers(const vk::raii::Device& device)
	{
		vk::CommandBufferAllocateInfo commandAllocInfo{
			.commandPool = m_graphicsCommandPool,
			.level = vk::CommandBufferLevel::ePrimary,
			.commandBufferCount = MAX_FRAME
		};
		m_CommandBuffers = vk::raii::CommandBuffers(device, commandAllocInfo);
	}
	void Command::createTransferCommandPool(const vk::raii::Device& device, uint32_t transferIndex)
	{
		vk::CommandPoolCreateInfo transferPoolInfo{
			.flags = vk::CommandPoolCreateFlagBits::eTransient,
			.queueFamilyIndex = transferIndex
		};
		m_transferCommandPool = vk::raii::CommandPool(device, transferPoolInfo);
	}
	void Command::createComputeCommandPool(const vk::raii::Device& device, uint32_t computeIndex)
	{
		vk::CommandPoolCreateInfo computePoolInfo{
			.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			.queueFamilyIndex = computeIndex
		};
		m_computeCommandPool = vk::raii::CommandPool(device, computePoolInfo);
	}
}