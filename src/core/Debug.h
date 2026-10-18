#pragma once
#include<vulkan/vulkan_raii.hpp>
#include<string>
#include<Logger.h>
#include"Validation.h"

namespace MiniRendering {
	
	class Debug
	{
	public:
		Debug() = default;
		~Debug();
		Debug(const Debug& other) = delete;
		Debug& operator=(const Debug& other) = delete;
#ifndef NDEBUG
		void setupDebugMessenger(const vk::raii::Instance& instance);
#endif 
	private:
		static VKAPI_ATTR vk::Bool32 VKAPI_CALL debugCallback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity, 
															  vk::DebugUtilsMessageTypeFlagsEXT type, 
															  const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData, 
															  void*)
		{
			if (severity == vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			{
				MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::error, std::string("IDName: ") + pCallbackData->pMessageIdName + "Message: " + pCallbackData->pMessage);
			}
			else
			{
				MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::warn, std::string("IDName: ") + pCallbackData->pMessageIdName + "Message: " +  pCallbackData->pMessage);
			}
			return vk::False;
		}
#ifndef NDEBUG
		vk::raii::DebugUtilsMessengerEXT m_debugMessenger{ nullptr };
#endif 
	};
}