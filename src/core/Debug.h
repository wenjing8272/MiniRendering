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
				MR_LOG_ERROR("IDName: {} Message: {}", pCallbackData->pMessageIdName, pCallbackData->pMessage);
			}
			else
			{
				MR_LOG_ERROR("IDName: {} Message: {}", pCallbackData->pMessageIdName, pCallbackData->pMessage);
			}
			return vk::False;
		}
#ifndef NDEBUG
		vk::raii::DebugUtilsMessengerEXT m_debugMessenger{ nullptr };
#endif 
	};
}