#include"Context.h"
#include"WindowSystem.h"
#include<Logger.h>

namespace MiniRendering {

	Context::Context()
	{
		initContext();
	}
	
	Context::Context(Context&& other) noexcept
	{
		m_context = std::move(other.m_context);
		m_instance = std::move(other.m_instance);
#ifndef NDEBUG
		m_debugMessenger = std::move(other.m_debugMessenger);
#endif // !defined(NDEBUG)
		m_physicalDevice = std::move(other.m_physicalDevice);
	}

	Context& Context::operator=(Context&& other) noexcept
	{
		if (this != &other)
		{
			m_context = std::move(other.m_context);
			m_instance = std::move(other.m_instance);
#ifndef NDEBUG
			m_debugMessenger = std::move(other.m_debugMessenger);
#endif // !defined(NDEBUG)
			m_physicalDevice = std::move(other.m_physicalDevice);
		}
		return *this;
	}

	Context::~Context()
	{
		MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::info, "destroy vulkan context");
	}
	void Context::initContext()
	{
		createInstance();
		pickPhysicalDevice();
	}

	void Context::createInstance()
	{
		constexpr vk::ApplicationInfo appInfo{
			.pApplicationName = "MiniRendering",
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = "MiniRendering",
			.engineVersion = VK_MAKE_VERSION(1, 0, 0),
			.apiVersion = vk::ApiVersion14
		};
		std::vector<const char*>requiredLayer;
		if (enableValidationLayers)
		{
			requiredLayer.assign(validationLayers.begin(), validationLayers.end());
		}
		auto layerProperties = m_context.enumerateInstanceLayerProperties();
		auto unsupportedLayerIt = std::ranges::find_if(requiredLayer,     
			[&layerProperties](auto const& requiredLayer) {				
				return std::ranges::none_of(layerProperties,			
					[requiredLayer](auto const& layerProperty) { return strcmp(layerProperty.layerName, requiredLayer) == 0; });
			});
		if (unsupportedLayerIt != requiredLayer.end())
		{
			MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal,"Required layer not supported: " + std::string(*unsupportedLayerIt));    //终止，并打印不支持的扩展
		}

		uint32_t glfwExtensionCount = 0;
		auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
		if (enableValidationLayers)
		{
			requiredExtensions.push_back(vk::EXTDebugUtilsExtensionName);
		}

		auto extensionProperties = m_context.enumerateInstanceExtensionProperties();
		auto unsupportedExtensionIt = std::ranges::find_if(requiredExtensions,
			[&extensionProperties](auto const& requiredExtension) {
				return std::ranges::none_of(extensionProperties,
					[requiredExtension](auto const& extensionProperty) { return strcmp(extensionProperty.extensionName, requiredExtension) == 0; });
			});

		if (unsupportedExtensionIt != requiredExtensions.end())
		{
			MiniRendering::LogSystem::log(MiniRendering::LogSystem::LogLevel::fatal, "Required extension not supported: " + std::string(*unsupportedExtensionIt));
		}
		vk::InstanceCreateInfo instanceCreateInfo{
			.pApplicationInfo = &appInfo,
			.enabledLayerCount = static_cast<uint32_t>(requiredLayer.size()),
			.ppEnabledLayerNames = requiredLayer.data(),
			.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size()),
			.ppEnabledExtensionNames = requiredExtensions.data()
		};
		m_instance = vk::raii::Instance(m_context, instanceCreateInfo);
	}
	void Context::pickPhysicalDevice()
	{

	}
} // namespace MiniRendering