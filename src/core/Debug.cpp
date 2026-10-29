#include"core/Debug.h"
#include"utils/Logger.h"

namespace MiniRendering {
	Debug::~Debug(){}
#ifndef NDEBUG
	void Debug::setupDebugMessenger(const vk::raii::Instance& instance)
	{
		if (enableValidationLayers)
		{
			//订阅消息的严重程度，在错误或者警告下，才调用
			vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(vk::DebugUtilsMessageSeverityFlagBitsEXT::eError |
				vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning);
			//eValidation	违反规范
			//ePerformance	性能问题
			//eGeneral	    其它一般消息（如设备丢失警告）
			//订阅这些消息类型
			vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
				vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
				vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral);
			vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{
				.messageSeverity = severityFlags,
				.messageType = messageTypeFlags,
				.pfnUserCallback = &debugCallback,
			};
			m_debugMessenger = vk::raii::DebugUtilsMessengerEXT(instance, debugUtilsMessengerCreateInfoEXT);
		}
	}
#endif
}