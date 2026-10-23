#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<QueueIndex.h>
#include<DeviceContext.h>
#include<vector>
#include<MR_Global.h>

namespace MiniRendering {

	class Command {
	public:
		Command() = default;
		~Command();
		Command(const Command& other) = delete;
		Command& operator=(const Command& other) = delete;
		std::vector<vk::raii::CommandBuffer>& getCommandBuffers();
		void createCommandPool(const DeviceContext& deviceContext, const QueueIndex& queueIndex);
		void createCommandBuffers(const vk::raii::Device& device);
	private:
		void createGraphicsCommandPool(const vk::raii::Device& device,uint32_t graphicsIndex);
		void createTransferCommandPool(const vk::raii::Device& device, uint32_t transferIndex);
		void createComputeCommandPool(const vk::raii::Device& device, uint32_t computeIndex);
		vk::raii::CommandPool m_graphicsCommandPool{ nullptr };
		vk::raii::CommandPool m_transferCommandPool{ nullptr };
		vk::raii::CommandPool m_computeCommandPool{ nullptr };
		std::vector<vk::raii::CommandBuffer>m_CommandBuffers;
	};
}