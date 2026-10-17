#pragma once
#include<optional>
#include<string>
#include<map>
#include<algorithm>
#include<Logger.h>
#include<vulkan/vulkan_raii.hpp>

namespace MiniRendering {
	struct QueueRole {
		vk::QueueFlagBits required;  //需要的队列
		vk::QueueFlags avoid;        //不必要的队列
	};

	struct QueuePosition {
		uint32_t familyIndex = 0;
		uint32_t queueIndex = 0;
	};

	struct QueueIndex{
		std::optional<QueuePosition> graphicsAndPresent;
		std::optional<QueuePosition> transfer;
		std::optional<QueuePosition> compute;

		bool isComplete() const
		{
			return graphicsAndPresent && transfer && compute;
		}
		static QueueIndex find(const vk::raii::PhysicalDevice& physical, const vk::raii::SurfaceKHR& surface)
		{
			const auto queueProperties = physical.getQueueFamilyProperties();

			constexpr QueueRole kTransfer{ .required = vk::QueueFlagBits::eTransfer,.avoid = vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eCompute };
			constexpr QueueRole kCompute{ .required = vk::QueueFlagBits::eCompute,.avoid = vk::QueueFlagBits::eGraphics | vk::QueueFlagBits::eTransfer };

			const uint32_t transferFamily = pickByRole(queueProperties, kTransfer, "transfer");
			const uint32_t computeFamily = pickByRole(queueProperties, kCompute, "compute");
			const uint32_t graphicsAndPresentFamily = pickPresentAndGraphics(physical, surface, queueProperties);
			//创建hash表，Map在访问nextLocalIndex[family]时如果为空会把值初始化为0
			std::map<uint32_t, uint32_t> nextLocalIndex;      
			auto claim = [&](uint32_t family) -> QueuePosition {
				return QueuePosition{ family, nextLocalIndex[family]++ };//如果family相同值偏移一位
				};
			QueueIndex queueIndex;
			queueIndex.graphicsAndPresent = claim(graphicsAndPresentFamily);
			queueIndex.transfer = claim(transferFamily);
			queueIndex.compute = claim(computeFamily);

			//验证是否超过队列族的队列上限
			for (const auto& [family, count] : nextLocalIndex)
			{
				if (count > queueProperties[family].queueCount) {
					MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, 
						"queue family: " + std::to_string(family) + "need queue counts: " + std::to_string(count) + "but queue family only have queue count: " + std::to_string(queueProperties[family].queueCount)
						);
				}
			}
			return queueIndex;
		}
		std::map<uint32_t, uint32_t> queueFamilyCounts()const
		{
			std::map<uint32_t, uint32_t> counts;
			const std::optional<QueuePosition> queueInformations[] = { graphicsAndPresent,transfer,compute };
			for (const auto& queueLocation : queueInformations)
			{
				if (queueLocation)counts[queueLocation->familyIndex] = std::max(counts[queueLocation->familyIndex], queueLocation->queueIndex + 1);
			}
			return counts;
		}
	private:
		//通过role寻找队列族，满足最低required，才进行下一步
		static uint32_t pickByRole(const std::vector<vk::QueueFamilyProperties>& queueProperties, const QueueRole role, const char* name)
		{
			std::optional<uint32_t> best;
			for (uint32_t i = 0;i < queueProperties.size();++i)
			{
				if (!(queueProperties[i].queueFlags & role.required))continue;
				else if (!(queueProperties[i].queueFlags & role.avoid))return i;
				else best = i;
			}
			if (!best)
			{
				MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, std::string("no queue family supports") + name);
			}
			return *best;
		}
		//寻找同时支持Graphics和Present的队列族
		static uint32_t pickPresentAndGraphics(const vk::raii::PhysicalDevice& physical, const vk::raii::SurfaceKHR& surface, const std::vector<vk::QueueFamilyProperties>& queueProperties)
		{
			for (uint32_t i = 0;i < queueProperties.size();++i)
			{
				if ((queueProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) && (physical.getSurfaceSupportKHR(i, surface)))
				{
					return i;
				}
			}
			MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "fail to find present and graphics queue");
			return 0;
		}
	};
}