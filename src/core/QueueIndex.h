#pragma once
#include<optional>
#include<Logger.h>
#include<vulkan/vulkan_raii.hpp>

namespace MiniRendering {
	struct QueueIndex{
		std::optional<uint32_t> graphicsQueueIndex;
		std::optional<uint32_t> presentQueueIndex;
		std::optional<uint32_t> transferQueueIndex;
		std::optional<uint32_t> computeQueueIndex;

		bool isComplete() const
		{
			return graphicsQueueIndex && presentQueueIndex && transferQueueIndex && computeQueueIndex; 
		}
		static QueueIndex find(const vk::raii::PhysicalDevice& physical, const vk::raii::SurfaceKHR& surface)
		{
			QueueIndex queueIndex;
			auto queueFamilyProperties = physical.getQueueFamilyProperties();
			for (size_t i = 0;i < queueFamilyProperties.size();++i)
			{
				if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics))
				{
					queueIndex.graphicsQueueIndex = i;
					break;
				}
			}
			if (!queueIndex.graphicsQueueIndex)
			{
				MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "no find graphics");
			}
			for (size_t i = 0;i < queueFamilyProperties.size();++i)
			{
				if (physical.getSurfaceSupportKHR(i, surface))
				{
					queueIndex.presentQueueIndex = i;
					break;
				}
			}
			if (!queueIndex.presentQueueIndex)
			{
				MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "no find present");
			}
			for (size_t i = 0;i < queueFamilyProperties.size();++i)
			{
				if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eTransfer))
				{
					queueIndex.transferQueueIndex = i;
					break;
				}
			}
			if (!queueIndex.transferQueueIndex)
			{
				MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "no find transfer");
			}
			for (size_t i = 0;i < queueFamilyProperties.size();++i)
			{
				if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eCompute))
				{
					queueIndex.computeQueueIndex = i;
					break;
				}
			}
			if (!queueIndex.computeQueueIndex)
			{
				MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "no find compute");
			}
			return queueIndex;
		}
	};
}